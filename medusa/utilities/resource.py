from __future__ import annotations

from types import ModuleType

import pkg_resources
from pathlib import Path


class Resource(object):
    def __init__(self, package: ModuleType, module: str):
        self.__package: ModuleType = package
        self.__module: str = module
        self.__filepath: Path = Path(pkg_resources.resource_filename(self.__package.__name__, self.__module))

    @property
    def filepath(self) -> Path:
        return self.__filepath

    def read(self, binary: bool = False) -> str | bytes:
        return open(self.filepath, "rb" if binary else "r").read()
