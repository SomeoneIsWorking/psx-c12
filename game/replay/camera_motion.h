// camera_motion.h — the main camera's source transform in a replay: the MATRIX its record points at (camera + 0x80,
// the camera's world rotation and position), which Camera_BuildMatrices turns into the view inside the draw section.
#pragma once

#include "replay/matrix_sample.h"

#include <optional>

namespace c12 {

class CameraMotion final : public psx::present::MotionSource {
public:
  [[nodiscard]] const char *name() const override {
    return "camera";
  }
  void sample(Core &core) override;
  psx::present::MotionCount write(psx::present::GuestRedraw &redraw, float t) const override;

private:
  std::optional<MatrixSample> from_;
  std::optional<MatrixSample> to_;
};

} // namespace c12
