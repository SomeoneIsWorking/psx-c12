#include "replay/camera_motion.h"

#include "core.h"
#include "facts/title_facts.h"

namespace c12 {

void CameraMotion::sample(Core &core) {
  from_ = to_;
  const std::uint32_t source = core.mem_r32(core.mem_r32(kCameraPointerAddress) + kCameraSourceTransform);
  to_ = source == 0u ? std::nullopt : std::optional(MatrixSample::read(core, source));
}

psx::present::MotionCount CameraMotion::write(psx::present::GuestRedraw &redraw, float t) const {
  // Another source (a cut to a scripted camera) is drawn as the later tick's.
  if (!to_ || !from_ || from_->address != to_->address) {
    return {.drawn = 1, .unpaired = 1};
  }
  to_->writeBlended(redraw, *from_, t);
  return {.drawn = 1, .unpaired = 0};
}

} // namespace c12
