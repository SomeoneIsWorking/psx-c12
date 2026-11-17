// mission_warp.h — the mission warp: the guest's own next-mission request with a chosen mission.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class Core;

namespace c12 {

// Armed between frames, applied at the next frame start while the gameplay loop runs: it writes the mission index,
// its first section and leave request 5, the words Mission_Advance (0x800857A4) writes, so the scene loop unloads
// the mission and loads the chosen one through its own owners.
class MissionWarp {
public:
  std::string arm(std::uint32_t mission);
  // Raises the armed request when legal; the mission requested.
  std::optional<std::uint32_t> apply(Core &core);

  // The gameplay loop is between frames in play with no leave request of its own.
  static bool legal(Core &core);
  // The mission's module file name, read from the level table.
  static std::string name(Core &core, std::uint32_t mission);
  // The missions a new game plays, in order: the first, then each record's next until a record names itself.
  static std::vector<std::uint32_t> chain(Core &core);

private:
  std::optional<std::uint32_t> armed_;
};

} // namespace c12
