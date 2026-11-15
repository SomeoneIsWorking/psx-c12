# 0012 — The dropship sequence's fade to black shows one bright frame

Status: open (2026-11-15). Owner: not traced.

## Observed
`replays/first-mission/menu-to-mission-run-right-left.pad`, unpaced, ires 0, 4K sink, presents from fence 1384: the
dropship cutscene fades to black by fence 1386, fence 1390 shows the letterboxed dropship at full brightness (mean
luma 32.5 against 0.0 on both sides), and fences 1391-1393 are black again before the mission fades in. The same
present shows with `PSXPORT_FPS60=0`, so interpolation is not the cause.

## Next
Read which draw the bright frame comes from: the fade's owner (the briefing's fade level against the record sealed at
that fence) and whether the record the presenter shows at 1390 is one the guest drew at full brightness.
