from __future__ import annotations

from typing import TYPE_CHECKING

from medusa.vulkan import vulkan as vk
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context


class Fence(object):
    def __init__(self, context: Context, signaled: bool = True):
        self.__context: Context = context

        info = vk.VkFenceCreateInfo(flags=vk.VK_FENCE_CREATE_SIGNALED_BIT if signaled else 0)

        self.__fence: VulkanHandle = vk.vkCreateFence(self.__context.logical_device.handle, info, None)

    def __del__(self):
        if self.__fence:
            vk.vkDestroyFence(self.__context.logical_device.handle, self.__fence, None)

        self.__fence = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__fence

    def wait(self, timeout: int = 1e9):
        vk.vkWaitForFences(self.__context.logical_device.handle, 1, [self.__fence], vk.VK_TRUE, int(timeout))

    def reset(self):
        vk.vkResetFences(self.__context.logical_device.handle, 1, [self.__fence])


class Semaphore(object):
    def __init__(self, context: Context):
        self.__context: Context = context

        info = vk.VkSemaphoreCreateInfo()

        self.__semaphore = vk.vkCreateSemaphore(self.__context.logical_device.handle, info, None)

    def __del__(self):
        if self.__semaphore:
            vk.vkDestroySemaphore(self.__context.logical_device.handle, self.__semaphore, None)

        self.__semaphore = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__semaphore
