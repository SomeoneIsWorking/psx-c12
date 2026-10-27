# 0006 — Boot presents miss one vsync in eight on a quiet host

Status: open (2026-10-27). Owner: not yet attributed.

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

## Not yet known
Which owner the late fields spend their time in: the profile is not tied to the 25 ms / 34-42 ms rhythm. Which guest
routine the boot's slow fields run (stores into code pages that force invalidations, or a dispatch-heavy
loop), and whether the 300-499 misses are the same. Next: per-field host-dispatch and invalidation counts against the
guest PC over fences 1-100. A review re-run on the same build missed 36 (1.56%, longest run 2, load 4.0).

On the psxport working tree with the timed SPU (psxport issue 0183) two 40 s paced hidden runs missed 89 (3.98%, longest
run 13, load 3.5; `scratch/player-bugs-2/realtime40-3`) and 59 (2.57%, longest run 3, load 5.0; `rt-c12-a`). A
6 s boot profile (`c12-perf/perf.data`) puts `SPU_UpdateFromCDC` at 0.74%, so the SPU running to guest time at its
accesses is not the cost; the spread between runs at the same load is as large as the change.
