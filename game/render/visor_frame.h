// visor_frame.h — the visor's rounded frame (Visor_Draw, 0x80090214) at the drawn picture's edges.
//
// The frame's lines start at the 512-wide buffer's edges; under a widening each row's left line moves to the drawn
// picture's left edge and its right line to the right edge, through the host packets of the table they are linked in.
// The tint and the subtractive tile continue into the margins by themselves (psxport reads the other buffer's canvas).
// Evidence in c12-decomp docs/re-frontier.md (present.visor).

#pragma once

#include <cstdint>
#include <optional>

class Core;
class Game;

namespace c12 {

class VisorFrame {
public:
  static void registerOverrides(Game &game);

  // Moves the frame lines the last Visor_Draw built into the table walked from `root`; nothing for another table, when
  // they did not fit or when no margin is drawn.
  void anchor(Core &core, std::uint32_t root) const;

private:
  // The walk root of the table the lines are linked in and the first line's packet.
  struct Lines {
    std::uint32_t root = 0;
    std::uint32_t first = 0;
  };

  // A frame's screen effect: the lines of an earlier frame's visor are no longer in any table.
  static void effectOverride(Core *core);
  static void drawOverride(Core *core);

  std::optional<Lines> lines_;
};

} // namespace c12
