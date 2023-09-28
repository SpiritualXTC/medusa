from __future__ import annotations

import logging
from enum import Flag
from typing import TYPE_CHECKING, Any

import numpy as np
from vulkan import ffi  # TODO: Maybe wrap? Better import

from medusa.core.exceptions import MedusaError
from medusa.vulkan import vulkan as vk
from medusa.vulkan.memory import Memory, MemoryFlags
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context


logger = logging.getLogger(__name__)


class BufferUsage(Flag):
    TransferSource = vk.VK_BUFFER_USAGE_TRANSFER_SRC_BIT
    TransferDestination = vk.VK_BUFFER_USAGE_TRANSFER_DST_BIT

    UniformBuffer = vk.VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
    StorageBuffer = vk.VK_BUFFER_USAGE_STORAGE_BUFFER_BIT

    IndexBuffer = vk.VK_BUFFER_USAGE_INDEX_BUFFER_BIT
    VertexBuffer = vk.VK_BUFFER_USAGE_VERTEX_BUFFER_BIT


class BufferView(object):
    def __init__(self, memory: Memory, offset: int, size: int):
        self.__memory: Memory = memory
        self.__context: Context = memory.context

        self.__offset: int = offset
        self.__size: int = size

        self.__array: np.ndarray = None
        self.__handle: Any = None

    def __del__(self):
        self.__context = None
        self.__memory = None

    def __enter__(self):
        self.map()
        return self.__handle

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.unmap()
        return False

    @property
    def offset(self):
        return self.__offset

    @property
    def size(self):
        return self.__size

    def map(self):
        self.__handle = vk.vkMapMemory(self.__context.logical_device.handle, self.__memory.handle, self.__offset, self.__size, 0)
        print(self.__handle, type(self.__handle))
        logger.info(f"Vulkan: Mapped Buffer: {self.__handle}")

    def unmap(self):
        if self.__handle:
            vk.vkUnmapMemory(self.__context.logical_device.handle, self.__memory.handle)
            logger.info(f"Vulkan: Unmapped Buffer: {self.__handle}")
        self.__handle = None

    def invalidate(self):
        # Only required for Non-Coherent
        mapped_range = vk.VkMappedMemoryRange(
            memory=self.__memory.handle,
            offset=0,
            size=0,
        )
        vk.vkInvalidateMappedMemoryRanges(self.__context.logical_device.handle, 1, [mapped_range])

    def flush(self):
        # Only required for NOn-Coherent
        mapped_range = vk.VkMappedMemoryRange(
            memory=self.__memory.handle,
            offset=0,
            size=0,
        )
        vk.vkFlushMappedMemoryRanges(self.__context.logical_device.handle, 1, [mapped_range])


class Buffer(object):
    def __init__(self, context: Context, size: int, usage: BufferUsage, memory_flags: MemoryFlags):
        self.__buffer: VulkanHandle = None
        self.__memory: Memory = None

        self.__context: Context = context

        # Create Buffer Handle
        buffer_info = vk.VkBufferCreateInfo(
            size=size,
            usage=usage.value,
            sharingMode=vk.VK_SHARING_MODE_EXCLUSIVE,
        )

        self.__buffer = vk.vkCreateBuffer(self.__context.logical_device.handle, buffer_info, None)
        logger.info(f"Vulkan Buffer: Created")

        self.__memory: Memory = Memory(self.__context, memory_flags, buffer=self.__buffer)

        vk.vkBindBufferMemory(self.__context.logical_device.handle, self.__buffer, self.__memory.handle, 0)

    def __del__(self):
        if self.__buffer:
            vk.vkDestroyBuffer(self.__context.logical_device.handle, self.__buffer, None)
            logger.info(f"Vulkan Buffer: Destroyed")

        self.__buffer = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__buffer

    @property
    def size(self) -> int:
        return self.__memory.size

    def data(self, offset: int = 0, size: int = None) -> BufferView:

        if size is None:
            size = self.__memory.size
        if offset + size > self.__memory.size:
            raise MedusaError("Attempting to retrieve pass the buffer dimensions")

        if MemoryFlags.HostVisible not in self.__memory.flags:
            raise MedusaError("Unable to map data when memory isn't accessible to the host")
        return BufferView(self.__memory, offset, size)

    def write(self, data: np.ndarray):
        buffer_size = data.size * data.itemsize
        if MemoryFlags.HostVisible in self.__memory.flags:
            # Write Directly
            with self.data(0, buffer_size) as buf:
                ptr = ffi.cast("float *", data.ctypes.data)
                ffi.memmove(buf, ptr, self.__memory.size)
        else:
            # Staging Buffer
            staging = Buffer(self.__context, buffer_size, BufferUsage.TransferSource, MemoryFlags.HostVisible | MemoryFlags.HostCoherent)
            staging.write(data)

            # Copy Via Staging
            (command_buffer,) = self.__context.graphics_command_pool.allocate(1)
            print(command_buffer, type(command_buffer))
            with command_buffer:
                region = vk.VkBufferCopy(0, 0, buffer_size)
                vk.vkCmdCopyBuffer(command_buffer.handle, staging.handle, self.handle, 1, [region])

            submit_info = vk.VkSubmitInfo(pCommandBuffers=[command_buffer.handle])
            vk.vkQueueSubmit(self.__context.logical_device.graphics_queue.handle, 1, [submit_info], vk.VK_NULL_HANDLE)
            vk.vkQueueWaitIdle(self.__context.logical_device.graphics_queue.handle)
            logger.info("Data copied")
