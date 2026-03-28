#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "GameEvent.h"
#include <memory>

class IGameObserver {
public:
    virtual ~IGameObserver() = default;
    virtual void onEvent(const GameEvent& event) = 0;
    
    virtual void onPlayerMoved(const Position& from, const Position& to) {
        onEvent(PlayerMovedEvent(from, to));
    }
    
    virtual void onPlayerAttacked(const std::string& target, int damage) {
        onEvent(PlayerAttackedEvent(target, damage));
    }
    
    virtual void onPlayerDamaged(int damage, const std::string& source) {
        onEvent(PlayerDamagedEvent(damage, source));
    }
    
    virtual void onEnemyKilled(const Position& position, int coinsReward) {
        onEvent(EnemyKilledEvent(position, coinsReward));
    }
    
    virtual void onTowerDestroyed(const Position& position) {
        onEvent(TowerDestroyedEvent(position));
    }
    
    virtual void onSpellUsed(const std::string& spellName, const Position& target) {
        onEvent(SpellUsedEvent(spellName, target));
    }
    
    virtual void onCommandExecuted(const std::string& command) {
        onEvent(CommandExecutedEvent(command));
    }
    
    virtual void onLevelUp(int newLevel) {
        onEvent(GameEvent(EventType::LEVEL_UP, 
                         "Player leveled up to level " + std::to_string(newLevel)));
    }
    
    virtual void onGameSaved() {
        onEvent(GameEvent(EventType::GAME_SAVED, "Game saved"));
    }
    
    virtual void onGameLoaded() {
        onEvent(GameEvent(EventType::GAME_LOADED, "Game loaded"));
    }
    
    virtual void onGameStarted() {
        onEvent(GameEvent(EventType::GAME_STARTED, "Game started"));
    }
    
    virtual void onGameEnded() {
        onEvent(GameEvent(EventType::GAME_ENDED, "Game ended"));
    }
};

#endif