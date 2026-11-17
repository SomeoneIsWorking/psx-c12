// session.h — C-12's per-Core state, created with each Game.

#pragma once

#include "game_runtime.h"
#include "render/cell_output.h"
#include "render/visor_frame.h"
#include "replay/draw_replay.h"

class Core;

namespace c12 {

class Session final : public psx::game::TitleSessionState {
public:
  WorldVisibility visibility;
  DrawReplay drawReplay;
  VisorFrame visorFrame;
};

// The world visibility and the draw replay of the Game `core` belongs to.
WorldVisibility &worldVisibility(Core &core);
DrawReplay &drawReplay(Core &core);
VisorFrame &visorFrame(Core &core);

} // namespace c12
