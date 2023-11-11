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
    bool steam_api_initialized{false};

public:
    using PluginService::PluginService;

public:
    bool Load(PluginId id, ISmmAPI* ismm, bool late) override;
    bool Unload() override;

private:
    bool Initialize()
    {
        this->steam_api_initialized = this->steam_api.Init();
        return this->steam_api_initialized;
    }

public:
    // We do not want to return a pointer to the
    [[nodiscard]] CSteamGameServerAPIContext* Get() { return this->Initialize() ? &this->steam_api : nullptr; }

    [[nodiscard]] ISteamClient* GetSteamClient() { return this->Initialize() ? this->steam_api.SteamClient() : nullptr; }
    [[nodiscard]] ISteamGameServer* GetSteamGameServer() { return this->Initialize() ? this->steam_api.SteamGameServer() : nullptr; }
    [[nodiscard]] ISteamUtils* GetSteamGameServerUtils() { return this->Initialize() ? this->steam_api.SteamGameServerUtils() : nullptr; }
    [[nodiscard]] ISteamNetworking* GetSteamGameServerNetworking() { return this->Initialize() ? this->steam_api.SteamGameServerNetworking() : nullptr; }
    [[nodiscard]] ISteamGameServerStats* GetSteamGameServerStats() { return this->Initialize() ? this->steam_api.SteamGameServerStats() : nullptr; }
    [[nodiscard]] ISteamHTTP* GetSteamHTTP() { return this->Initialize() ? this->steam_api.SteamHTTP() : nullptr; }
    [[nodiscard]] ISteamInventory* GetSteamInventory() { return this->Initialize() ? this->steam_api.SteamInventory() : nullptr; }
    [[nodiscard]] ISteamUGC* GetSteamUGC() { return this->Initialize() ? this->steam_api.SteamUGC() : nullptr; }
};

}
