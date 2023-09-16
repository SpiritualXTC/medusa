from __future__ import annotations

import logging
import sys

from typing import TYPE_CHECKING, Set, Tuple

from medusa.vulkan import vulkan as vk
from medusa.vulkan import extensions as vkx

from medusa.core.exceptions import MedusaError
from medusa.vulkan.extensions import InstanceProcAddr
from medusa.vulkan.queue import QueueFamily
from medusa.vulkan.surface import Surface
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.core.window import Window


logger = logging.getLogger(__name__)


class Instance(object):
    @staticmethod
    def __get_enabled_layers() -> Set[str]:
        required = {"VK_LAYER_KHRONOS_validation"}
        available = {layer.layerName for layer in vk.vkEnumerateInstanceLayerProperties()}

        layers = available & required

        if (layers & required) != required:
            raise MedusaError(f"Missing instance layers {required & layers}")

        return layers

    @staticmethod
    def __get_enabled_extensions() -> Set[str]:
        required = {"VK_EXT_debug_report", "VK_KHR_surface"}
        if sys.platform == "win32":
            required |= {"VK_KHR_win32_surface"}

        available = {ext.extensionName for ext in vk.vkEnumerateInstanceExtensionProperties(None)}

        extensions = available & required
        if (extensions & required) != required:
            raise ValueError(f"Missing extensions {required & extensions}")
        return extensions

    def __init__(self, enable_validation_layers: bool):

        self.__instance: VulkanHandle = None

        self.__layers: Set[str] = Instance.__get_enabled_layers()
        self.__extensions: Set[str] = Instance.__get_enabled_extensions()

        app_info = vk.VkApplicationInfo(
            pApplicationName="Medusa",
            applicationVersion=vk.VK_MAKE_VERSION(0, 0, 0),
            pEngineName="Medusa",
            engineVersion=vk.VK_MAKE_VERSION(0, 0, 0),
            apiVersion=vk.VK_API_VERSION_1_0,
        )

        if enable_validation_layers:
            msg_severity = (
                vk.VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | vk.VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
            )
            msg_type = (
                vk.VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | vk.VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | vk.VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            )

            debug_info = vk.VkDebugUtilsMessengerCreateInfoEXT(
                messageSeverity=msg_severity,
                messageType=msg_type,
                pfnUserCallback=self._debug_callback,
            )
        else:
            debug_info = None

        create_info = vk.VkInstanceCreateInfo(
            pNext=debug_info,
            pApplicationInfo=app_info,
            ppEnabledLayerNames=self.__layers,
            ppEnabledExtensionNames=self.__extensions,
        )

        # Create Instance
        self.__instance = vk.vkCreateInstance(create_info, None)
        logger.info("Vulkan Instance: Created")

        InstanceProcAddr.T = self.__instance

    def __del__(self):
        if self.__instance:
            vk.vkDestroyInstance(self.__instance, None)
            logger.info("Vulkan Instance: Destroyed")

        self.__instance = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__instance

    def _debug_callback(self, *args):
        logging.debug(f"{args}")
        return 0

    def get_enabled_layers(self) -> Tuple[str]:
        return tuple(self.__layers)


