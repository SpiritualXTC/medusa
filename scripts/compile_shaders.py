import logging
import subprocess
from pathlib import Path


logger = logging.getLogger(__name__)


class VulkanShaders(object):
    @staticmethod
    def compile(in_path: Path, out_path: Path):

        extensions = {".comp", ".vert", ".frag", ".geom"}

        matched_files = (p.resolve() for p in Path(in_path).rglob("*") if p.suffix in extensions)

        for in_file in matched_files:
            out_file = out_path / f"{in_file.name}.spv"

            args = ["glslc", in_file, "-o", out_file]

            result = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            std_out = result.stdout.read().decode("utf-8")
            std_err = result.stderr.read().decode("utf-8")

            if std_out or std_err:
                if std_out:
                    logger.info(f"Compiled {in_file.name} to {out_file.name}. Output of")
                    logger.info(std_out)
                if std_err:
                    logger.info(f"Compiled {in_file.name} to {out_file.name} with errors")
                    logger.error(std_err)
            else:
                logger.info(f"Compiled: `{in_file.name}` -> `{out_file.name}`")
