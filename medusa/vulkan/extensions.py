import logging

import vulkan as vk


logger = logging.getLogger(__name__)


# This seems like a hack ... but a nice hack
class InstanceProcAddr(object):
    T = None

    def __init__(self, func):
        self.__func = func

    def __call__(self, *args, **kwargs):
        name = self.__func.__name__

        func = InstanceProcAddr.procfunc(name)
        if func:
            return func(*args, **kwargs)
        else:
            return vk.VK_ERROR_EXTENSION_NOT_PRESENT

    @staticmethod
    def procfunc(funcName):
        # The 'T' is a hack so it knows which instance to pull
        return vk.vkGetInstanceProcAddr(InstanceProcAddr.T, funcName)


class DeviceProcAddr(InstanceProcAddr):
    T = None

    @staticmethod
    def procfunc(funcName):
        # The 'T' is a hack so it knows which instance to pull
        return vk.vkGetDeviceProcAddr(DeviceProcAddr.T, funcName)


@InstanceProcAddr
def vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator):  # NOQA
    pass


@InstanceProcAddr
def vkDestroyDebugReportCallbackEXT(instance, pCreateInfo, pAllocator):  # NOQA
    pass


@InstanceProcAddr
def vkDestroySurfaceKHR(instance, surface, pAllocator):  # NOQA
    pass


@InstanceProcAddr
def vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface):  # NOQA
    pass


@InstanceProcAddr
def vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface):  # NOQA
    pass


@InstanceProcAddr
def vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface):  # NOQA
    pass


@InstanceProcAddr
def vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface):  # NOQA
    pass


# Device ext functions
@DeviceProcAddr
def vkCreateSwapchainKHR(device, pCreateInfo, pAllocator):  # NOQA
    pass


@DeviceProcAddr
def vkDestroySwapchainKHR(device, swapchain, pAllocator):  # NOQA
    pass


@DeviceProcAddr
def vkGetSwapchainImagesKHR(device, swapchain):  # NOQA
    pass


@DeviceProcAddr
def vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence):  # NOQA
    pass


@DeviceProcAddr
def vkQueuePresentKHR(queue, pPresentInfo):  # NOQA
    pass


# Windows Specific
@InstanceProcAddr
def vkCreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator):  # NOQA
    pass


# Linux Specific
@InstanceProcAddr
def vkCreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator):  # NOQA
    pass


# Mac Specific (via MoltenVK)
