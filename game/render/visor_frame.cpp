#include "render/visor_frame.h"

#include "core.h"
#include "facts/title_facts.h"
#include "game.h"
#include "guest_widescreen_projection.h"
#include "native_dispatch.h"
#include "replay/draw_section.h"
#include "runtime/session.h"

#include <lucent/log.h>

namespace c12 {

void VisorFrame::registerOverrides(Game &game) {
  psx::cpu::installNativeOverride(game.core, kScreenEffectDrawAddress, "c12::VisorFrame effect", effectOverride);
  psx::cpu::installNativeOverride(game.core, kVisorDrawAddress, "c12::VisorFrame", drawOverride);
  lucent::info("c12-wide", "installed visor frame owner at 0x{:08X}", kVisorDrawAddress);
}

void VisorFrame::effectOverride(Core *host) {
  Core &core = *host;
  visorFrame(core).lines_.reset();
  psx::cpu::callOriginalToReturn(
      core, kScreenEffectDrawAddress, psx::cpu::ExecutionBudget::currentTurn(core), "C-12 screen effect");
}

void VisorFrame::drawOverride(Core *host) {
  Core &core = *host;
  const std::uint32_t cursor = core.mem_r32(kPacketCursorAddress);
  const bool fits = cursor + kVisorFrameRows * 2u * kVisorFrameLineBytes < core.mem_r32(kPacketEndAddress);
  const std::uint32_t root = orderingTableHead(core.mem_r32(kDrawTableAddress), core.mem_r16(kDrawTableLengthAddress));
  psx::cpu::callOriginalToReturn(
      core, kVisorDrawAddress, psx::cpu::ExecutionBudget::currentTurn(core), "C-12 visor frame");
  VisorFrame &frame = visorFrame(core);
  frame.lines_ = fits ? std::optional<Lines>(Lines{.root = root, .first = cursor}) : std::nullopt;
  frame.anchor(core, root);
}

void VisorFrame::anchor(Core &core, std::uint32_t root) const {
  const psx::present::HorizontalMargins edges = psx::present::frameWindows(core).drawn;
  if (!lines_ || lines_->root != root || edges.total() == 0) {
    return;
  }
  for (std::uint32_t row = 0; row < kVisorFrameRows; ++row) {
    const std::uint32_t left = lines_->first + row * 2u * kVisorFrameLineBytes;
    core.hostPackets.moveX(core, root, left, -edges.left);
    core.hostPackets.moveX(core, root, left + kVisorFrameLineBytes, edges.right);
  }
}

} // namespace c12
