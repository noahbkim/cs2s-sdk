#include <usermessages.pb.h>

#include "cs2s/macro.h"
#include "cs2s/service/message.h"

#define LOG_PREFIX "[" STR(PLUGIN_NAME) ":messages] "

namespace cs2s::service
{

bool PluginMessageService::Load(PluginId id, ISmmAPI* ismm, bool late)
{
    this->game_event_system = reinterpret_cast<IGameEventSystem*>(
        ismm->VInterfaceMatch(ismm->GetEngineFactory(), GAMEEVENTSYSTEM_INTERFACE_VERSION, 0)
    );
    if (!this->game_event_system)
    {
        Log_Error(this->log, LOG_PREFIX "Could not resolve " GAMEEVENTSYSTEM_INTERFACE_VERSION "\n");
        return false;
    }

    this->network_messages = reinterpret_cast<INetworkMessages*>(
        ismm->VInterfaceMatch(ismm->GetEngineFactory(), NETWORKMESSAGES_INTERFACE_VERSION, 0)
    );
    if (!this->network_messages)
    {
        Log_Error(this->log, LOG_PREFIX "Could not resolve " NETWORKMESSAGES_INTERFACE_VERSION "\n");
        return false;
    }

    return true;
}

bool PluginMessageService::Unload()
{
    this->game_event_system = nullptr;
    this->network_messages = nullptr;
    return true;
}

void PluginMessageService::SendText(int target, const char* text) const
{
    INetworkSerializable* message_type = this->network_messages->FindNetworkMessagePartial("TextMsg");
    if (!message_type)
    {
        Log_Error(this->log, LOG_PREFIX "Could not resolve message type TextMsg\n");
        return;
    }

    CUserMessageTextMsg message;
    message.set_dest(target);
    message.add_param(text);
    this->Send(message_type, &message);
}

void PluginMessageService::SendText(int target, const char* text, IRecipientFilter* recipients) const
{
    INetworkSerializable* message_type = this->network_messages->FindNetworkMessagePartial("TextMsg");
    if (!message_type)
    {
        Log_Error(this->log, LOG_PREFIX "Could not resolve message type TextMsg\n");
        return;
    }

    CUserMessageTextMsg message;
    message.set_dest(target);
    message.add_param(text);
    this->Send(message_type, &message, recipients);
}

}
