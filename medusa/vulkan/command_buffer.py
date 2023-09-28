from __future__ import annotations

import logging
from typing import TYPE_CHECKING, Tuple

from medusa.vulkan import vulkan as vk
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context
    from medusa.vulkan.queue import QueueFamily


logger = logging.getLogger(__name__)


class CommandPool(object):
    def __init__(self, context: Context, queue_family: QueueFamily):
        self.__pool: VulkanHandle = None

        self.__context: Context = context

        pool_info = vk.VkCommandPoolCreateInfo(
            queueFamilyIndex=queue_family.queue_family_index,
            flags=vk.VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | vk.VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        )

        self.__pool = vk.vkCreateCommandPool(self.__context.logical_device.handle, pool_info, None)
        logger.info("Vulkan Command Pool: Created")

    def __del__(self):
        if self.__pool:
            vk.vkDestroyCommandPool(self.__context.logical_device.handle, self.__pool, None)
            logger.info("Vulkan Command Pool: Destroyed")

        self.__pool = None
        self.__context = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__pool

    def allocate(self, count: int = 1) -> Tuple[CommandBuffer, ...]:

        command_buffer_allocate_info = vk.VkCommandBufferAllocateInfo(
            level=vk.VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            commandPool=self.handle,
            commandBufferCount=count,
        )

        command_buffers = vk.vkAllocateCommandBuffers(self.__context.logical_device.handle, command_buffer_allocate_info)
        logger.info(f"Vulkan Command Pool: Allocated {len(command_buffers)} command buffers")
        return tuple((CommandBuffer(self.__context, self, handle) for handle in command_buffers))


class CommandBuffer(object):
    # TODO: Setup a "one-time-use" allocator?
    def __init__(self, context: Context, command_pool: CommandPool, command_buffer_handle: VulkanHandle):
        self.__context: Context = context
        self.__command_pool: CommandPool = command_pool
        self.__command_buffer: VulkanHandle = command_buffer_handle

    def __del__(self):
        if self.__command_buffer:
            vk.vkFreeCommandBuffers(self.__context.logical_device.handle, self.__command_pool.handle, 1, [self.__command_buffer])
        self.__command_buffer = None
        self.__command_pool = None
        self.__context = None

    def __enter__(self):
        self.begin()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.end()
        return False

    @property
    def handle(self) -> VulkanHandle:
        return self.__command_buffer

    def begin(self):
        # command_buffer = self.__command_buffers[self._temp_frame_index]
        begin_info = vk.VkCommandBufferBeginInfo()

        vk.vkBeginCommandBuffer(self.__command_buffer, begin_info)

    def end(self):
        vk.vkEndCommandBuffer(self.__command_buffer)
