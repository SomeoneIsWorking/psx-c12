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

The replay links the main camera's two DR_ENV packets into its table as `Gfx_SwapAndSubmit` does, so a cutscene's
letterbox draw area and offset hold in the in-betweens (dropship briefing, fences 1372-1380: before, every real frame
was letterboxed and every in-between full screen).

## Left
- Cuts are not read from the title. The camera's source is always the rig (0x800F0930); in the first mission it jumps
  three times (by 4000, 6000 and 18982 units). Two jumps change the rig's controller (`g_CameraRig` +4) and fall
  under black fades outside the replayed intervals; the third is a shot change inside the dropship sequence, which
  snaps every tick (c12-decomp `docs/frame-loop.md`, Camera rig and controllers), so its declaration is the code
  that sets that record's shot. A camera whose source changes and an object or skeleton that changes are drawn
  as the later tick; a scripted switch that keeps the source matrix, or an in-level teleport, blends across the jump.
- An animation clock that wraps or changes track draws the later tick's pose for that in-between.
- The front-end divergence at frame 36 is not traced.
