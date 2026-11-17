#include "debug/mission_warp.h"

#include "core.h"
#include "facts/title_facts.h"

#include <lucent/log.h>

namespace c12 {

bool MissionWarp::legal(Core &core) {
  return core.mem_r32(kSceneStateAddress) == kSceneStateRunning && core.mem_r32(kGameModeAddress) == 0u &&
         core.mem_r32(kLeaveRequestAddress) == 0u && core.pc == kFrameLoopVSyncReturn;
}

std::string MissionWarp::name(Core &core, std::uint32_t mission) {
  const std::uint32_t text = core.mem_r32(kLevelTableAddress + mission * kLevelRecordStride + kLevelModuleName);
  std::string result;
  for (std::uint32_t i = 0; i < kLevelModuleNameMax; ++i) {
    const char c = static_cast<char>(core.mem_r8(text + i));
    if (c == '\0') {
      break;
    }
    result.push_back(c);
  }
  return result;
}

std::vector<std::uint32_t> MissionWarp::chain(Core &core) {
  std::vector<std::uint32_t> missions;
  std::uint32_t mission = kFirstMission;
  while (missions.size() <= kLastMission) {
    missions.push_back(mission);
    const std::uint32_t next = core.mem_r16(kLevelTableAddress + mission * kLevelRecordStride + kLevelNextMission);
    if (next == mission || next > kLastMission) {
      break;
    }
    mission = next;
  }
  return missions;
}

std::string MissionWarp::arm(std::uint32_t mission) {
  armed_ = mission;
  return lucent::format("ok: warp armed for mission {}", mission);
}

std::optional<std::uint32_t> MissionWarp::apply(Core &core) {
  if (!armed_ || !legal(core)) {
    return std::nullopt;
  }
  const std::uint32_t mission = *armed_;
  armed_.reset();
  core.mem_w32(kMissionIndexAddress, mission);
  core.mem_w32(kMissionSectionAddress, 0u);
  core.mem_w32(kLeaveRequestAddress, kLeaveNextMission);
  lucent::info("c12-warp", "mission {} requested", mission);
  return mission;
}

} // namespace c12
