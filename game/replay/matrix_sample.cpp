#include "replay/matrix_sample.h"

#include "core.h"
#include "guest_redraw.h"

namespace c12 {

MatrixSample MatrixSample::read(Core &core, std::uint32_t address) {
  MatrixSample sample{.address = address};
  for (std::uint32_t i = 0; i < sample.words.size(); ++i) {
    sample.words[i] = core.mem_r32(address + 4u * i);
  }
  return sample;
}

void MatrixSample::writeBlended(psx::present::GuestRedraw &redraw, const MatrixSample &from, float t) const {
  static constexpr psx::present::TransformRule kRule;
  std::array<std::uint32_t, psx::present::TransformRule::kWords> out{};
  kRule.blend({.address = address, .from = from.words, .to = words}, t, out);
  for (std::uint32_t i = 0; i < out.size(); ++i) {
    redraw.writeWord(address + 4u * i, out[i]);
  }
}

} // namespace c12
