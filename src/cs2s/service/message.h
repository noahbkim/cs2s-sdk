#pragma once

#include <cstdint>
#include <cstdio>
#include <utility>

#include <tier0/logging.h>
#include <igameeventsystem.h>
#include <networksystem/inetworkmessages.h>

#include "cs2s/macro.h"
#include "cs2s/service.h"
#include "cs2s/service/library.h"

// vendor/hl2sdk/game/shared/shareddefs.h
#define HUD_PRINTNOTIFY     1
#define HUD_PRINTCONSOLE    2
#define HUD_PRINTTALK       3
#define HUD_PRINTCENTER     4

// https://github.com/NapasP/advertising/blob/main/colors/colors.h
#define CHAT_DEFAULT        "\x01"
#define CHAT_RED            "\x02"
#define CHAT_LIGHTPURPLE    "\x03"
#define CHAT_GREEN          "\x04"
#define CHAT_LIME           "\x05"
#define CHAT_LIGHTGREEN     "\x06"
#define CHAT_LIGHTRED       "\x07"
#define CHAT_GRAY           "\x08"
#define CHAT_LIGHTOLIVE     "\x09"
#define CHAT_OLIVE          "\x10"
#define CHAT_LIGHTBLUE      "\x0B"
#define CHAT_BLUE           "\x0C"
#define CHAT_PURPLE         "\x0E"
#define CHAT_GRAYBLUE       "\x0A"
#define CHAT_NEWLINE        "\xe2\x80\xa9"

namespace cs2s::service
{

// Avoid creating LOG_PREFIX in global namespace
#define MESSAGES_LOG_PREFIX "[" STR(PLUGIN_NAME) "] "

template<typename... Args>
bool format(std::string& into, const char* fmt, Args&& ... args)
{
    int size = snprintf(nullptr, 0, fmt, args...);
    if (size <= 0)
    {
        return false;
    }

    char* buffer = new char[size + 1];
    snprintf(buffer, size + 1, fmt, args...);
    into.assign(buffer, size);
    return true;
}

class PluginMessageService : public PluginService
{
private:
    IGameEventSystem* game_event_system{nullptr};
    INetworkMessages* network_messages{nullptr};

public:
    using PluginService::PluginService;

public:
    bool Load(PluginId id, ISmmAPI* ismm, bool late) override;
    bool Unload() override;

public:
    void Send(INetworkSerializable* message_type, void* message) const
    {
        this->game_event_system->PostEventAbstract(0, false, -1, nullptr, message_type, message, 0, BUF_RELIABLE);
    }

    template<typename T>
    void Send(INetworkSerializable* message_type, T* message) const
    {
        this->game_event_system->PostEventAbstract(0, false, -1, nullptr, message_type, message, sizeof(T), BUF_RELIABLE);
    }

    void Send(INetworkSerializable* message_type, void* message, IRecipientFilter* recipients) const
    {
        this->game_event_system->PostEventAbstract(0, false, recipients, message_type, message, 0);
    }

    template<typename T>
    void Send(INetworkSerializable* message_type, T* message, IRecipientFilter* recipients) const
    {
        this->game_event_system->PostEventAbstract(0, false, recipients, message_type, message, sizeof(T));
    }

    void SendText(int target, const char* text) const;
    void SendText(int target, const char* text, IRecipientFilter* recipients) const;

    template<typename... Args>
    void Print(CEntityInstance* player, int hud, const char* fmt, Args&& ... args) const
    {
        std::string buffer;
        if (!format(buffer, fmt, std::forward<Args>(args)...))
        {
            Log_Error(this->log, MESSAGES_LOG_PREFIX "Failed to interpolate format: %s\n", fmt);
            return;
        }

        // TODO: recipient filter
        this->SendText(hud, buffer.c_str());
    }

    void Print(CEntityInstance* player, int hud, const char* fmt) const
    {
        // TODO: recipient filter
        this->SendText(hud, fmt);
    }

    template<typename... Args>
    void Print(int hud, const char* fmt, Args&& ... args) const
    {
        std::string buffer;
        if (!format(buffer, fmt, std::forward<Args>(args)...))
        {
            Log_Error(this->log, MESSAGES_LOG_PREFIX "Failed to interpolate format: %s\n", fmt);
            return;
        }

        this->SendText(hud, buffer.c_str());
    }

    void Print(int hud, const char* fmt) const
    {
        this->SendText(hud, fmt);
    }
};

}
