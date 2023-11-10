#include "steam.h"

#include "cs2s/macro.h"

#define LOG_PREFIX "[" STR(PLUGIN_NAME) ":steam] "

namespace cs2s::service
{

bool PluginSteamService::Load(SourceMM::PluginId id, SourceMM::ISmmAPI* ismm, bool late)
{
    return true;
}

bool PluginSteamService::Unload()
{
    if (this->steam_api_loaded)
    {
        this->steam_api.Clear();
        this->steam_api_loaded = false;
    }

    return true;
}

CSteamGameServerAPIContext* PluginSteamService::Get()
{
    // We want to avoid hooking `GameServerSteamAPIActivated` to know when
    // the API is available, so we check every time instead.
    if (!this->steam_api_loaded)
    {
        this->steam_api_loaded = this->steam_api.Init();
    }

    // Something something `std::optional`...
    if (this->steam_api_loaded)
    {
        return &this->steam_api;
    }
    else
    {
        Log_Warning(this->log, LOG_PREFIX "The Steam API is currently unavailable!\n");
        return nullptr;
    }
}

}
