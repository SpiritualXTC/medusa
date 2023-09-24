from __future__ import annotations

import logging
from typing import TYPE_CHECKING, List

from medusa.vulkan import vulkan as vk

from medusa.vulkan.frame_buffer import FrameBuffer
from medusa.vulkan.render_pass import RenderPass
from medusa.vulkan.surface import SurfaceInfo
from medusa.vulkan.swap_chain import SwapChain, FrameSync, Frame

if TYPE_CHECKING:
    from medusa.core.window import Window
    from medusa.vulkan.context import Context
    from medusa.vulkan.command_buffer import CommandBuffer


logger = logging.getLogger(__name__)


class Renderer(object):
    def __init__(self, window: Window, context: Context):

        self.__window: Window = window
        self.__context: Context = context

        self.__surface_info: SurfaceInfo = None

        self.__swap_chain: SwapChain = None
        self.__render_pass: RenderPass = None

        self.__frame_buffer: List[FrameBuffer] = None

        self.recreate_swap_chain()

        command_buffer = self.__context.graphics_command_pool.allocate(2)
        # TODO: Would be nice if this could be run by the SwapChain.
        self.__sync = [FrameSync(context, cmd_buf) for cmd_buf in command_buffer]

        self.__current_frame: Frame = None  # Current frame in the swap chain
        self.__current_sync: FrameSync = None  # Current sync
        self._temp_frame_index = 0

    def recreate_swap_chain(self):
        surface_format, surface_color_space = self.__context.physical_device.select_swap_surface_format()
        depth_format = self.__context.physical_device.select_swap_depth_format()
        width, height = self.__context.physical_device.calc_surface_extent(vk.VkExtent2D(*self.__window.size))

        self.__surface_info = SurfaceInfo(
            surface_format=surface_format,
            surface_colour_space=surface_color_space,
            depth_format=depth_format,
            width=width,
            height=height,
        )

        self.__swap_chain: SwapChain = SwapChain(self.__context, self.__surface_info)
        self.__render_pass: RenderPass = RenderPass(self.__context, self.__surface_info)
        self.__frame_buffer = [FrameBuffer(self.__context, self.__render_pass, frame) for frame in self.__swap_chain.iter_frames()]

        self._temp_frame_index = 0

    def begin_frame(self) -> CommandBuffer:
        # This returns a special sync object with the command buffers etc...
        self.__current_sync = self.__sync[self._temp_frame_index]

        self.__current_frame = self.__swap_chain.acquire_next_frame(self.__current_sync)
        if self.__current_frame is None:
            # recreate?
            raise MemoryError("Swap Chain Requires recreating?")

        # Begin Command Buffer
        self.__current_sync.render_queue.begin()
        return self.__current_sync.render_queue

    def end_frame(self) -> None:
        if self.__current_frame is None:
            logger.error("No Current Frame")
            return

        # End Command buffer
        self.__current_sync.render_queue.end()

        # Present the frame
        self.__swap_chain.present(self.__current_sync.render_queue, self.__current_sync, self.__current_frame)

        # Update counters and dereference
        self._temp_frame_index = (self._temp_frame_index + 1) % 2

        self.__current_sync = None
        self.__current_frame = None
        return

    def begin_render_pass(self) -> None:
        self.__render_pass.begin(self.__frame_buffer[self.__current_frame.image_index], self.__current_sync.render_queue)
        return

    def end_render_pass(self) -> None:
        self.__render_pass.end()
        return

    @property
    def render_pass(self) -> RenderPass:
        return self.__render_pass
