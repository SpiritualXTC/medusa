import logging
from enum import Flag

from medusa.vulkan import vulkan as vk
from medusa.vulkan.context import Context
from medusa.vulkan.vulkan import VulkanHandle


logger = logging.getLogger(__name__)


class MemoryFlags(Flag):
    DeviceLocal = vk.VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

    HostVisible = vk.VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  # Mappable
    HostCoherent = vk.VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  # Flush/Invalidate not required
    HostCached = vk.VK_MEMORY_PROPERTY_HOST_CACHED_BIT  # Cached


class Memory(object):
    def __init__(self, context: Context, memory_flags: MemoryFlags, *, buffer: VulkanHandle = None, image: VulkanHandle = None):

        self.__memory: VulkanHandle = None
        self.__memory_flags: MemoryFlags = memory_flags

        self.__context: Context = context

        if buffer and image:
            raise ValueError("Buffer and image parameters are mutually exclusive")
        if buffer:
            memory_requirements = vk.vkGetBufferMemoryRequirements(self.__context.logical_device.handle, buffer)
        elif image:
            memory_requirements = vk.vkGetImageMemoryRequirements(self.__context.logical_device.handle, image)
        else:
            raise ValueError("Either buffer or image must be a valid handle")

        memory_index = self.__context.find_memory_type(memory_requirements.memoryTypeBits, memory_flags.value)

        # Allocate Memory [TODO: PyVMA]
        alloc_info = vk.VkMemoryAllocateInfo(
            allocationSize=memory_requirements.size,
            memoryTypeIndex=memory_index,
        )

        self.__memory = vk.vkAllocateMemory(self.__context.logical_device.handle, alloc_info, None)
        logger.info(f"Vulkan: Allocated {memory_requirements.size} bytes of memory")

        self.__size: int = memory_requirements.size

    def __del__(self):
        if self.__memory:
            vk.vkFreeMemory(self.__context.logical_device.handle, self.__memory, None)
            logger.info(f"Vulkan: Freed memory")

        self.__memory = None
        self.__context = None

    @property
    def context(self):
        return self.__context

    @property
    def handle(self):
        return self.__memory

    @property
    def size(self):
        return self.__size

    @property
    def flags(self) -> MemoryFlags:
        return self.__memory_flags
