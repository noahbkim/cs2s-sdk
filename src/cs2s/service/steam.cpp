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
    if (this->steam_api_initialized)
    {
        this->steam_api.Clear();
        this->steam_api_initialized = false;
    }

    return true;
}

}
