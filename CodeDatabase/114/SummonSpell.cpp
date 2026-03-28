#include "SummonSpell.h"
#include "Player.h"
#include "Pet.h"
#include "../events/EventNotifier.h"
#include "../events/PetSummonedEvent.h"

bool SummonSpell::cast(int pendingBoost, Entity*, ISpellField* field) {
    Player& player = field->getPlayer();
    const int dxs[4] = {1,-1,0,0};
    const int dys[4] = {0,0,1,-1};
    for (int j = 0; j < pendingBoost; ++j) {
        for (int i = 0; i < 4; ++i) {
            int nx = player.getX() + dxs[i];
            int ny = player.getY() + dys[i];
            if (!field->isValidPosition(nx, ny)) continue;
            if (!field->isPositionPassable(nx, ny)) continue;
            field->getPets().emplace_back();
            field->getPets().back().setPosition(nx, ny);
            field->getCells()[ny][nx].setHasEntity(true);
            
            // Уведомляем о призыве союзника
            PetSummonedEvent summonEvent(nx, ny);
            EventNotifier::getInstance().notify(summonEvent);
        }
    }
    return true;
}



