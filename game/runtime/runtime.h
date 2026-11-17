#pragma once

#include "debug/debug_menu.h"
#include "game_runtime.h"
#include "stock_libcd.h"
#include "stock_vsync.h"
#include "widescreen/widescreen_policy.h"

namespace c12 {

// Runs the guest startup unmodified from the executable entry; FrameDriver owns each field.
class Runtime final : public psx::game::GameRuntime {
public:
  Runtime();
  void registerOverrides(Game &game) override;
  std::unique_ptr<psx::game::TitleSessionState> createSession(Game &game) override;
  std::unique_ptr<psx::game::FrameDriver> createFrameDriver(Game &game) override;
  GuestAddressRange guestCodeModuleWindow() const override;
  std::optional<psx::gpu::PacketPoolExtent> packetPool(Core &core) const override;
  const psx::present::GuestWidescreenProjection *guestWidescreenProjection() const override;
  void reportRun(Core &core) const override;
  bool interpolates() const override {
    return true;
  }
  psx::present::ReplayFrame *replayFrame(Core &core) override;
  std::span<const psx::game::TitleIntSetting> titleIntSettings() const override;
  const psx::game::TitleDebugMenu &debugMenu() const override {
    return debugMenu_;
  }

private:
  psx::hle::StockVSync vsync_;
  psx::cd::GuestInterruptLibcd libcd_;
  WidescreenPolicy widescreenPolicy_;
  DebugMenu debugMenu_;
};

} // namespace c12
