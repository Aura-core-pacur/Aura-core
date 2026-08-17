/*
 * Aura Core
 * Copyright (C) 2026 Aura-core-pacur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

import ctypes
import os
from pathlib import Path


class AuraScripting:
    def __init__(self, library_path: str | None = None):
        lib_path = library_path or self._resolve_default_library()
        self.lib = ctypes.CDLL(lib_path)

        self.lib.aura_init.argtypes = []
        self.lib.aura_init.restype = None

        self.lib.aura_render_frame.argtypes = []
        self.lib.aura_render_frame.restype = None

        self.lib.aura_eval_js.argtypes = [ctypes.c_char_p]
        self.lib.aura_eval_js.restype = None

    def _resolve_default_library(self) -> str:
        candidates = [
            Path(__file__).resolve().parents[2] / "build" / "libaura_core_native.so",
            Path(__file__).resolve().parents[2] / "build" / "libaura_core_native.so.1",
            Path("/usr/local/lib/libaura_core_native.so"),
        ]
        for candidate in candidates:
            if candidate.exists():
                return str(candidate)
        raise FileNotFoundError("No se encontró libaura_core_native.so en los paths esperados")

    def init(self):
        self.lib.aura_init()

    def render_frame(self):
        self.lib.aura_render_frame()

    def eval_js(self, script: str):
        self.lib.aura_eval_js(script.encode("utf-8"))

    def run_plugin(self, script: str):
        self.eval_js(script)
        return {"status": "ok", "script": script}


if __name__ == "__main__":
    engine = AuraScripting()
    engine.init()
    engine.render_frame()
    engine.eval_js("console.log('Aura plugin loaded from Python');")
