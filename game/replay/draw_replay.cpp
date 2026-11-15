#include "replay/draw_replay.h"

#include "core.h"
#include "facts/title_facts.h"
#include "game.h"
#include "native_dispatch.h"
#include "runtime/session.h"

namespace c12 {

DrawReplay::DrawReplay() : frame_(section_, {}, sources_) {
}

void DrawReplay::sectionOverride(Core *core) {
  drawReplay(*core).frame().beforeDraw(*core);
  psx::cpu::callOriginalToReturn(
      *core, kDrawSectionAddress, psx::cpu::ExecutionBudget::currentTurn(*core), "C-12 draw section");
}

void DrawReplay::registerOverrides(Game &game) {
  psx::cpu::installNativeOverride(game.core, kDrawSectionAddress, "C-12 draw replay sample", sectionOverride);
}

} // namespace c12
