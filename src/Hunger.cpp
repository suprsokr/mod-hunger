/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Config.h"

private:
    uint32 drainTick; // for example purposes, see OnWorldUpdate()
    uint32 drainAmount;
};

// Add player scripts
class Hunger_World : public WorldScript
{
public:
    Hunger_World() : WorldScript("Hunger_World") {}

    void OnAfterConfigLoad(bool reload) {
        drainTick = sConfigMgr->GetOption<int32>("DrainTick", 5000);
        drainAmount = sConfigMgr->GetOption<int32>("DrainAmount", 1);
    }

    void OnWorldUpdate(uint32 diff)
    {
        if (diff > drainTick)
        {
            sWorld->SendWorldText("Players hunger!");
            drainTick = sConfigMgr->GetOption<int32>("DrainTick", 5000);;
        }
        else
        {
            drainTick -= diff;
        }
    }
};

void AddSC_Hunger()
{
    new Hunger_World();
}
