// draw_section.h — C-12's draw section: Game_Draw(mode), the gameplay loop's last call, which walks each camera's
// display lists into its ordering table and runs the world mesh pass. Gfx_SwapAndSubmit hands the main camera's table
// to DrawOTag after the next VSync, so its chain belongs to the next drawing record.
#pragma once

#include "replay_frame.h"

#include <array>
#include <cstdint>
#include <optional>

class Core;

namespace c12 {

class DrawSection final : public psx::present::ReplaySection {
public:
  [[nodiscard]] std::optional<std::uint32_t> draw(Core &core, psx::present::GuestRedraw &redraw) const override;
  [[nodiscard]] bool linked(std::uint32_t head) const override;
  [[nodiscard]] bool submitsAfterWait() const override {
    return true;
  }

private:
  // The main camera's two ordering tables' last entries, as the section last linked them.
  mutable std::array<std::uint32_t, 2> heads_{};
};

} // namespace c12
