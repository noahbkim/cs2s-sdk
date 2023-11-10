#pragma once

#include <vector>

#include <steam/steam_gameserver.h>

#include <ISmmPlugin.h>

#include "cs2s/service.h"

namespace cs2s::service
{

class PluginSteamService : public PluginService
{
protected:
    ISmmAPI* metamod{nullptr};

    // Must be late-initialized once the Steam API library is loaded.
    CSteamGameServerAPIContext steam_api{};
    bool steam_api_loaded{false};

public:
    using PluginService::PluginService;

public:
    bool Load(PluginId id, ISmmAPI* ismm, bool late) override;
    bool Unload() override;

public:
    CSteamGameServerAPIContext* Get();
};

}
