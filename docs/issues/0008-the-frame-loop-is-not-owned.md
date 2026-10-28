# 0008 — The frame loop is not owned

Status: open, cadence owned (2026-10-28). Owner: `game/frame/frame_driver.cpp:FrameDriver::stepFrame`.

## What the code does
Each step runs the guest from its PC until libetc VSync `0x800A1758` exits as a frame boundary, resumes the VSync
continuation and commits `Timing::takeStepFields()` fields, with one SPU advance per field.

The gameplay loop is `FUN_80038094` (called from `FUN_8003736c` at `0x800375A8`). Per iteration it calls
`FUN_8009E52C(0)`, then `VSync(2)` at `0x800380FC` unless two or more VBlanks (`gp+0xCB4` less `gp+0x104`) already
passed, stores `gp+0x104 = gp+0xCB4`, and runs about 50 per-frame calls switched on `DAT_800F1B4C` while
`DAT_800F1B44 == 0`. Over the first-mission replay the waits are `VSync(2)` from the loop (`ra 0x80038104`, 4926 calls),
from module code (`ra 0x80121204`, 727) and from the loader `FUN_80010968` (`ra 0x80010A7C`, 257), and a few `VSync(0)`.

## Fixed: every VSync(2) was answered with one field
`StockVSync::serve` ended the turn and the driver committed one field whatever the argument, so the gameplay loop ran
one logic frame per field (60 Hz logic against retail's 30 Hz), and load turns that overran the field budget left the
boundary off the field grid (48 steps over the replay). Now `StockVSync` notes the wait (`Timing::noteVSyncWait`) and
the driver commits what it spans: n less the fields earlier no-wait steps already delivered since the last return, at
least the field the clock is in; a turn with no wait delivers the field edges it crossed. The driver's turn takes
`ExecutionBudget::untilFieldEdge`, ending on the next field edge (psxport `docs/presentation.md`, Frame fence and pacing).

Shown on `replays/first-mission/menu-to-mission-run-right-left.pad`, 6000 steps unpaced, `PSXPORT_DEBUG=fielddigest,presentseq`:

| | cadence (fields/parts) | steps off the field grid | present shots at fences 1000..6000 |
|---|---|---|---|
| before | 6000 × 1/1 | 48 (loads: steps 1, 9–23, 761–804, 1392) | `d8aaca09 00d57ecb 1f210377 ea059f5e 621f5f13 4068bc09` |
| after | 5909 × 2/1, 91 × 1/1 | 0 | identical |

Guest RAM hashes match up to step 1 and differ from step 2 on: the VBlank counter and every clock-driven timer now
advance two fields per logic frame.

## Left
- The loop body still runs as guest code from its own PC; the driver owns the cadence at the replaced wait, not the
  per-frame update and draw. Driving the ~50 per-frame calls from the driver needs them identified one by one.
- The loop's skip branch (no wait when two VBlanks already passed) can only be taken when an iteration spans a turn
  that ran out without a wait: the VBlank counter moves at commits, never inside a turn.
- No console-oracle comparison of the 30 Hz cadence or of the skip branch.
