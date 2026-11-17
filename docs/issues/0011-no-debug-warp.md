# 0011 — C-12 has no debug warp

Status: fixed (2026-11-17). Owner: `game/debug/mission_warp.*` (`MissionWarp`), `game/debug/debug_menu.*`.

## Observed
`warp ?` answers `not a debug verb`; the Debug tab says the title declares no debug routes. A mission is reached only by
replaying `replays/first-mission/` from boot.

## Route the game offers
NEW GAME → `GRISHAM'S LOG` → briefing → mission, with the master state word `0x800F1B40` at 3 in a mission
(`docs/re-frontier.md`). The mission index that page loads, and the guest call that starts it, are not yet traced. A
warp catalogue (mission → start) would arm that index and enter the log page's own start path at a frame boundary,
modelled on psxport `TitleDebugMenu::warp`.

## Fix and proof
`warp ?` lists the missions a new game plays, from the level table's next-mission chain (2 `RONIN1.LVB` through 23;
records 0 and 1 are `TI.LVB` and name themselves next, so they are not listed). `warp <n>` arms the place and the
frame driver applies it at the gameplay loop's VSync return: it writes the mission index, section 0 and leave request
5, the words `Mission_Advance` writes, and the scene loop unloads and loads through its own owners. From the first
mission, `warp 3` loads mission 5 (`RONIN2C.LVB`, `g_LoadedMission` 5) and its opening dialogue plays
(`scratch/skips/vw/sheet.png`, `run.log`).
