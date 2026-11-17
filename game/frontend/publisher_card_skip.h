// publisher_card_skip.h — Start/Cross ends the front end's publisher card from its first frame.
#pragma once

class Core;

namespace c12 {

// The front-end module's card state counts a 300-frame hold and lets any button end it only in its last 150 frames.
// A Start/Cross press the guest scanned before then brings the count to its last frame, so the card's own count ends
// it and the module enters the title through its own transition.
class PublisherCardSkip {
public:
  // At a frame start, before the guest turn.
  static void apply(Core &core);
};

} // namespace c12
