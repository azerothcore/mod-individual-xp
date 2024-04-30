#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "Player.h"
#include "Object.h"
#include "DataMap.h"

using namespace Acore::ChatCommands;

/*
Coded by Talamortis - For Azerothcore
Thanks to Rochet for the assistance
*/

bool IndividualXpEnabled;
bool IndividualXpAnnounceModule;
uint32 MaxRate;
uint32 DefaultRate;

class Individual_XP_conf : public WorldScript
{
public:
    Individual_XP_conf() : WorldScript("Individual_XP_conf_conf") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        IndividualXpAnnounceModule = sConfigMgr->GetOption<bool>("IndividualXp.Announce", 1);
        IndividualXpEnabled = sConfigMgr->GetBoolDefault("IndividualXp.Enabled", 1);
        MaxRate = sConfigMgr->GetIntDefault("MaxXPRate", 10);
        DefaultRate = sConfigMgr->GetIntDefault("DefaultXPRate", 1);
    }
};

enum IndividualXP
{
    ACORE_STRING_CREDIT = 35411,
    ACORE_STRING_MODULE_DISABLED,
    ACORE_STRING_RATES_DISABLED,
    ACORE_STRING_COMMAND_VIEW,
    ACORE_STRING_MAX_RATE,
    ACORE_STRING_MIN_RATE,
    ACORE_STRING_COMMAND_SET,
    ACORE_STRING_COMMAND_DISABLED,
    ACORE_STRING_COMMAND_ENABLED,
    ACORE_STRING_COMMAND_DEFAULT
};

class Individual_Xp_Announce : public PlayerScript
{
public:

    Individual_Xp_Announce() : PlayerScript("Individual_Xp_Announce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (IndividualXpEnabled & IndividualXpAnnounceModule)
        {
            ChatHandler(player->GetSession()).SendSysMessage(ACORE_STRING_CREDIT);
        }
    }
};

class PlayerXpRate : public DataMap::Base
{
public:
    PlayerXpRate() {}
    PlayerXpRate(uint32 XPRate) : XPRate(XPRate) {}
    uint32 XPRate = 1;
};

class Individual_XP : public PlayerScript
{
public:
    Individual_XP() : PlayerScript("Individual_XP") { }

    void OnLogin(Player* p) override
    {
        QueryResult result = CharacterDatabase.Query("SELECT `XPRate` FROM `individualxp` WHERE `CharacterGUID` = '{}'", p->GetGUID().GetCounter());
        if (!result)
        {
            p->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = DefaultRate;
        }
        else
        {
            Field* fields = result->Fetch();
            p->CustomData.Set("Individual_XP", new PlayerXpRate(fields[0].Get<uint32>()));
        }
    }

    void OnLogout(Player* p) override
    {
        if (PlayerXpRate* data = p->CustomData.Get<PlayerXpRate>("Individual_XP"))
        {
            uint32 rate = data->XPRate;
            CharacterDatabase.DirectExecute("REPLACE INTO `individualxp` (`CharacterGUID`, `XPRate`) VALUES ('{}', '{}');", p->GetGUID().GetCounter(), rate);
        }
    }

    void OnGiveXP(Player* p, uint32& amount, Unit* /*victim*/, uint8 /*xpSource*/) override
    {
        if (IndividualXpEnabled) {
            if (PlayerXpRate* data = p->CustomData.Get<PlayerXpRate>("Individual_XP"))
                amount *= data->XPRate;
        }
    }

};

class IndividualXPCommand : public CommandScript
{
public:
    IndividualXPCommand() : CommandScript("IndividualXPCommand") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable IndividualXPCommandTable =
        {
            { "enable",  HandleEnableCommand, SEC_PLAYER, Console::No },
            { "disable",  HandleDisableCommand, SEC_PLAYER, Console::No },
            { "view",  HandleViewCommand, SEC_PLAYER, Console::No },
            { "set",  HandleSetCommand, SEC_PLAYER, Console::No },
            { "default",  HandleDefaultCommand, SEC_PLAYER, Console::No }
        };

        static ChatCommandTable IndividualXPBaseTable =
        {
            { "xp",  IndividualXPCommandTable }
        };

        return IndividualXPBaseTable;
    }

    static bool HandleViewCommand(ChatHandler* handler)
    {
        if (!IndividualXpEnabled)
        {
            handler->PSendSysMessage(ACORE_STRING_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
            return false;

        if (player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN))
        {
            handler->PSendSysMessage(ACORE_STRING_RATES_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
        {
            player->GetSession()->SendAreaTriggerMessage(ACORE_STRING_COMMAND_VIEW, player->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate);
        }
        return true;
    }

    static bool HandleSetCommand(ChatHandler* handler, uint32 rate)
    {
        if (!IndividualXpEnabled)
        {
            handler->PSendSysMessage(ACORE_STRING_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!rate)
            return false;

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
            return false;

        if (rate > MaxRate)
        {
            handler->PSendSysMessage(ACORE_STRING_MAX_RATE, MaxRate);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else if (rate == 0)
        {
            handler->PSendSysMessage(ACORE_STRING_MIN_RATE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        player->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = rate;
        player->GetSession()->SendAreaTriggerMessage(ACORE_STRING_COMMAND_SET, rate);
        return true;
    }

    static bool HandleDisableCommand(ChatHandler* handler)
    {
        if (!IndividualXpEnabled)
        {
            handler->PSendSysMessage(ACORE_STRING_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
            return false;

        // Turn Disabled On But Don't Change Value...
        player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
        player->GetSession()->SendAreaTriggerMessage(ACORE_STRING_COMMAND_DISABLED);
        return true;
    }

    static bool HandleEnableCommand(ChatHandler* handler)
    {
        if (!IndividualXpEnabled)
        {
            handler->PSendSysMessage(ACORE_STRING_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
            return false;

        player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
        player->GetSession()->SendAreaTriggerMessage(ACORE_STRING_COMMAND_ENABLED);
        return true;
    }

    // Default Command
    static bool HandleDefaultCommand(ChatHandler* handler)
    {
        if (!IndividualXpEnabled)
        {
            handler->PSendSysMessage(ACORE_STRING_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
            return false;

        player->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = DefaultRate;
        player->GetSession()->SendAreaTriggerMessage(ACORE_STRING_COMMAND_DEFAULT, DefaultRate);
        return true;
    }
};

void AddIndividual_XPScripts()
{
    new Individual_XP_conf();
    new Individual_Xp_Announce();
    new Individual_XP();
    new IndividualXPCommand();
}
