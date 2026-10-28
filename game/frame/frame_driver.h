// frame_driver.h — the C-12 display-field owner.

#pragma once

#include "game_runtime.h"

#include <cstdint>

class Core;
class Game;

namespace c12 {

// One step is one logic frame: the logic-frame start (clock, producer census, pad), one guest turn to the next libetc
// VSync(n) `0x800A1758`, the VSync continuation, one presentation commit of the fields that wait spans
// (Timing::takeStepFields), then audio per field.
class FrameDriver final : public psx::game::FrameDriver {
public:
  explicit FrameDriver(Game &game);

  void stepFrame(Core &core, std::uint32_t frame) override;

  std::uint64_t fields() const {
    return fields_;
  }
  std::uint64_t boundaries() const {
    return boundaries_;
  }

private:
  Game &game_;
  std::uint64_t fields_ = 0;
  std::uint64_t boundaries_ = 0;
};

} // namespace c12
