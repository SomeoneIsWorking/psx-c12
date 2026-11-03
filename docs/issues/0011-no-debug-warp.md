# 0011 — C-12 has no debug warp

Status: open (2026-11-03). Owner: `game/runtime/runtime.h` (`debugMenu_`, an empty psxport `TitleDebugMenu`).

## Observed
`warp ?` answers `not a debug verb`; the Debug tab says the title declares no debug routes. A mission is reached only by
replaying `replays/first-mission/` from boot.

## Route the game offers
NEW GAME → `GRISHAM'S LOG` → briefing → mission, with the master state word `0x800F1B40` at 3 in a mission
(`docs/re-frontier.md`). The mission index that page loads, and the guest call that starts it, are not yet traced. A
warp catalogue (mission → start) would arm that index and enter the log page's own start path at a frame boundary,
modelled on psxport `TitleDebugMenu::warp`.
