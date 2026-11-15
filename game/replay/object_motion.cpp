#include "replay/object_motion.h"

#include "core.h"
#include "facts/title_facts.h"

#include <algorithm>
#include <cmath>

namespace c12 {

void ObjectMotion::sample(Core &core) {
  std::swap(from_, to_);
  to_.clear();
  for (std::uint32_t record = core.mem_r32(kObjectListHead); record != kObjectListSentinel;
       record = core.mem_r32(record + kObjectNext)) {
    Object &object = to_.emplace_back(Object{.record = record,
                                             .objectClass = core.mem_r32(record + kObjectClass),
                                             .transform = MatrixSample::read(core, record + kObjectTransform)});
    sampleClocks(core, object);
  }
}

void ObjectMotion::sampleClocks(Core &core, Object &object) const {
  const std::uint32_t skeletonSlot = core.mem_r32(object.record + kObjectSkeleton);
  object.skeleton = skeletonSlot == 0u ? 0u : core.mem_r32(skeletonSlot);
  if (object.skeleton == 0u) {
    return;
  }
  const std::uint32_t joints = core.mem_r32(object.skeleton + kSkeletonJoints);
  const std::uint32_t count = core.mem_r16(object.skeleton + kSkeletonJointCount);
  for (std::uint32_t joint = 0; joint < count; ++joint) {
    const std::uint32_t record = joints + joint * kJointStride;
    const Clock clock{.address = core.mem_r32(record + kJointClock), .track = core.mem_r32(record + kJointTrack)};
    if (clock.address == 0u || clock.track == 0u || std::ranges::any_of(object.clocks, [&](const Clock &seen) {
          return seen.address == clock.address;
        })) {
      continue;
    }
    object.clocks.push_back({.address = clock.address, .track = clock.track, .time = core.mem_r32(clock.address)});
  }
}

const ObjectMotion::Object *ObjectMotion::earlier(const Object &object) const {
  const auto found = std::ranges::find_if(from_, [&](const Object &candidate) {
    return candidate.record == object.record && candidate.objectClass == object.objectClass &&
           candidate.skeleton == object.skeleton;
  });
  return found == from_.end() ? nullptr : &*found;
}

psx::present::MotionCount
ObjectMotion::writeClock(psx::present::GuestRedraw &redraw, const Object &from, const Clock &clock, float t) {
  const auto earlier = std::ranges::find_if(from.clocks, [&](const Clock &candidate) {
    return candidate.address == clock.address && candidate.track == clock.track;
  });
  if (earlier == from.clocks.end() || clock.time < earlier->time) {
    return {.drawn = 1, .unpaired = 1};
  }
  const double step = static_cast<double>(clock.time - earlier->time) * static_cast<double>(t);
  redraw.writeWord(clock.address, earlier->time + static_cast<std::uint32_t>(std::lround(step)));
  return {.drawn = 1, .unpaired = 0};
}

psx::present::MotionCount ObjectMotion::write(psx::present::GuestRedraw &redraw, float t) const {
  psx::present::MotionCount count;
  for (const Object &object : to_) {
    ++count.drawn;
    const Object *from = earlier(object);
    if (from == nullptr) {
      ++count.unpaired;
      count.drawn += object.clocks.size();
      count.unpaired += object.clocks.size();
      continue;
    }
    object.transform.writeBlended(redraw, from->transform, t);
    for (const Clock &clock : object.clocks) {
      count += writeClock(redraw, *from, clock, t);
    }
  }
  return count;
}

} // namespace c12
