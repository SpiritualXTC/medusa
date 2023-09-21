from __future__ import annotations

from types import ModuleType

import pkg_resources


class Resource(object):
    def __init__(self, package: ModuleType, module: str):
        self._package: ModuleType = package
        self._module: str = module

    def filename(self) -> str:
        return pkg_resources.resource_filename(self._package.__name__, self._module)

    def read(self, binary: bool = False) -> str | bytes:
        filename = pkg_resources.resource_filename(self._package.__name__, self._module)

        return open(filename).read()
