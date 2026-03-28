#ifndef EVENT_PUBLISHER_H
#define EVENT_PUBLISHER_H

#include "game_event.h"
#include "event_bus.h"
#include <memory>

class EventPublisher {
public:
    static void publishMoveEvent(const std::string& entity, 
                                std::pair<int, int> from, 
                                std::pair<int, int> to) {
        EventBus::getInstance().publish(
            std::make_shared<MoveEvent>(entity, from, to)
        );
    }
    
    static void publishAttackEvent(const std::string& attacker, 
                                  const std::string& defender, 
                                  int damage) {
        EventBus::getInstance().publish(
            std::make_shared<AttackEvent>(attacker, defender, damage)
        );
    }

    static void publishDamageEvent(const std::string& entity, 
                                  int damage, 
                                  int remainingHealth) {
        EventBus::getInstance().publish(
            std::make_shared<DamageEvent>(entity, damage, remainingHealth)
        );
    }

    static void publishDeathEvent(const std::string& entity, 
                                 int reward = 0) {
        EventBus::getInstance().publish(
            std::make_shared<DeathEvent>(entity, reward)
        );
    }

    static void publishSpellCastEvent(const std::string& caster, 
                                     const std::string& spellName, 
                                     std::pair<int, int> target) {
        EventBus::getInstance().publish(
            std::make_shared<SpellCastEvent>(caster, spellName, target)
        );
    }

    static void publishSpellAcquiredEvent(const std::string& entity, 
                                         const std::string& spellName) {
        EventBus::getInstance().publish(
            std::make_shared<SpellAcquiredEvent>(entity, spellName)
        );
    }

    static void publishLevelCompletedEvent(int level, int score) {
        EventBus::getInstance().publish(
            std::make_shared<LevelCompletedEvent>(level, score)
        );
    }

    static void publishGameOverEvent(bool won, int finalScore) {
        EventBus::getInstance().publish(
            std::make_shared<GameOverEvent>(won, finalScore)
        );
    }
};

#endif