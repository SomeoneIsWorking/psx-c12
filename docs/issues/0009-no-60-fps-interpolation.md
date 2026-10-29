# 0009 — No 60 fps interpolation

Status: open (2026-10-29). Owner: `game/runtime/runtime.h:c12::Runtime`, which does not override `GameRuntime::interpolates()` (false).

## Observed
`fps60=1` changes nothing: the first-mission replay presents one guest frame per two fields (3969 fields across
2030 presents, `scratch/readme-media/clip169/run.log`), each new present differs from the last, and no in-between is
composed. The README therefore has a 30 fps gameplay clip and no "original 30 fps / interpolated 60 fps" comparison.

## Proper fix
The project's 60 fps goal (`docs/project-state.md` S010): give the record path what it needs to compose an in-between
between two guest frames, through psxport's draw-site tags (stable ids and lerped transforms for the camera, the
terrain pass `drawWorldMeshPass`, the actors and the HUD), with whole-frame coverage or none, and then override
`interpolates()` to return true. The cadence it lerps across is the gameplay loop's `VSync(2)` (issue 0008).

## Left out until then
`docs/media/` has no interpolation clip for C-12.
