from __future__ import annotations

from dataclasses import dataclass
from typing import Callable

import glm


class GLMStruct(type):
    def __new__(cls, name, bases, attrs):
        c = super().__new__(cls, name, bases, attrs)
        size = 0
        for b in bases:
            size += getattr(b, "_size", 0)

        size += GLMStruct._calc_size(attrs)
        c._size = size

        return c

    @staticmethod
    def _calc_size(attr) -> int:
        size = 0
        for k, v in attr.items():
            if k.startswith("__"):
                continue
            if isinstance(v, (Callable, classmethod, property)):
                continue
            try:
                size += glm.sizeof(v)
            except TypeError as e:
                raise e

        return size


@dataclass
class PushConstants(metaclass=GLMStruct):
    """This is the base class"""

    @classmethod
    def get_size(cls):
        return cls._size

    @property
    def size(self):
        return self.get_size()
