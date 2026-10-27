# 0006 — Boot presents miss one vsync in eight on a quiet host

Status: open (2026-10-27). Owner: not yet attributed.

## Observed
40 s paced, hidden run from boot (`psxport/scratch/player-bugs/realtime_all.py`, host load 4.8, every other title at
0-0.7% missed): `[playcheck] presents: intervals=1988 p50=16.75ms p99=56.00ms worst=81.97ms missed=249 (12.53%)
longest_miss_run=13`, no error lines (`scratch/player-bugs/realtime40/run.log`). Earlier runs the same day: 7.9% at
13:45 and 43.6% at 15:07 under heavier load.

## Not yet known
Which guest stretch the misses fall in (the boot's disc module load, the logo sequence or the title's own field loop),
and whether the pacer or a slow step owns them. Next: the present intervals against the guest PC per field over the
first 40 s.
