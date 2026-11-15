# 0009 — 60 fps interpolation does not hold its cuts

Status: open (2026-10-29, narrowed 2026-11-15). Owner: `game/replay/draw_replay.h:c12::DrawReplay`.

## Done
The in-betweens replay `Game_Draw` (0x80038440, c12-decomp `docs/frame-loop.md`) with three inputs blended: the main
camera's source MATRIX, each object's MATRIX and each skeleton's animation clock, which the game's own sampler
interpolates between keys. In the first mission (`replays/first-mission/`, presents 1800–1900) the street, the
player's position and his run cycle and blade swing sit halfway between their ticks; the replay at t = 1 links the
real frame's ordering table word for word; a paced run advances 0.5 tick on 698 of 701 simulated 60 Hz vblanks. With
fps60 on and off the replay ends on the same guest RAM apart from four words that already part at frame 36 in the
front end, before any replay runs (an interrupt-time counter at 0x80105758 and three list links near 0x800F8024).

## Left
- Cuts are not read from the title. A camera whose source changes and an object or skeleton that changes are drawn
  as the later tick; a scripted switch that keeps the source matrix, or an in-level teleport, blends across the jump.
- An animation clock that wraps or changes track draws the later tick's pose for that in-between.
- The front-end divergence at frame 36 is not traced.
