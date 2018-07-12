#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "Player.h"
#include "Object.h"
#include "DataMap.h"

/*
Coded by Talamortis - For Azerothcore
Thanks to Rochet for the assistance
*/

uint32 MaxRate;
uint32 DefaultRate;

class PlayerXpRate : public DataMap::Base
{
public:
    PlayerXpRate() {}
    PlayerXpRate(uint32 XPRate) : XPRate(XPRate) {}
    uint32 XPRate = DefaultRate;
};

class Individual_XP : public PlayerScript
{
public:
    Individual_XP() : PlayerScript("Individual_XP") { }

    void OnLogin(Player* p) override
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT `XPRate` FROM `individualxp` WHERE `CharacterGUID` = %u", p->GetGUIDLow());
        if (!result)
        {
            p->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = DefaultRate;
        }
        else
        {
            Field* fields = result->Fetch();
            p->CustomData.Set("Individual_XP", new PlayerXpRate(fields[0].GetUInt32()));
        }
            ChatHandler(p->GetSession()).SendSysMessage("This server is running the |cff4CFF00Individual XP |rmodule. Use .XP to see all the commands.");
    }

    void OnLogout(Player* p) override
    {
        if (PlayerXpRate* data = p->CustomData.Get<PlayerXpRate>("Individual_XP"))
        {
            uint32 rate = data->XPRate;
            CharacterDatabase.DirectPExecute("REPLACE INTO `individualxp` (`CharacterGUID`, `XPRate`) VALUES (%u, %u);", p->GetGUIDLow(), rate);
        }
    }

    void OnGiveXP(Player* p, uint32& amount, Unit* victim) override
    {
        if (PlayerXpRate* data = p->CustomData.Get<PlayerXpRate>("Individual_XP"))
            amount *= data->XPRate;
    }
};

class Individual_XP_command : public CommandScript
{
public:
    Individual_XP_command() : CommandScript("Individual_XP_command") { }

    std::vector<ChatCommand> GetCommands() const override
    {
      
        static std::vector<ChatCommand> IndividualXPCommandTable =
        {
            // View Command
            { "View", SEC_PLAYER, false, &HandleViewCommand, "" },
            // Set Command
            { "Set", SEC_PLAYER, false, &HandleSetCommand, "" },
            // Default Command
            { "Default", SEC_PLAYER, false, &HandleDefaultCommand, "" },
            // Disable Command
            { "Disable", SEC_PLAYER, false, &HandleDisableCommand, "" }
        };
        
        static std::vector<ChatCommand> IndividualXPBaseTable =
        {
            { "XP", SEC_PLAYER, false, nullptr, "", IndividualXPCommandTable }
        };
        
        return IndividualXPBaseTable;
    }
    
    static bool HandleViewCommand(ChatHandler* handler, char const* args)
    {
        if (*args)
            return false;
          
        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;
        
        me->GetSession()->SendAreaTriggerMessage("Your current XP rate is %u", me->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate);
        return true;
    }
    
    static bool HandleSetCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        uint32 rate = (uint32)atol(args);
        if (rate > MaxRate)
            return false;

        me->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = rate;
        me->GetSession()->SendAreaTriggerMessage("You have updated your XP rate to %u", rate);
        return true;
    }
    
    static bool HandleDisableCommand(ChatHandler* handler, char const* args)
    {
        if (*args)
            return false;
          
        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;
        
        me->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = 0;
        me->GetSession()->SendAreaTriggerMessage("You have updated your XP rate to 0");
        return true;
    }
    
    static bool HandleDefaultCommand(ChatHandler* handler, char const* args)
    {
        if (*args)
            return false;
          
        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;
        
        me->CustomData.GetDefault<PlayerXpRate>("Individual_XP")->XPRate = DefaultRate;
        me->GetSession()->SendAreaTriggerMessage("You have restored your XP rate to the default value or %u", DefaultRate);
        return true;
    }
};

class Individual_XP_conf : public WorldScript
{
public:
    Individual_XP_conf() : WorldScript("Individual_XP_conf_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/Individual-XP.conf";

#ifdef WIN32
            cfg_file = "Individual-XP.conf";
#endif

            std::string cfg_def_file = cfg_file + ".dist";
            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());
            MaxRate = sConfigMgr->GetIntDefault("MaxXPRate", 10);
            DefaultRate = sConfigMgr->GetIntDefault("DefaultXPRate", 5);
        }
    }
};

void AddIndividual_XPScripts()
{
    new Individual_XP();
    new Individual_XP_conf();
    new Individual_XP_command();
}
