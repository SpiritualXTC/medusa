from __future__ import annotations

import logging
from typing import TYPE_CHECKING

from medusa.vulkan import vulkan as vk

from medusa.vulkan.surface import SurfaceInfo

from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context
    from medusa.vulkan.command_buffer import CommandBuffer
    from medusa.vulkan.frame_buffer import FrameBuffer


logger = logging.getLogger(__name__)


class RenderPass(object):
    def __init__(self, context: Context, surface_info: SurfaceInfo):
        self.__render_pass: VulkanHandle = None

        self.__current_render_queue: CommandBuffer = None
        self.__context: Context = context

        depth_attachment = vk.VkAttachmentDescription(
            format=surface_info.depth_format,
            samples=vk.VK_SAMPLE_COUNT_1_BIT,
            loadOp=vk.VK_ATTACHMENT_LOAD_OP_CLEAR,
            storeOp=vk.VK_ATTACHMENT_STORE_OP_DONT_CARE,
            stencilLoadOp=vk.VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            stencilStoreOp=vk.VK_ATTACHMENT_STORE_OP_DONT_CARE,
            initialLayout=vk.VK_IMAGE_LAYOUT_UNDEFINED,
            finalLayout=vk.VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        )

        depth_attachment_ref = vk.VkAttachmentReference(
            attachment=1,
            layout=vk.VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        )

        colour_attachment = vk.VkAttachmentDescription(
            format=surface_info.surface_format,
            samples=vk.VK_SAMPLE_COUNT_1_BIT,
            loadOp=vk.VK_ATTACHMENT_LOAD_OP_CLEAR,
            storeOp=vk.VK_ATTACHMENT_STORE_OP_STORE,
            stencilLoadOp=vk.VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            stencilStoreOp=vk.VK_ATTACHMENT_STORE_OP_DONT_CARE,
            initialLayout=vk.VK_IMAGE_LAYOUT_UNDEFINED,
            finalLayout=vk.VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        )

        colour_attachment_ref = vk.VkAttachmentReference(
            attachment=0,
            layout=vk.VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        )

        sub_pass = vk.VkSubpassDescription(
            pipelineBindPoint=vk.VK_PIPELINE_BIND_POINT_GRAPHICS,
            pColorAttachments=[colour_attachment_ref],
            pDepthStencilAttachment=[depth_attachment_ref],
        )

        sub_pass_dependency = vk.VkSubpassDependency(
            srcSubpass=vk.VK_SUBPASS_EXTERNAL,
            srcAccessMask=0,
            srcStageMask=vk.VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | vk.VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            dstSubpass=0,
            dstStageMask=vk.VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | vk.VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            dstAccessMask=vk.VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | vk.VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        )

        render_pass_info = vk.VkRenderPassCreateInfo(
            pAttachments=[colour_attachment, depth_attachment],
            pSubpasses=[sub_pass],
            pDependencies=[sub_pass_dependency],
        )

        self.__render_pass = vk.vkCreateRenderPass(self.__context.logical_device.handle, render_pass_info, None)
        logger.info("Vulkan Render Pass: Created")

        self.__viewport = vk.VkViewport(0.0, 0.0, float(surface_info.width), float(surface_info.height), 0.0, 1.0)
        self.__region = vk.VkRect2D(vk.VkOffset2D(0, 0), vk.VkExtent2D(surface_info.width, surface_info.height))
        self.__clear_color = vk.VkClearColorValue(float32=[0.2, 0.2, 0.2, 1])
        self.__clear_depth = vk.VkClearDepthStencilValue(1.0, 0)

    def __del__(self):
        if self.__render_pass:
            vk.vkDestroyRenderPass(self.__context.logical_device.handle, self.__render_pass, None)
            logger.info("Vulkan Render Pass: Destroyed")

        self.__render_pass = None
        self.__current_render_queue: CommandBuffer = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__render_pass

    def begin(self, frame_buffer: FrameBuffer, render_queue: CommandBuffer):
        self.__current_render_queue = render_queue

        # render_area = vk.VkRect2D(offset=vk.VkOffset2D(x=0, y=0), extent=self.__surface_info.get_extent())
        # clear_color = vk.VkClearColorValue(float32=[0.2, 0.2, 0.2, 1])
        # clear_depth = vk.VkClearDepthStencilValue(1.0, 0)
        #
        render_pass_begin_info = vk.VkRenderPassBeginInfo(
            renderPass=self.__render_pass,
            framebuffer=frame_buffer.handle,  # Combine the frame buffer with Frame? Kinda annoying
            renderArea=self.__region,
            pClearValues=[vk.VkClearValue(color=self.__clear_color), vk.VkClearValue(depthStencil=self.__clear_depth)],
        )

        vk.vkCmdBeginRenderPass(render_queue.handle, render_pass_begin_info, vk.VK_SUBPASS_CONTENTS_INLINE)

        vk.vkCmdSetViewport(render_queue.handle, 0, 1, [self.__viewport])
        vk.vkCmdSetScissor(render_queue.handle, 0, 1, [self.__region])

    def end(self):
        vk.vkCmdEndRenderPass(self.__current_render_queue.handle)

        self.__current_render_queue = None
