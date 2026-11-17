#include "frontend/publisher_card_skip.h"

#include "core.h"
#include "facts/title_facts.h"

#include <lucent/log.h>

namespace c12 {

void PublisherCardSkip::apply(Core &core) {
  if (core.mem_r32(kSceneStateAddress) != kSceneStateFrontEnd) {
    return;
  }
  const std::uint32_t module = core.mem_r32(kModuleSlotAddress);
  if (core.pc != module + kFrontEndFrameVSyncReturn ||
      core.mem_r32(module + kFrontEndState) != kFrontEndStateCard ||
      core.mem_r32(module + kFrontEndCardHold) < kFrontEndCardButtonsFrom ||
      (core.mem_r32(kPressedButtonsAddress) & kConfirmButtons) == 0u) {
    return;
  }
  core.mem_w32(module + kFrontEndCardHold, 1u);
  lucent::info("c12-skip", "Start/Cross ends the publisher card");
}

} // namespace c12
