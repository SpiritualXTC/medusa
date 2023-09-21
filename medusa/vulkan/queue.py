from __future__ import annotations

from typing import TYPE_CHECKING

from medusa.vulkan import vulkan as vk
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import LogicalDevice


class QueueFamily(object):
    def __init__(self, logical_device: LogicalDevice, family_index: int, queue_index: int = 0):
        self.__family_index: int = family_index
        self.__queue_index: int = queue_index
        self.__queue_family = vk.vkGetDeviceQueue(logical_device.handle, family_index, queue_index)

    def __del__(self):
        self.__queue_family = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__queue_family

    @property
    def queue_family_index(self) -> int:
        return self.__family_index
