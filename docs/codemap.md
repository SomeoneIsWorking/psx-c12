# Codemap — C-12: Final Resistance

Where C-12's code lives and what each owner owns. Feature chains are in `docs/features.md`; the
framework side is `external/psxport/docs/codemap.md`.

Frame loop: cadence owned. Each step runs the guest to its next VSync `0x800A1758` and delivers the fields that wait spans (`Timing::takeStepFields`; the gameplay loop `FUN_80038094` asks for 2); the loop body is still guest code (issue 0008).

## Owners

| owner | path | owns |
|---|---|---|
| `createCatalog` | `game/entry/catalog.*` | The one-title catalog: `TitleIdentity` from `title.json` and a `c12::Runtime` |
| `c12::Runtime` | `game/runtime/runtime.*` | The `GameRuntime`: stock VSync and libcd objects bound in `registerOverrides`, the native overrides, session, frame driver, module window, packet pool, Record path, run report |
| `c12::Session`, `worldVisibility`, `drawReplay` | `game/runtime/session.*` | Per-Core title state: the extended cell list the collector last published, the draw replay |
| `c12::FrameDriver` | `game/frame/frame_driver.*` | One logic frame: one guest turn to the VSync, one commit of its fields, audio per field |
| `resumeVsyncContinuation` | `game/frame/vsync_continuation.*` | Resuming at a VSync continuation in `r31`, or refusing it |
| `DebugMenu` | `game/debug/debug_menu.*` | The Debug tab and `warp <mission>`, over the missions a new game plays |
| `MissionWarp` | `game/debug/mission_warp.*` | The mission warp: the guest's next-mission request with a chosen mission, applied at a frame start |
| `PublisherCardSkip` | `game/frontend/publisher_card_skip.*` | Start/Cross ends the front end's publisher card from its first frame |
| `completeCdReadySyncCommand` | `game/cd/cd_command_completion.*` | `CdReadySync` (`0x800ABD98`) and the command-status completion it owes |
| `rebuildViewFrustumCorners` | `game/render/view_frustum.*` | `FUN_800660D8`, widened with the canvas |
| `collectVisibleCells`, `CellCollector` | `game/render/cell_collector.*` | `FUN_800684C0`: the retail cell buffer and the extended host list |
| `CellOutput`, `WorldVisibility` | `game/render/cell_output.*` | Guest and host cell outputs; the published list, nearest first |
| `drawWorldMeshPass` | `game/render/world_mesh_pass.*` | `FUN_8006769C`: terrain polygons, the widened screen reject, the extended far clip |
| `DrawDistance` | `game/render/draw_distance.*` | The one percent-to-scale rule for the draw distance knob |
| `FogTables` | `game/render/fog_table.*` | The fade tables, stretched by the draw distance scale |
| `DrawReplay` | `game/replay/draw_replay.*` | The 60 fps in-betweens: samples at `Game_Draw` (`0x80038440`) and replays it per in-between |
| `DrawSection` | `game/replay/draw_section.*` | `Game_Draw` as psxport's `ReplaySection`: its run and the main camera's ordering table |
| `CameraMotion` | `game/replay/camera_motion.*` | The main camera's source MATRIX (camera + 0x80), blended |
| `ObjectMotion` | `game/replay/object_motion.*` | Each object's MATRIX and its skeleton's animation clocks, paired by record, class and skeleton |
| `MatrixSample` | `game/replay/matrix_sample.*` | One guest MATRIX sampled and written blended by psxport's `TransformRule` |
| `WidescreenPolicy` | `game/widescreen/widescreen_policy.*` | The title's `GuestWidescreenProjection`: 16:9 when the knob is on |
| `widescreenCvar`, `drawDistanceCvar`, `titleIntSettings` | `game/title/enhancements.*` | The title's `pc_enh` knobs and its settings-menu row (`docs/config.md`) |
| measured addresses | `game/facts/title_facts.h` | Guest addresses the title's own code reads, hand copies of the decomp's symbol map until plan 0001 |
| executable identity | `game/facts/executable_identity.h.in` | Generated from `title.json` |
| launcher | `bootstrap.py`, `tools/run.py`, `tools/config.py`, `tools/title_identity.py` | The `external/psxport` fetch, disc resolution, extraction, authentication and build for the zero-argument launch, on psxport `tools/port/launcher.py` |
| replays | `replays/first-mission/` | Boot to the first mission and a run right then left (`PSXPORT_PAD_REPLAY`) |

## Where new work goes

- A guest function C-12 replaces → `game/<subsystem>/`, installed from `c12::Runtime::registerOverrides`.
- Per-Core title state → `c12::Session`.
- A measured guest address → its name in `external/c12-decomp/config/symbols/scus_946.66.txt` and its evidence in
  `external/c12-decomp/docs/re-frontier.md` (a decomp commit first), then `game/facts/title_facts.h` (plan 0001).
- A title knob → `game/title/enhancements.*` and `docs/config.md`.
- Anything every title shares → psxport, not here.
