# 0010 — A save point stays on "Accessing MEMORY CARD - please wait"

Status: open (2026-11-03), cause not yet named. Owner: between psxport's card HLE (`runtime/psx/platform/memcard.cpp`)
and the title's libmcrd caller.

## Observed
Player report `20261031-180552-game-stuck-here`, pad frame 7791: at a save point the game shows "Accessing MEMORY CARD -
please wait" and never leaves it.

## Reproduction
The report's `repro.pad` on a copy of its card, hidden, unpaced, reaches the same screen at 7791
(psx-spyro `scratch/depth/runs/c12-stuck/cmp.png`, left ours, right the report's) and is still there 400 frames later.

## Trace so far
`PSXPORT_DEBUG=card`: the only card calls are `_card_info` (A0:0xAB) twice, `_new_card` (B0:0x50), the probe
`_card_write(0, 0x3F, 0)` (B0:0x4E) and `_card_load` (A0:0xAC); each completes synchronously with SwCARD and HwCARD
EvSpIOE. No `open`/`firstfile` follows. In the binary this is libmcrd's MemCardAccept (`FUN_800ac964`, card check
`FUN_800ac5e0`): `_card_info` finds IOE on a card whose bit in `0x801056A8` is clear, so it reports a new card (status 3);
Accept then runs `_new_card`, the probe write and `_card_load`, and the four SwCARD callbacks (`0x800B60D0..0x800B610C`,
flags `0x80105834..40`) return IOE. `dumpram` at frames 7900 and 8200 (psx-spyro `scratch/depth/runs/c12-ram/run.log`; script `scratch/depth/c12ram.txt`) shows libmcrd idle:
task depth `0x800EF1E0` = -1, status `0x801056A0` = 0, all eight callback flags 0. So libmcrd finished and the wait is
in the caller, which has not been located yet.

## Next
Find the save-point caller of MemCardAccept/MemCardSync (refs to `0x801056A0` beyond libmcrd, the `"Accessing MEMORY
CARD"` string) and what it waits for after a new-card Accept; compare with what retail's `_card_load` does to the
card's "new" state.
