from __future__ import annotations

import logging
from typing import Any, Tuple

import sdl2
import sdl2.ext


logger = logging.getLogger(__name__)


# TODO: Handle Platform Abstractions via some lightweight abstractions
class Window(object):
    def __init__(self, width: int, height: int):

        self.__window = None

        self.__width: int = 0
        self.__height: int = 0

        logger.info("Initialise SDL2")
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
        logger.info("Window: Created")

    def __del__(self):
        if self.__window:
            sdl2.SDL_DestroyWindow(self.__window)
            logger.info("Window: Destroyed")

        self.__window = None

    def poll_events(self) -> bool:

        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                return False
            if event.type == sdl2.SDL_WINDOWEVENT_RESIZED:
                logger.warning("Window resized. Swap Chain is NOT being resized as of yet")

        return True

    @property
    def handle(self) -> Any:
        return self.__window

    @property
    def size(self) -> Tuple[int, int]:
        return self.__width, self.__height

    def set_title(self, title: str):
        sdl2.SDL_SetWindowTitle(self.__window, title.encode("utf-8"))
