from __future__ import annotations

import logging
from typing import Any

import sdl2
import sdl2.ext


logger = logging.getLogger(__name__)


# TODO: Handle Platform Abstractions via some lightweight abstractions
class Window(object):
    def __init__(self, width: int, height: int):

        self.__window = None

        logger.debug("Initialise SDL2")
        if sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING) != 0:
            raise Exception(sdl2.SDL_GetError())

        self.__window = sdl2.SDL_CreateWindow(
            "launcher".encode("ascii"),
            sdl2.SDL_WINDOWPOS_CENTERED,
            sdl2.SDL_WINDOWPOS_CENTERED,
            width,
            height,
            sdl2.SDL_WINDOW_VULKAN | sdl2.SDL_WINDOW_SHOWN,
        )
        logger.debug("Window: Created")

    def __del__(self):
        if self.__window:
            sdl2.SDL_DestroyWindow(self.__window)
            logger.debug("Window: Destroyed")

        self.__window = None

    def poll_events(self) -> bool:

        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                return False

        return True

    @property
    def handle(self) -> Any:
        return self.__window
