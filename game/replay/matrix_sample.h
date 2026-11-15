// matrix_sample.h — one PsyQ MATRIX in guest memory, sampled at a real draw and written blended into a replay.
#pragma once

#include "motion_set.h"

#include <array>
#include <cstdint>

class Core;

namespace c12 {

struct MatrixSample {
  std::uint32_t address = 0;
  std::array<std::uint32_t, psx::present::TransformRule::kWords> words{};

  [[nodiscard]] static MatrixSample read(Core &core, std::uint32_t address);
  // This (the later sample) `t` of the way from `from` at the same address.
  void writeBlended(psx::present::GuestRedraw &redraw, const MatrixSample &from, float t) const;
};

} // namespace c12
