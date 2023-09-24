from __future__ import annotations

import logging
from enum import Enum
from typing import TYPE_CHECKING, Dict

from medusa.vulkan import vulkan as vk
from medusa.vulkan.context import Context
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.pipeline import PipelineStage


logger = logging.getLogger(__name__)


class DescriptorType(Enum):
    # TODO: There are more, but no idea how most of them are used...

    StorageImage = vk.VK_DESCRIPTOR_TYPE_STORAGE_IMAGE  # Image Resource via an Image View [IE G-Buffer?]

    Sampler = vk.VK_DESCRIPTOR_TYPE_SAMPLER
    SampledImage = vk.VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE  # Same as StorageImage, but supports sampling ops [ G-Buffer? ]

    UniformBuffer = vk.VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
    StorageBuffer = vk.VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
    # UniformBufferDynamic = vk.VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
    # StorageBufferDynamic = vk.VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC'
    InputAttachment = vk.VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT  # G-Buffer Write?


class DescriptorSetLayout(object):
    def __init__(self, context: Context):
        self.__descriptor_set_layout: VulkanHandle = None

        self.__context: Context = context

        self.__bindings: Dict[int, vk.VkDescriptorSetLayoutBinding] | None = None

    def __del__(self):
        if self.__descriptor_set_layout:
            vk.vkDestroyDescriptorSetLayout(self.__context.logical_device.handle, self.__descriptor_set_layout, None)
            logger.info("Vulkan Descriptor Set: Destroyed")

        self.__descriptor_set_layout = None
        self.__context = None

    def __enter__(self):
        if self.__descriptor_set_layout:
            raise ValueError("Descriptor set already created.")
        self.__bindings = {}
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        bindings = [b for b in self.__bindings.values()]

        layout_info = vk.VkDescriptorSetLayoutCreateInfo(
            pBindings=bindings,
        )

        self.__descriptor_set_layout = vk.vkCreateDescriptorSetLayout(self.__context.logical_device.handle, layout_info, None)
        logger.info(f"Vulkan Descriptor Set: Created with {len(bindings)} bindings")

        return False

    @property
    def handle(self):
        return self.__descriptor_set_layout

    def add_binding(self, binding: int, descriptor_type: DescriptorType, stage_flags: PipelineStage, count: int = 1) -> DescriptorSetLayout:
        if self.__bindings is None:
            raise ValueError("Bindings must be added from within a ContextManager (with block)")
        if binding in self.__bindings:
            raise ValueError("Binding in use")

        layout_binding = vk.VkDescriptorSetLayoutBinding(
            binding=binding,
            descriptorType=descriptor_type.value,
            descriptorCount=count,
            stageFlags=stage_flags.value,
        )
        self.__bindings[binding] = layout_binding

        return self
