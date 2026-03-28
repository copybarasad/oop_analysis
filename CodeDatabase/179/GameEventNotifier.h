#ifndef GAMEEVENTNOTIFIER_H
#define GAMEEVENTNOTIFIER_H

#include "Position.h"  // ДОБАВИТЬ ЭТУ СТРОКУ
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <memory>

enum class GameEventType {
    PLAYER_MOVE,
    PLAYER_ATTACK,
    PLAYER_CAST_SPELL,
    PLAYER_DAMAGE_TAKEN,
    PLAYER_HEAL,
    ENEMY_SPAWN,
    ENEMY_DESTROYED,
    BUILDING_DESTROYED,
    TOWER_DESTROYED,
    ALLY_SUMMONED,
    TRAP_TRIGGERED,
    LEVEL_UP,
    GAME_SAVED,
    GAME_LOADED,
    GAME_OVER
};

struct GameEvent {
    GameEventType type;
    std::string description;
    Position position; // Если применимо
    int value; // Значение (урон, здоровье и т.д.)
    
    GameEvent(GameEventType t, const std::string& desc, Position pos = Position(), int val = 0)
        : type(t), description(desc), position(pos), value(val) {}
};

class GameEventNotifier {
private:
    static GameEventNotifier* instance;
    std::vector<std::function<void(const GameEvent&)>> observers;
    
    GameEventNotifier() = default;
    
public:
    static GameEventNotifier& getInstance();
    
    void addObserver(std::function<void(const GameEvent&)> observer);
    void notify(const GameEvent& event);
    
    // Вспомогательные методы для создания стандартных событий
    void notifyPlayerMove(const Position& from, const Position& to);
    void notifyPlayerAttack(const Position& target, int damage, const std::string& targetName);
    void notifyPlayerCastSpell(const std::string& spellName, const Position& target);
    void notifyPlayerDamage(int damage, const std::string& source);
    void notifyEnemyDestroyed(const Position& pos, const std::string& enemyName);
    void notifyBuildingDestroyed(const Position& pos, const std::string& buildingName);
    void notifyTowerDestroyed(const Position& pos, const std::string& towerName);
    void notifyAllySummoned(const Position& pos);
    void notifyTrapTriggered(const Position& pos, int damage);
    void notifyLevelUp(int newLevel);
    void notifyGameSaved(const std::string& filename);
    void notifyGameLoaded(const std::string& filename);
    void notifyGameOver(bool win);
};

#endif
