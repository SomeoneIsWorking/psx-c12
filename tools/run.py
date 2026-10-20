#!/usr/bin/env python3
"""Resolve the C-12 disc, extract and authenticate its executable, build c12_port, and launch it."""

from __future__ import annotations

import os
from collections.abc import Sequence
from pathlib import Path

import title_identity
from automation.process import ToolError
from config import DISC_VARIABLE, ConfigurationError, resolve_config
from port import launcher
from port.launch_environment import player_environment

ROOT = Path(__file__).resolve().parents[1]
EXTRACTED = ROOT / "scratch" / "c12-identity"


def provision(discdump: Path, disc: Path) -> Path:
    """Extract the executable the disc's SYSTEM.CNF boots and authenticate it."""
    EXTRACTED.mkdir(parents=True, exist_ok=True)
    for name in ("SYSTEM.CNF", title_identity.EXECUTABLE_NAME):
        launcher.run([discdump, "get", name, disc, EXTRACTED], os.environ, cwd=ROOT, quiet=True)
    try:
        program = title_identity.authenticate_extraction(EXTRACTED)
    except title_identity.IdentityError as error:
        raise ToolError(str(error)) from error
    launcher.say(f"provisioned {program.executable} ({program.executable.stat().st_size} bytes, authenticated)")
    return program.executable


def launch(disc: str | None, prepare_only: bool) -> None:
    try:
        config = resolve_config(ROOT, Path(disc) if disc else None, os.environ)
    except ConfigurationError as error:
        raise ToolError(str(error)) from error
    launcher.preflight()
    executable = provision(launcher.build_discdump(ROOT, os.environ), config.disc)
    build = launcher.build_product(ROOT, "c12_port", os.environ)
    if prepare_only:
        launcher.say("C-12 is built and ready.")
        return
    env = player_environment(os.environ, product="c12")
    env.setdefault(DISC_VARIABLE, str(config.disc))
    launcher.exec_product(build / "c12_port", [str(executable)], env, ROOT)


def main(argv: Sequence[str] | None = None) -> int:
    args = launcher.argument_parser(__doc__).parse_args(argv)
    return launcher.main(lambda: launch(args.disc, args.prepare_only))


if __name__ == "__main__":
    raise SystemExit(main())
