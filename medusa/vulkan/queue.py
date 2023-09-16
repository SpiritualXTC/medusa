from __future__ import annotations

from typing import TYPE_CHECKING

from medusa.vulkan import vulkan as vk

if TYPE_CHECKING:
    from medusa.vulkan.context import LogicalDevice


class QueueFamily(object):
    def __init__(self, logical_device: LogicalDevice, family_index: int, queue_index: int = 0):
        self.__queue_family = vk.vkGetDeviceQueue(logical_device.handle, family_index, queue_index)

    def __del__(self):
        self.__queue_family = None
