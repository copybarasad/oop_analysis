#include "GameEventNotifier.h"
#include "Logger.h"

GameEventNotifier* GameEventNotifier::instance = nullptr;

GameEventNotifier& GameEventNotifier::getInstance() {
    if (!instance) {
        instance = new GameEventNotifier();
    }
    return *instance;
}

void GameEventNotifier::addObserver(std::function<void(const GameEvent&)> observer) {
    observers.push_back(observer);
}

void GameEventNotifier::notify(const GameEvent& event) {
    for (const auto& observer : observers) {
        observer(event);
    }
}

void GameEventNotifier::notifyPlayerMove(const Position& from, const Position& to) {
    std::string desc = "Player moved from (" + std::to_string(from.x) + "," + std::to_string(from.y) +
                       ") to (" + std::to_string(to.x) + "," + std::to_string(to.y) + ")";
    notify(GameEvent(GameEventType::PLAYER_MOVE, desc, to));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyPlayerAttack(const Position& target, int damage, const std::string& targetName) {
    std::string desc = "Player attacked " + targetName + " at (" + std::to_string(target.x) +
                       "," + std::to_string(target.y) + ") for " + std::to_string(damage) + " damage";
    notify(GameEvent(GameEventType::PLAYER_ATTACK, desc, target, damage));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyPlayerCastSpell(const std::string& spellName, const Position& target) {
    std::string desc = "Player cast " + spellName + " at (" + std::to_string(target.x) +
                       "," + std::to_string(target.y) + ")";
    notify(GameEvent(GameEventType::PLAYER_CAST_SPELL, desc, target));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyPlayerDamage(int damage, const std::string& source) {
    std::string desc = "Player took " + std::to_string(damage) + " damage from " + source;
    notify(GameEvent(GameEventType::PLAYER_DAMAGE_TAKEN, desc, Position(), damage));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyEnemyDestroyed(const Position& pos, const std::string& enemyName) {
    std::string desc = enemyName + " destroyed at (" + std::to_string(pos.x) +
                       "," + std::to_string(pos.y) + ")";
    notify(GameEvent(GameEventType::ENEMY_DESTROYED, desc, pos));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyBuildingDestroyed(const Position& pos, const std::string& buildingName) {
    std::string desc = buildingName + " destroyed at (" + std::to_string(pos.x) +
                       "," + std::to_string(pos.y) + ")";
    notify(GameEvent(GameEventType::BUILDING_DESTROYED, desc, pos));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyTowerDestroyed(const Position& pos, const std::string& towerName) {
    std::string desc = towerName + " destroyed at (" + std::to_string(pos.x) +
                       "," + std::to_string(pos.y) + ")";
    notify(GameEvent(GameEventType::TOWER_DESTROYED, desc, pos));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyAllySummoned(const Position& pos) {
    std::string desc = "Ally summoned at (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")";
    notify(GameEvent(GameEventType::ALLY_SUMMONED, desc, pos));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyTrapTriggered(const Position& pos, int damage) {
    std::string desc = "Trap triggered at (" + std::to_string(pos.x) + "," +
                       std::to_string(pos.y) + ") dealing " + std::to_string(damage) + " damage";
    notify(GameEvent(GameEventType::TRAP_TRIGGERED, desc, pos, damage));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyLevelUp(int newLevel) {
    std::string desc = "Level up! Now at level " + std::to_string(newLevel);
    notify(GameEvent(GameEventType::LEVEL_UP, desc));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyGameSaved(const std::string& filename) {
    std::string desc = "Game saved to " + filename;
    notify(GameEvent(GameEventType::GAME_SAVED, desc));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyGameLoaded(const std::string& filename) {
    std::string desc = "Game loaded from " + filename;
    notify(GameEvent(GameEventType::GAME_LOADED, desc));
    LOG_GAME_EVENT(desc);
}

void GameEventNotifier::notifyGameOver(bool win) {
    std::string desc = win ? "Game completed successfully!" : "Game over - player died";
    notify(GameEvent(GameEventType::GAME_OVER, desc));
    LOG_GAME_EVENT(desc);
}
