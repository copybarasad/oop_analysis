#include "combat_system.h"
#include "field.h"
#include "event_system.h"
#include <iostream>

int CombatSystem::killCounter_ = 0;

bool CombatSystem::dealDamage(Field& field, int targetId, int damage, int sourceId) {
    auto entity = field.getEntity(targetId);
    if (!entity) return false;

    entity->setHealth(entity->getHealth() - damage);
    // Notify about the damage: who hit whom
    auto src = field.getEntity(sourceId);
    if (src && src->getType() == EntityType::Player) {
        EventSystem::getInstance().logPlayerAttacked(targetId, damage);
    }
    if (entity->getType() == EntityType::Player) {
        EventSystem::getInstance().logPlayerTakeDamage(damage, sourceId);
    }
    
    if (!entity->isAlive()) {
        handleEntityDeath(field, targetId, sourceId);
        return true;
    }
    return false;
}

void CombatSystem::handleEntityDeath(Field& field, int entityId, int killerId) {
    int r, c;
    // capture type before removal
    Entity* entBefore = field.getEntity(entityId);
    EntityType etype = entBefore ? entBefore->getType() : EntityType::Enemy; // default assume enemy
    if (field.getPosition(entityId, r, c)) {
        field.removeEntity(entityId);
    }

    // Log if an enemy died
    if (etype == EntityType::Enemy) {
        EventSystem::getInstance().logEnemyDefeated(entityId);
    }

    if (killerId == field.getPlayerId()) {
        auto player = field.getPlayerById(killerId);
        if (player) {
            processKillRewards(field, *player);
        }
    }
}

void CombatSystem::processKillRewards(Field& field, Player& player) {
    player.addScore(10);
    
    ++killCounter_;
    // Inform about accumulated kills (optional)
    // (We do not create a separate event type for kill counter; the spell grant is logged via Hand::addRandomSpell)
    
    if (killCounter_ >= KILLS_FOR_SPELL) {
        // Give new random spell to player; Hand::addRandomSpell will log the spell learned event
        killCounter_ = 0;
        if (player.hand().size() < player.hand().capacity()) {
            player.hand().addRandomSpell();
        }
    }
}