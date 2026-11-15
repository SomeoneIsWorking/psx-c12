// draw_replay.h — C-12's in-betweens: the draw section replayed with the moving inputs blended.
#pragma once

#include "replay/camera_motion.h"
#include "replay/draw_section.h"
#include "replay/object_motion.h"
#include "replay_frame.h"

#include <array>

class Core;
class Game;

namespace c12 {

class DrawReplay {
public:
  DrawReplay();

  // The sample at the draw section's entry.
  static void registerOverrides(Game &game);

  [[nodiscard]] psx::present::ReplayFrame &frame() {
    return frame_;
  }

private:
  static void sectionOverride(Core *core);

  DrawSection section_;
  CameraMotion camera_;
  ObjectMotion objects_;
  std::array<psx::present::MotionSource *, 2> sources_{&camera_, &objects_};
  psx::present::ReplayFrame frame_;
};

} // namespace c12
