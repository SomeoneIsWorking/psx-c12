#include "debug/debug_menu.h"

#include "core.h"
#include "debug/mission_warp.h"
#include "facts/title_facts.h"
#include "frame/frame_driver.h"
#include "game.h"

#include <lucent/log.h>

#include <vector>

namespace c12 {

DebugMenu::DebugMenu() {
  warp({
      .levels = {"mission"},
      .places =
          [](Core &core) {
            std::vector<psx::game::WarpPlace> missions;
            for (std::uint32_t mission : MissionWarp::chain(core)) {
              missions.push_back({
                  .name = lucent::format("{:02} {}", mission, MissionWarp::name(core, mission)),
                  .arrive =
                      [mission](Core &c) {
                        return c.game->frameDriverAs<FrameDriver>().armWarp(mission);
                      },
              });
            }
            return missions;
          },
      .refusal =
          [](Core &core) {
            return core.game->frameDriverIf<FrameDriver>() != nullptr && MissionWarp::legal(core)
                       ? std::string()
                       : "a warp is legal only in play, with no pause view or leave request";
          },
  });
}

} // namespace c12
