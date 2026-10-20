#include "title/enhancements.h"

#include "render/draw_distance.h"

#include <array>

namespace c12 {
namespace {

psx::config::Var<bool> cvWidescreen("PSXPORT_C12_WIDESCREEN", true, psx::config::kSaved);

psx::config::Var<long> cvDrawDistance("PSXPORT_C12_DRAW_DISTANCE", kDrawDistanceDefaultPercent, psx::config::kSaved);
const psx::config::Registration kRegistration{cvWidescreen, cvDrawDistance};

const std::array<psx::game::TitleIntSetting, 1> kTitleIntSettings{{
    {"draw_distance", "Draw Distance", "%", &cvDrawDistance, kDrawDistanceRange},
}};

} // namespace

psx::config::Var<bool> &widescreenCvar() {
  return cvWidescreen;
}

psx::config::Var<long> &drawDistanceCvar() {
  return cvDrawDistance;
}

std::span<const psx::game::TitleIntSetting> titleIntSettings() {
  return kTitleIntSettings;
}

} // namespace c12
