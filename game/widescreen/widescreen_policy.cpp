#include "widescreen/widescreen_policy.h"

#include "enhancement_gate.h"
#include "title/enhancements.h"

namespace c12 {

psx::present::PresentationAspect WidescreenPolicy::presentationAspect(const Core &) const {
  return psx::config::enh(widescreenCvar()) ? psx::present::PresentationAspect::Wide16x9
                                            : psx::present::PresentationAspect::Standard4x3;
}

} // namespace c12
