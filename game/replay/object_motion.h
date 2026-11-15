// object_motion.h — the objects' motion in a replay: each record on the object list (0x800F1100) carries its world
// MATRIX at +0x14, and a skinned object's skeleton the animation clocks its joints read; the section samples the
// pose at the clock's time (Anim_Sample, which interpolates the keys) and composes the joints from the matrix.
#pragma once

#include "replay/matrix_sample.h"

#include <vector>

namespace c12 {

class ObjectMotion final : public psx::present::MotionSource {
public:
  [[nodiscard]] const char *name() const override {
    return "objects";
  }
  void sample(Core &core) override;
  psx::present::MotionCount write(psx::present::GuestRedraw &redraw, float t) const override;

private:
  // A joint's animation clock: the word holding its time (frame << 12 | fraction) and the track it plays.
  struct Clock {
    std::uint32_t address = 0;
    std::uint32_t track = 0;
    std::uint32_t time = 0;
  };
  struct Object {
    std::uint32_t record = 0;
    std::uint32_t objectClass = 0;
    std::uint32_t skeleton = 0;
    MatrixSample transform;
    std::vector<Clock> clocks;
  };
  void sampleClocks(Core &core, Object &object) const;
  // The later sample's object pairs with the earlier's at the same record holding the same class and skeleton.
  [[nodiscard]] const Object *earlier(const Object &object) const;
  // The clock's time `t` of the way, while it plays one track forward; a wrap or a new track steps.
  static psx::present::MotionCount
  writeClock(psx::present::GuestRedraw &redraw, const Object &from, const Clock &clock, float t);

  std::vector<Object> from_;
  std::vector<Object> to_;
};

} // namespace c12
