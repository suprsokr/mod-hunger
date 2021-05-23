/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Config.h"
#include "IWorld.h"
#include "Player.h"

class Hunger_World : public WorldScript
{
private:
    int drainTick;
    int curDrainTick;
    int drainAmount;
    int playerHealthRegen;
    bool enabled;

public:
    Hunger_World() : WorldScript("Hunger_World") {}

    void OnAfterConfigLoad(bool /*reload*/) {
        drainTick = sConfigMgr->GetOption<int>("Hunger.DrainTick", 5000);
        curDrainTick = drainTick;
        drainAmount = sConfigMgr->GetOption<int>("Hunger.DrainAmount", 1);
        playerHealthRegen = sConfigMgr->GetOption<int>("Hunger.PlayerHealthRegen", 0);
        enabled = sConfigMgr->GetOption<bool>("Hunger.Enable", false);

        sWorld->setRate(RATE_HEALTH, (float)playerHealthRegen);

        LOG_INFO("server", "Hunger.Enable: %u, Hunger.DrainTick: %u, Hunger.DrainAmount: %u, Hunger.PlayerHealthRegen: %u", enabled, drainTick, drainAmount, playerHealthRegen);
    }

    void OnUpdate(uint32 diff) {
        if (enabled){
            if ((int)diff > curDrainTick) {
                curDrainTick = drainTick;
                ReduceAllPlayersHealth();
            } else {
                curDrainTick -= diff;
            }
        }
    }

    void ReduceAllPlayersHealth(){
        const SessionMap& s = sWorld->GetAllSessions();
        SessionMap::const_iterator itr;
        for (itr = s.begin(); itr != s.end(); ++itr)
        {
            if (!itr->second)
                continue;

            Player* player = itr->second->GetPlayer();
            if (!player)
                continue;

            if (player->IsInWorld() && player->IsAlive())
                player->ModifyHealth(- (int32)drainAmount);
        }
        return;
    }
};

void AddSC_Hunger()
{
    new Hunger_World();
}
