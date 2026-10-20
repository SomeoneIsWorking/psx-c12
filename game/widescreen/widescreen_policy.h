#pragma once

#include "guest_widescreen_projection.h"

namespace c12 {

// Declares the aspect the title asks for; on the record path the canvas holds the margins.
class WidescreenPolicy final : public psx::present::GuestWidescreenProjection {
public:
  psx::present::PresentationAspect presentationAspect(const Core &core) const override;
};

} // namespace c12
