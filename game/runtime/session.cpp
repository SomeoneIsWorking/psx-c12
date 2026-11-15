#include "runtime/session.h"

#include "core.h"
#include "game.h"

namespace c12 {

WorldVisibility &worldVisibility(Core &core) {
  return static_cast<Session &>(*core.game->sessionState).visibility;
}

DrawReplay &drawReplay(Core &core) {
  return static_cast<Session &>(*core.game->sessionState).drawReplay;
}

} // namespace c12
