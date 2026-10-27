# 0007 — Skipping the briefing issues a Setloc into the lead-in

Status: open (2026-10-28). Owner: not yet traced; the refusal is psxport `stock_libcd.cpp` `positionFromParam`.

## Observed
Start, X (NEW GAME), X (story page), X (skip the briefing cutscene): the log shows
`[cd:error] setloc 00:00:00 is inside the lead-in; no position set` and the same for the implicit Setloc, once, at the
skip. The first mission loads and plays afterwards. Seen with and without the stock CdRead binding
(`scratch/player-bugs-3/base-newgame/run.log`, `optb-newgame/run.log`), so it predates issue 0006's fix.

## Next
Find which guest call passes the zero position at the skip (the cutscene's XA/STR stop path is the first suspect) and
whether the console issues the same command; a Setloc to 00:00:00 the drive refuses may be retail behaviour.