class PhysicalDevice(object):
    @staticmethod
    def __queue_family_indices(device_handle: VulkanHandle, surface_handle: VulkanHandle) -> Tuple[int, int, int]:
        graphics_family: int = None
        present_family: int = None
        compute_family: int = None

        queue_family_props = vk.vkGetPhysicalDeviceQueueFamilyProperties(device_handle)

        for idx, queue_family in enumerate(queue_family_props):
            if queue_family.queueCount == 0:
                continue

            if queue_family.queueFlags & vk.VK_QUEUE_GRAPHICS_BIT:
                graphics_family = idx

            if queue_family.queueFlags & vk.VK_QUEUE_COMPUTE_BIT:
                compute_family = idx

            present_support = vkx.vkGetPhysicalDeviceSurfaceSupportKHR(device_handle, idx, surface_handle)
            if present_support:
                present_family = idx

            if None not in (graphics_family, present_family, compute_family):
                return graphics_family, present_family, compute_family

    @staticmethod
    def __has_swap_chain_support(device_handle: VulkanHandle, surface_handle: VulkanHandle) -> bool:
        """Device must support at least one format and present mode"""
        # swap_chain_caps = vkx.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device_handle, surface_handle)

        formats = vkx.vkGetPhysicalDeviceSurfaceFormatsKHR(device_handle, surface_handle)
        present_modes = vkx.vkGetPhysicalDeviceSurfacePresentModesKHR(device_handle, surface_handle)

        return formats and present_modes

    @staticmethod
    def __is_device_suitable(device_handle: VulkanHandle, surface_handle: VulkanHandle) -> bool:

        # Find a device supporting all required queue families
        queue_indices = PhysicalDevice.__queue_family_indices(device_handle, surface_handle)
        if None in queue_indices:
            return False

        # Find a device with correct swap chain support
        if not PhysicalDevice.__has_swap_chain_support(device_handle, surface_handle):
            return False

        # TODO: Also need to check for VK_KHR_SWAPCHAIN_EXTENSION_NAME in the deviceExtensions

        return True

    def __init__(self, instance: Instance, surface: Surface):
        self.__device: VulkanHandle = None
        self.__surface: Surface = surface

        devices = vk.vkEnumeratePhysicalDevices(instance.handle)
        if len(devices) == 0:
            raise MedusaError("No devices found")

        for device in devices:
            if PhysicalDevice.__is_device_suitable(device, surface.handle):
                self.__device = device
                break
        else:
            raise MedusaError("No suitable device found")

        props = vk.vkGetPhysicalDeviceProperties(self.__device)
        logger.info(f"Using device: {props.deviceName}")

    @property
    def handle(self) -> VulkanHandle:
        return self.__device

    def get_queue_family_indices(self) -> Tuple[int, int, int]:
        return PhysicalDevice.__queue_family_indices(self.__device, self.__surface.handle)


class LogicalDevice(object):
    @staticmethod
    def __get_enabled_extensions(physical_device: PhysicalDevice) -> Set[str]:
        required = {vk.VK_KHR_SWAPCHAIN_EXTENSION_NAME}
        available = {ext.extensionName for ext in vk.vkEnumerateDeviceExtensionProperties(physical_device.handle, None)}

        extensions = available & required
        if (extensions & required) != required:
            raise ValueError(f"Missing extensions {required & extensions}")
        return extensions

    def __init__(self, instance: Instance, physical_device: PhysicalDevice):
        self.__logical_device: VulkanHandle = None

        self.__instance = instance
        self.__physical_device: PhysicalDevice = physical_device

        self.__extensions: Set[str] = LogicalDevice.__get_enabled_extensions(physical_device)

        queue_indices: Tuple[int, int, int] = self.__physical_device.get_queue_family_indices()

        queue_info = []
        for queue_idx in set(queue_indices):
            q_info = vk.VkDeviceQueueCreateInfo(queueFamilyIndex=queue_idx, queueCount=1, pQueuePriorities=[1.0])

            queue_info += [q_info]

        device_info = vk.VkDeviceCreateInfo(
            pQueueCreateInfos=queue_info,
            pEnabledFeatures=vk.VkPhysicalDeviceFeatures(),
            ppEnabledLayerNames=instance.get_enabled_layers(),
            ppEnabledExtensionNames=self.__extensions,
        )

        self.__logical_device = vk.vkCreateDevice(physical_device.handle, device_info, None)
        logger.info("Logical Device: Created")

        # Get Queues
        graphics, present, compute = queue_indices

        self.__graphics_queue = QueueFamily(self, graphics)
        self.__present_queue = QueueFamily(self, present)
        self.__compute_queue = QueueFamily(self, compute)

    def __del__(self):

        self.__compute_queue = None
        self.__present_queue = None
        self.__graphics_queue = None

        if self.__logical_device:
            vk.vkDestroyDevice(self.__logical_device, None)
            logger.info("Logical Device: Destroyed")

        self.__logical_device = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__logical_device


class Context(object):
    def __init__(self, window: Window):
        if window is None:
            raise MedusaError("Invalid Window")

        self.__window: Window = window

        # Create Vulkan Instance
        self.__instance: Instance = Instance(True)
        self.__surface: Surface = Surface(self.__window, self.__instance)

        self.__physical_device: PhysicalDevice = PhysicalDevice(self.__instance, self.__surface)

        self.__logical_device: LogicalDevice = LogicalDevice(self.__instance, self.__physical_device)

    def __del__(self):

        self.__logical_device = None
        self.__physical_device = None
        self.__surface = None
        self.__instance = None
