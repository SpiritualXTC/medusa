from __future__ import annotations

import logging
from typing import TYPE_CHECKING

from medusa.vulkan import vulkan as vk
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context
    from medusa.vulkan.render_pass import RenderPass
    from medusa.vulkan.swap_chain import Frame

logger = logging.getLogger(__name__)


class FrameBuffer(object):
    def __init__(self, context: Context, render_pass: RenderPass, frame: Frame):

        self.__frame_buffer: VulkanHandle = None

        self.__context: Context = context

        attachments = frame.get_handles()

        frame_buffer_info = vk.VkFramebufferCreateInfo(
            renderPass=render_pass.handle,
            pAttachments=attachments,
            width=frame.surface_info().width,
            height=frame.surface_info().height,
            layers=1,
        )

        self.__frame_buffer = vk.vkCreateFramebuffer(self.__context.logical_device.handle, frame_buffer_info, None)
        logger.info(f"Vulkan Frame Buffer: Created {frame.surface_info().width}x{frame.surface_info().height}")

    def __del__(self):
        if self.__frame_buffer:
            vk.vkDestroyFramebuffer(self.__context.logical_device.handle, self.__frame_buffer, None)
            logger.info("Vulkan Frame Buffer: Destroyed")

        self.__frame_buffer = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__frame_buffer
