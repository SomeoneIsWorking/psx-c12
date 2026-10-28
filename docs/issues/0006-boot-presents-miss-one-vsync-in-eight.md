# 0006 — Boot presents miss one vsync in eight on a quiet host

Status: fixed (2026-10-28). Owner: `game/runtime/runtime.cpp` (the stock libcd binding); facts in
`game/facts/title_facts.h`.

## Observed
40 s paced, hidden run from boot (`psxport/scratch/player-bugs/realtime_all.py`, host load 4.8, every other title at
0-0.7% missed): `[playcheck] presents: intervals=1988 p50=16.75ms p99=56.00ms worst=81.97ms missed=249 (12.53%)
longest_miss_run=13`, no error lines (`scratch/player-bugs/realtime40/run.log`). Earlier runs the same day: 7.9% at
13:45 and 43.6% at 15:07 under heavier load.

## Measured, 2026-10-27
On psxport `8a4215c5` the same 40 s paced hidden run misses 37-41 of 2311 presents (1.6-1.8%, load 4.0-4.6;
`psxport/scratch/player-bugs-2/realtime_all.log`, `scratch/player-bugs-2/rt-pacer/run.log` with
`PSXPORT_DEBUG=pacer,presentseq`). 39 of the 46 late presents are fences 5-94, the first 1.5 s of the boot, in a
fixed rhythm: every 3-4 fences one interval of about 25 ms and the next of 34-42 ms. The other 7 fall in fences
300-499. No title log line marks those fields.

A flat user-space profile of the whole first 2 s (`perf record -e cpu-clock:u`,
`scratch/player-bugs-2/perf-boot/perf.data`) has no dominant symbol and does not isolate the late fields, so it
attributes nothing; its top entries are `lightrec_rw` 6.0%, `NativeDispatcher::classify` 5.5%,
`LightrecExecutor::blockBoundary` 5.3%, `OwnerLedger::port` 5.3%, `notifyExecutableWrite` 2.6% and
`lightrec_invalidate_blocks` 1.7%, besides the disc's LZMA decode at open.

On the psxport working tree with the timed SPU (psxport issue 0183) two 40 s paced hidden runs missed 89 (3.98%, longest
run 13, load 3.5; `scratch/player-bugs-2/realtime40-3`) and 59 (2.57%, longest run 3, load 5.0; `rt-c12-a`). A
6 s boot profile (`c12-perf/perf.data`) puts `SPU_UpdateFromCDC` at 0.74%, so the SPU running to guest time at its
accesses is not the cost; the spread between runs at the same load is as large as the change.

## Attributed, 2026-10-28
Per-field counters on fences 1-500 (unpaced, `scratch/player-bugs-3/pages/run.log`): every slow field (f1-f94 and
f396-f409) ends on the 4M-cycle budget (`kCyclesPerTurn`) rather than at a VSync. Each runs about 2.0M guest
instructions in 25-46 ms, with about 45k host dispatches, 594k Lightrec memory callbacks and 273k CPU-store
invalidation requests (227k to the stack page `0x801FF000`, 45k to `0x80105000`), and 0 Lightrec blocks invalidated.
The guest is in libcd's CdReady wait, `FUN_800b49e0` (polling VSync(-1) `FUN_800a1758`, timeout counter
`0x801057AC`), while the boot loader reads its files one 2x-speed sector at a time (225,792 ticks each,
`cdcpace`). Fields that reach a VSync cost 2-3 ms.

Two causes:
1. psxport `StockLibcd::applyCommand` ran the Pause that precedes a ReadN through the controller
   (`retirePendingCommand`) without latching the interrupt edge it raised. The guest spinning in CdReady never reads
   I_STAT, so the INT3/INT2 and the first sector's INT1 waited for the field end. Every new file read lost the rest of
   a field (`scratch/player-bugs-3/cdwait3/excerpt.log`: `dataready head=2 tail=5 ... edge=1` with no IRQ2 until the
   field ended). Fixed: `applyCommand` latches as a guest controller write does; sectors now arrive one per drive
   period inside the field (`cdwait5/excerpt.log`), and budget-ended fields fall from 94 to 89.
2. What remains is the cost of the wait itself. All guest RAM accesses go through Lightrec memory callbacks, and every
   store also requests an invalidation, so a field of CdReady spin costs 25-40 ms (psxport issue 0185).

A 40 s paced run after fix 1 missed 45 of 2310 (1.95%, p99 34.5 ms, longest run 3, load 5.1;
`scratch/player-bugs-3/rt2/run.log`); 46 (1.99%, load 5.4) before. That difference is inside the run-to-run spread
above, so fix 1 alone has no shown effect on what the player sees.

## Fixed, 2026-10-28
The loader's blocking read `FUN_800abce4` is CdIntToPos, CdControlB(Setloc), CdRead `FUN_800af9d4`(sectors, buf,
0x80) and CdReadSync `FUN_800afb70`(0, 0); the streamer (`FUN_8009cfd0`/`FUN_8009d06c`, `FUN_800ac358`/`FUN_800ac3d0`)
issues the same CdRead and polls CdReadSync(1). Both entries are now bound as psxport's stock `CdRead`/`CdReadSync`,
the instant-CD design: a read lands whole when issued and CdReadSync finds nothing left, so no load waits in CdReady.
The loader's `RELOCS/GT.LVB` lands in the module window through psxport's `announceStockReadLanding`, which now
activates the window as a DMA3 landing does.

Shown, hidden runs on the product (ires 0, fps60 1, 3840x2160 sink):
- 40 s paced from boot: `presents: intervals=2361 p50=16.75ms p99=17.00ms worst=22.85ms missed=0 (0.00%)`, load 4.0
  (`scratch/player-bugs-3/optb-paced/run.log`); 45-46 missed before.
- Unpaced from boot: the title's PRESS START BUTTON screen at 25 s and 50 s, the code module activated at its first
  landing, no fault (`optb-unpaced2/`).
- Start, X, X, X, Start: the menu, the story page, the briefing, the first mission street with the HEALTH bar, and the
  pause menu (`optb-newgame/strip.png`). The same route without the binding logs the same Setloc error at the
  briefing skip (`base-newgame/run.log`), recorded as issue 0007.

The CdReady spin cost (psxport 0185) is no longer paid at boot; 0185 stays open for the framework.

## Turn length, 2026-10-28
psxport `335ea43a` made a turn budget guest ticks (one per instruction) instead of Lightrec cycles (two), so the fixed
4M `kCyclesPerTurn` became about 7 fields of guest work before one commit: 2 budget-ended turns and 2 missed presents
(worst 49-56 ms, `scratch/player-bugs-5/paced40-review`). The driver now takes `ExecutionBudget::currentTurn`, one
field: 40 s paced, load 0.96, `worst=20.71ms missed=0`, 17 budget-ended turns (`scratch/player-bugs-5/paced40-turn`).
