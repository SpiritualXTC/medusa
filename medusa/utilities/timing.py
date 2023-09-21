from __future__ import annotations

from time import perf_counter
from typing import Callable

from medusa.utilities.invoke import Invoke


class Timing(object):
    def __init__(self, on_second: Callable[[Timing], None] = None):

        self._frame_last: float = 0.0
        self._frame_time: float = 0.0

        self._count: int = 0
        self._frames: int = 0
        self._timer: float = 0.0

        self._fps: int = 0
        self._callback = on_second

    @property
    def frame_time(self) -> float:
        return self._frame_time

    @property
    def frame_rate(self) -> float:
        return 1.0 / self._frame_time

    @property
    def fps(self) -> int:
        return self._fps

    @property
    def frames(self) -> int:
        return self._frames

    def update(self):

        current = perf_counter()
        self._frame_time = current - self._frame_last
        self._frame_last = current

        self._timer += self._frame_time
        self._count += 1
        if self._timer > 1.0:
            self._timer -= 1.0
            self._fps = self._count
            self._count = 0
            if self._callback:
                Invoke().invoke(self._callback, self)
        self._frames += 1
