from __future__ import annotations

from typing import TypeVar, Any

import glm
import numpy as np

from vulkan import ffi


T = TypeVar("T")


class GLMField(object):
    def __init__(self, default: T):
        self._default = default
        self._glm_type = default.__class__
        self._size = glm.sizeof(self._glm_type)
        self._index: int = 0

    def __set_name__(self, owner, name: str):
        self._name = f"_{name}"
        if not issubclass(owner, GLMStruct):
            raise ValueError("GLM Fields require a GLM Struct")
        # TODO: Add size automatically as fields are added
        fields = getattr(owner, "__glm_fields__", [])
        self._index = len(fields)
        fields.append(self)

        setattr(owner, "__glm_fields__", fields)

    def __get__(self, instance: GLMStruct, owner) -> T:
        value = getattr(instance, self._name)

        return value

    def __set__(self, instance: GLMStruct, value: T):
        if not isinstance(value, self._glm_type):
            raise ValueError("Invalid Type")
        setattr(instance, self._name, value)
        instance.upd(self._index)

    @property
    def name(self) -> str:
        return self._name

    @property
    def default(self) -> T:
        return self._default

    @property
    def size(self):
        return glm.sizeof(self.default)

    @property
    def index(self):
        return self._index


class GLMStruct(object):
    """The class that does all the setting up"""

    @classmethod
    def fields(cls):
        return getattr(cls, "__glm_fields__")

    def __new__(cls, *args, **kwargs):
        instance = super().__new__(cls)
        fields = getattr(cls, "__glm_fields__")

        nbytes = 0

        for f in fields:
            setattr(instance, f.name, f.default)
            nbytes += f.size

        instance._nbytes = nbytes

        return instance

    def __init__(self, stage_flags):
        self._stage_flags = stage_flags

        fields = tuple((getattr(self, f.name) for f in self.fields()))
        self._buffer = np.void(fields)
        self._handle = ffi.cast("void *", self._buffer.ctypes.data)

    @property
    def stage_flags(self):
        return self._stage_flags

    def upd(self, index: int):
        self._buffer[index] = getattr(self, self.fields()[index].name)

    @property
    def handle(self) -> Any:
        return self._handle

    @property
    def nbytes(self) -> int:
        return self._nbytes
