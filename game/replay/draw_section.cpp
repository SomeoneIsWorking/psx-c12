#include "replay/draw_section.h"

#include "core.h"
#include "facts/title_facts.h"
#include "guest_redraw.h"

#include <algorithm>

namespace c12 {

std::optional<std::uint32_t> DrawSection::draw(Core &core, psx::present::GuestRedraw &redraw) const {
  const std::array<std::uint32_t, 1> mode{core.mem_r32(kGameModeAddress)};
  if (!redraw.run(kDrawSectionAddress, mode)) {
    return std::nullopt;
  }
  // DrawOTag walks the table being built from its last entry.
  const std::uint32_t camera = core.mem_r32(kCameraPointerAddress);
  const std::uint32_t table = core.mem_r32(camera + kCameraBuildTable);
  const std::uint32_t head = table + core.mem_r16(camera + kCameraTableLength) * 4u - 4u;
  if (std::ranges::find(heads_, head) == heads_.end()) {
    heads_ = {heads_[1], head};
  }
  return head;
}

bool DrawSection::linked(std::uint32_t head) const {
  return head != 0u && std::ranges::find(heads_, head) != heads_.end();
}

} // namespace c12
