# 0007 — Skipping the briefing issues a Setloc into the lead-in

Status: fixed (2026-10-28). Owner: psxport `runtime/psx/cd/cdc_native.cpp` (`CdController::getlocL`), psxport issue 0190.

## Observed
Start, X (NEW GAME), X (story page), X (skip the briefing cutscene): the log shows
`[cd:error] setloc 00:00:00 is inside the lead-in; no position set` and the same for the implicit Setloc, once, at the
skip. The first mission loads and plays afterwards. Seen with and without the stock CdRead binding
(`scratch/player-bugs-3/base-newgame/run.log`, `optb-newgame/run.log`), so it predates issue 0006's fix.

## Cause
The guest's own skip is correct. The sound queue `FUN_800577AC` saves the playing XA music with GetlocL (case 7) before
the briefing and resumes it at the skip with Setloc/ReadS on the saved header (case 9). psxport never answered GetlocL,
so the saved header was zero and the resume asked for 00:00:00.

## Fix and proof
psxport answers GetlocL from the header of the last sector read (issue 0190). The same route now logs `Pause`,
`Setloc 23:03:62`, `ReadS 23:03:62` at the skip, no `cd:error`, and the first mission plays
(`scratch/player-bugs-4/briefing2/run.log`, `d_small.png`).

The music is heard after the skip (`scratch/player-bugs-4/briefing3/`, paced, `PSXPORT_DEBUG=cdcmd,xa`, the SPU mix
captured with `PSXPORT_WAV`): the music stream starts at LBA 103629 (23:03:54), GetlocL latches it, the briefing voice
plays from 148549, and at the skip `xa stop @ LBA 148549` is followed by `xa start @ LBA 103637` (23:03:62). That stream
is not stopped again before the run ends 16 s later, and the mix stays at RMS 1900-2800 per second over those 16 s
(0 before the first sound, 600 in the briefing's quiet tail). `after_small.png` is the first mission at that point.
