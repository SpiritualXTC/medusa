from __future__ import annotations

import logging

from enum import Flag
from typing import TYPE_CHECKING, Any, Dict

from medusa.vulkan import vulkan as vk
from medusa.vulkan.command_buffer import CommandBuffer
from medusa.vulkan.descriptors import DescriptorSetLayout

from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context
    from medusa.vulkan.render_pass import RenderPass
    from medusa.vulkan.constants import PushConstants

    from medusa.utilities.resource import Resource


logger = logging.getLogger(__name__)


class PipelineStage(Flag):
    NotSet = 0

    Vertex = vk.VK_SHADER_STAGE_VERTEX_BIT
    Fragment = vk.VK_SHADER_STAGE_FRAGMENT_BIT
    Geometry = vk.VK_SHADER_STAGE_GEOMETRY_BIT

    Compute = vk.VK_SHADER_STAGE_COMPUTE_BIT


class PipelineShader(object):
    def __init__(self, context: Context, resource: Resource, stage: PipelineStage):
        self.__shader_module = None

        self.__context: Context = context
        self.__stage: PipelineStage = stage

        code = resource.read(binary=True)

        shader_info = vk.VkShaderModuleCreateInfo(
            codeSize=len(code),
            pCode=code,
        )

        self.__shader_module = vk.vkCreateShaderModule(context.logical_device.handle, shader_info, None)
        logger.info(f"Vulkan Shader: Created `{resource.filepath.name}`")

    def __del__(self):
        if self.__shader_module:
            vk.vkDestroyShaderModule(self.__context.logical_device.handle, self.__shader_module, None)

        self.__shader_module = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__shader_module

    @property
    def stage(self) -> PipelineStage:
        return self.__stage


class PipelineLayout(object):
    def __init__(
        self,
        context: Context,
        stage_flags: PipelineStage,
        descriptor_set_layout: DescriptorSetLayout,
        constants: PushConstants = None,
    ):
        self.__layout: VulkanHandle = None

        self.__context: Context = context

        if constants and constants.size != 0:
            # TODO: Needs to be pulled from the class. NOT the instance
            constant_range = vk.VkPushConstantRange(
                stageFlags=stage_flags.value,
                offset=0,
                size=constants.size,
            )
        else:
            constant_range = None

        pipeline_layout_info = vk.VkPipelineLayoutCreateInfo(
            pSetLayouts=[descriptor_set_layout.handle],
            pPushConstantRanges=[constant_range] if constant_range else None,
        )

        self.__layout = vk.vkCreatePipelineLayout(self.__context.logical_device.handle, pipeline_layout_info, None)
        logger.info("Vulkan Pipeline Layout: Created")

    def __del__(self):
        if self.__layout:
            vk.vkDestroyPipelineLayout(self.__context.logical_device.handle, self.__layout, None)
            logger.info("Vulkan Pipeline Layout: Created")

        self.__layout = None

    @property
    def handle(self):
        return self.__layout


