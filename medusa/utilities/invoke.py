from __future__ import annotations

from dataclasses import dataclass
from queue import Queue
from typing import Tuple, Any, Dict

from medusa.utilities.singleton import Singleton


@dataclass
class InvokeCall:
    fn: callable
    args: Tuple[Any, ...]
    kwargs: Dict[str, Any]

    def __call__(self):
        return self.fn(*self.args, **self.kwargs)


class Invoke(metaclass=Singleton):
    def __init__(self):
        self._queue: Queue = Queue()

    def invoke(self, fn: callable, *args, **kwargs):
        self._queue.put(InvokeCall(fn, args, kwargs))

    def update(self):
        while not self._queue.empty():
            call = self._queue.get()
            call()
