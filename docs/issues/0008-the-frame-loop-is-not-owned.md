# 0008 — The frame loop is not owned

Status: open (2026-10-28). Owner: `game/frame/frame_driver.cpp:FrameDriver::stepFrame`.

## What the code does
Each step runs the guest from its PC until libetc VSync `0x800A1758` exits as a frame boundary, resumes the VSync continuation and commits one field.

## Why it matters
The workspace rule (psxport `docs/workspace/WORKSPACE.md`, "Each title owns its frame loop") wants the frame driver to run the guest's per-frame update and draw and decide the cadence, replacing the guest's VSync waits. Until then the loop is on the fallback; it is not yet on `Timing::vsyncFields` either, since every guest VSync is answered with one field whatever its argument.

## What blocks it
The main loop is not identified. VSync has 37 call sites in 21 functions (`decomp_pipeline.py --callers 0x800A1758`); the ones read so far are loaders with their own waits, e.g. the async resource loop `FUN_800107FC` (VSync(2) per iteration, called from `0x8003777C` and `0x8005D97C`). Next: find the per-frame update and draw under `0x8003777C`/`0x8005D97C`, then drive them from the driver with the loop's own field count.
The converted driver counts `Timing::vsyncFields` of the loop's own n at the replaced wait (psxport `docs/presentation.md`, Frame fence and pacing), so an overrunning frame keeps its VBlanks, audio and fields on the guest clock's grid; it is done in that conversion, not as a separate step on the current one-field answer.