class GraphicsPipeline(object):
    def __init__(self, context: Context, descriptor_set: DescriptorSetLayout, render_pass: RenderPass, constants: PushConstants = None):
        self.__pipeline: VulkanHandle = None

        self.__context: Context = context
        self.__render_pass: RenderPass = render_pass
        self.__descriptor_set: DescriptorSetLayout = descriptor_set
        self.__constants: PushConstants = constants

        self.__layout: PipelineLayout = None
        self.__stages: Dict[PipelineStage, PipelineShader] = {}

        self._input_assembly_state = None
        self._viewport_state = None
        self._rasterization_state = None
        self._multisample_state = None
        self._color_blend_attachment = None
        self._color_blend_state = None
        self._depth_stencil_state = None

        self._bindings = []
        self._attributes = []

        self._dynamic_state = []

    def __del__(self):
        if self.__pipeline:
            vk.vkDestroyPipeline(self.__context.logical_device.handle, self.__pipeline, None)
            logger.info("Vulkan Pipeline: Destroyed")

        self.__pipeline = None

        self.__descriptor_set = None
        self.__render_pass = None
        self.__context = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__pipeline

    def bind(self, command_buffer: CommandBuffer):
        vk.vkCmdBindPipeline(command_buffer.handle, vk.VK_PIPELINE_BIND_POINT_GRAPHICS, self.__pipeline)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        stage_flags = PipelineStage.NotSet
        for stage in self.__stages.keys():
            stage_flags |= stage

        self.__layout = PipelineLayout(self.__context, stage_flags, self.__descriptor_set, self.__constants)

        vertex_info = vk.VkPipelineVertexInputStateCreateInfo(
            pVertexAttributeDescriptions=self._attributes,
            pVertexBindingDescriptions=self._bindings,
        )

        pipeline_stage_info = []
        for flag, stage in self.__stages.items():
            stage_info = vk.VkPipelineShaderStageCreateInfo(
                stage=flag.value,
                module=stage.handle,
                pName=b"main",
                flags=0,
            )
            pipeline_stage_info.append(stage_info)

        pipeline_info = vk.VkGraphicsPipelineCreateInfo(
            pStages=pipeline_stage_info,
            pVertexInputState=vertex_info,
            pInputAssemblyState=self._input_assembly_state or self.set_input_assembly_info(),
            pViewportState=self._viewport_state or self.set_viewport_state(),
            pRasterizationState=self._rasterization_state or self.set_rasterization_state(),
            pMultisampleState=self._multisample_state or self.set_multisample_state(),
            pColorBlendState=self._color_blend_state or self.set_color_blend_state(),
            pDepthStencilState=self._depth_stencil_state or self.set_depth_stencil_state(),
            pDynamicState=vk.VkPipelineDynamicStateCreateInfo(
                pDynamicStates=self._dynamic_state,
                flags=0,
            ),
            layout=self.__layout.handle,
            renderPass=self.__render_pass.handle,
            subpass=0,
            basePipelineHandle=None,
            basePipelineIndex=-1,
        )

        pipelines = vk.vkCreateGraphicsPipelines(self.__context.logical_device.handle, None, 1, pipeline_info, None)
        self.__pipeline = pipelines[0]
        logger.info("Vulkan Pipeline: Created")

        return False

    def _validate_shader(self, stage: PipelineStage):
        return stage in [PipelineStage.Vertex, PipelineStage.Fragment, PipelineStage.Geometry]

    def attach_shader(self, resource: Resource, stage: PipelineStage):
        logger.info("Attaching Shader")

        if stage in self.__stages:
            raise ValueError("This has already been attached")
        if not self._validate_shader(stage):
            raise ValueError("Invalid stage for this type of shader")

        self.__stages[stage] = PipelineShader(self.__context, resource, stage)

    def add_binding(self, stride: int, binding: int = None, input_rate_vertex: bool = True):
        binding_info = vk.VkVertexInputBindingDescription(
            binding=binding if binding is not None else len(self._bindings),
            stride=stride,
            inputRate=vk.VK_VERTEX_INPUT_RATE_VERTEX if input_rate_vertex else vk.VK_VERTEX_INPUT_RATE_INSTANCE,
        )
        self._bindings.append(binding_info)

    def add_attribute(self, location: int, binding: int, fmt: int, offset: int):
        attribute_info = vk.VkVertexInputAttributeDescription(
            location=location,
            binding=binding,
            format=fmt,
            offset=offset,
        )
        self._attributes.append(attribute_info)

    def set_input_assembly_info(self) -> Any:
        input_assembly_state = vk.VkPipelineInputAssemblyStateCreateInfo(
            topology=vk.VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            primitiveRestartEnable=False,  # This allows inserting sentinels, into the stream to reset triangle/list strips.
        )
        self._input_assembly_state = input_assembly_state
        return input_assembly_state

    def set_viewport_state(self, viewport: vk.VkViewport = None, scissor: vk.VkRect2D = None) -> Any:
        viewport_state = vk.VkPipelineViewportStateCreateInfo(
            viewportCount=len(viewport) if viewport else 1,
            scissorCount=len(scissor) if scissor else 1,
            pViewports=[viewport] if viewport else None,
            pScissors=[scissor] if scissor else None,
        )
        if viewport is None:
            self._dynamic_state += [vk.VK_DYNAMIC_STATE_VIEWPORT]
        if scissor is None:
            self._dynamic_state += [vk.VK_DYNAMIC_STATE_SCISSOR]

        self._viewport_state = viewport_state
        return viewport_state

    def set_rasterization_state(self):
        rasterization_state = vk.VkPipelineRasterizationStateCreateInfo(
            rasterizerDiscardEnable=False,
            polygonMode=vk.VK_POLYGON_MODE_FILL,
            cullMode=vk.VK_CULL_MODE_NONE,
            frontFace=vk.VK_FRONT_FACE_CLOCKWISE,
            lineWidth=1.0,
            depthBiasEnable=False,
            depthBiasClamp=0.0,
            depthClampEnable=False,
            depthBiasConstantFactor=0.0,
            depthBiasSlopeFactor=0.0,
        )

        self._rasterization_state = rasterization_state
        return rasterization_state

    def set_multisample_state(self) -> Any:

        multi_sample_state = vk.VkPipelineMultisampleStateCreateInfo(
            sampleShadingEnable=False,
            rasterizationSamples=vk.VK_SAMPLE_COUNT_1_BIT,
            minSampleShading=1.0,
            pSampleMask=None,
            alphaToCoverageEnable=False,
            alphaToOneEnable=False,
        )

        self._multisample_state = multi_sample_state
        return multi_sample_state

    def set_color_blend_attachment(self) -> Any:
        if self._color_blend_state:
            raise ValueError("Set Colour Blend Attachment prior to setting the colour blend state")

        colour_mask = vk.VK_COLOR_COMPONENT_R_BIT | vk.VK_COLOR_COMPONENT_G_BIT | vk.VK_COLOR_COMPONENT_B_BIT | vk.VK_COLOR_COMPONENT_A_BIT

        colour_blend_attachment = vk.VkPipelineColorBlendAttachmentState(
            colorWriteMask=colour_mask,
            blendEnable=False,
            srcColorBlendFactor=vk.VK_BLEND_FACTOR_ONE,  # VK_BLEND_FACTOR_SRC_ALPHA
            dstColorBlendFactor=vk.VK_BLEND_FACTOR_ZERO,  # VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
            colorBlendOp=vk.VK_BLEND_OP_ADD,
            srcAlphaBlendFactor=vk.VK_BLEND_FACTOR_ONE,
            dstAlphaBlendFactor=vk.VK_BLEND_FACTOR_ZERO,
            alphaBlendOp=vk.VK_BLEND_OP_ADD,
        )

        self._color_blend_attachment = colour_blend_attachment
        return colour_blend_attachment

    def set_color_blend_state(self) -> Any:

        if not self._color_blend_attachment:
            self._color_blend_attachment = self.set_color_blend_attachment()

        colour_blend_state = vk.VkPipelineColorBlendStateCreateInfo(
            logicOpEnable=False,
            logicOp=vk.VK_LOGIC_OP_COPY,
            pAttachments=[self._color_blend_attachment],
            blendConstants=(0.0, 0.0, 0.0, 0.0),
        )

        self._color_blend_state = colour_blend_state
        return colour_blend_state

    def set_depth_stencil_state(self, depth_test_enable: bool = True, depth_write_enable: bool = None) -> Any:

        depth_stencil_state = vk.VkPipelineDepthStencilStateCreateInfo(
            depthTestEnable=depth_test_enable,
            depthWriteEnable=depth_write_enable if depth_write_enable is not None else depth_test_enable,
            depthCompareOp=vk.VK_COMPARE_OP_LESS,
            depthBoundsTestEnable=False,
            minDepthBounds=0.0,
            maxDepthBounds=1.0,
            stencilTestEnable=False,
            front=(),
            back=(),
        )

        self._depth_stencil_state = depth_stencil_state
        return depth_stencil_state
