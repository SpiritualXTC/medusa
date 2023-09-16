from __future__ import annotations

import ctypes
import logging
from typing import TYPE_CHECKING

import sdl2

from medusa.vulkan import vulkan as vk
from medusa.vulkan import extensions as vkx
from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.core.window import Window
    from medusa.vulkan.context import Instance


logger = logging.getLogger(__name__)


# def surface_xlib():
#     print("Create Xlib surface")
#     vkCreateXlibSurfaceKHR = vk.vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR")
#     surface_create = VkXlibSurfaceCreateInfoKHR(
#         sType=VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
#         dpy=wm_info.info.x11.display,
#         window=wm_info.info.x11.window,
#         flags=0)
#     return vkCreateXlibSurfaceKHR(instance, surface_create, None)
#
# def surface_wayland():
#     print("Create wayland surface")
#     vkCreateWaylandSurfaceKHR = vk.vkGetInstanceProcAddr(instance, "vkCreateWaylandSurfaceKHR")
#     surface_create = VkWaylandSurfaceCreateInfoKHR(
#         sType=VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
#         display=wm_info.info.wl.display,
#         surface=wm_info.info.wl.surface,
#         flags=0)
#     return vkCreateWaylandSurfaceKHR(instance, surface_create, None)


def surface_win32(wm_info: sdl2.SDL_SysWMinfo, instance: Instance):
    def get_instance(hWnd):
        """Hack needed before SDL 2.0.6"""
        from cffi import FFI

        _ffi = FFI()
        _ffi.cdef("long __stdcall GetWindowLongA(void* hWnd, int nIndex);")
        _lib = _ffi.dlopen("User32.dll")
        return _lib.GetWindowLongA(_ffi.cast("void*", hWnd), -6)

    surface_create = vk.VkWin32SurfaceCreateInfoKHR(
        hinstance=get_instance(wm_info.info.win.window), hwnd=wm_info.info.win.window, flags=0
    )
    return vkx.vkCreateWin32SurfaceKHR(instance.handle, surface_create, None)


class Surface(object):
    def __init__(self, window: Window, instance: Instance):
        self.__surface: VulkanHandle = None

        self.__instance: Instance = instance

        wm_info = sdl2.SDL_SysWMinfo()
        sdl2.SDL_VERSION(wm_info.version)
        sdl2.SDL_GetWindowWMInfo(window.handle, ctypes.byref(wm_info))

        self.__surface = surface_win32(wm_info, instance)
        logger.info("Vulkan Surface: Created [Win32]")

    def __del__(self):
        if self.__surface:
            vkx.vkDestroySurfaceKHR(self.__instance.handle, self.__surface, None)
            logger.info("Vulkan Surface: Destroyed")

        self.__surface = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__surface
