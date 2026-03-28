#include "GameEvent.h"

GameEvent::GameEvent(EventType type, const std::string& description)
    : type_(type),
      description_(description) {}

EventType GameEvent::GetType() const {
    return type_;
}

const std::string& GameEvent::GetDescription() const {
    return description_;
}

std::string GameEvent::ToString() const {
    return "[" + EventTypeToString(type_) + "] " + description_;
}

std::string GameEvent::EventTypeToString(EventType type) {
    switch (type) {
        case EventType::GAME_START: return "GAME_START";
        case EventType::GAME_END: return "GAME_END";
        case EventType::LEVEL_START: return "LEVEL_START";
        case EventType::LEVEL_COMPLETE: return "LEVEL_COMPLETE";
        case EventType::PLAYER_MOVE: return "PLAYER_MOVE";
        case EventType::PLAYER_ATTACK: return "PLAYER_ATTACK";
        case EventType::PLAYER_DAMAGE: return "PLAYER_DAMAGE";
        case EventType::PLAYER_HEAL: return "PLAYER_HEAL";
        case EventType::PLAYER_DEATH: return "PLAYER_DEATH";
        case EventType::PLAYER_LEVEL_UP: return "PLAYER_LEVEL_UP";
        case EventType::ENEMY_SPAWN: return "ENEMY_SPAWN";
        case EventType::ENEMY_MOVE: return "ENEMY_MOVE";
        case EventType::ENEMY_ATTACK: return "ENEMY_ATTACK";
        case EventType::ENEMY_DEATH: return "ENEMY_DEATH";
        case EventType::ALLY_SPAWN: return "ALLY_SPAWN";
        case EventType::ALLY_MOVE: return "ALLY_MOVE";
        case EventType::ALLY_ATTACK: return "ALLY_ATTACK";
        case EventType::ALLY_DEATH: return "ALLY_DEATH";
        case EventType::SPELL_CAST: return "SPELL_CAST";
        case EventType::SPELL_ACQUIRED: return "SPELL_ACQUIRED";
        case EventType::TOWER_ATTACK: return "TOWER_ATTACK";
        case EventType::BUILDING_SPAWN: return "BUILDING_SPAWN";
        case EventType::TRAP_TRIGGERED: return "TRAP_TRIGGERED";
        case EventType::SAVE_GAME: return "SAVE_GAME";
        case EventType::LOAD_GAME: return "LOAD_GAME";
        case EventType::UPGRADE_SELECTED: return "UPGRADE_SELECTED";
        default: return "UNKNOWN";
    }
}

GameEvent GameEventFactory::PlayerMove(int from_x, int from_y, int to_x, int to_y) {
    std::ostringstream oss;
    oss << "Player moved from (" << from_x << ", " << from_y << ") to (" << to_x << ", " << to_y << ")";
    return GameEvent(EventType::PLAYER_MOVE, oss.str());
}

GameEvent GameEventFactory::PlayerAttack(int target_x, int target_y, int damage) {
    std::ostringstream oss;
    oss << "Player attacked target at (" << target_x << ", " << target_y << ") for " << damage << " damage";
    return GameEvent(EventType::PLAYER_ATTACK, oss.str());
}

GameEvent GameEventFactory::PlayerDamage(int damage, int remaining_health) {
    std::ostringstream oss;
    oss << "Player took " << damage << " damage, " << remaining_health << " HP remaining";
    return GameEvent(EventType::PLAYER_DAMAGE, oss.str());
}

GameEvent GameEventFactory::EnemySpawn(int x, int y, int health) {
    std::ostringstream oss;
    oss << "Enemy spawned at (" << x << ", " << y << ") with " << health << " HP";
    return GameEvent(EventType::ENEMY_SPAWN, oss.str());
}

GameEvent GameEventFactory::EnemyDeath(int x, int y, int score) {
    std::ostringstream oss;
    oss << "Enemy died at (" << x << ", " << y << "), +" << score << " score";
    return GameEvent(EventType::ENEMY_DEATH, oss.str());
}

GameEvent GameEventFactory::SpellCast(const std::string& spell_name, int target_x, int target_y) {
    std::ostringstream oss;
    oss << "Spell '" << spell_name << "' cast at (" << target_x << ", " << target_y << ")";
    return GameEvent(EventType::SPELL_CAST, oss.str());
}

GameEvent GameEventFactory::SpellAcquired(const std::string& spell_name) {
    return GameEvent(EventType::SPELL_ACQUIRED, "Acquired spell: " + spell_name);
}

GameEvent GameEventFactory::LevelComplete(int level, int score) {
    std::ostringstream oss;
    oss << "Level " << level << " completed with score " << score;
    return GameEvent(EventType::LEVEL_COMPLETE, oss.str());
}

GameEvent GameEventFactory::TrapTriggered(int x, int y, int damage) {
    std::ostringstream oss;
    oss << "Trap triggered at (" << x << ", " << y << ") dealing " << damage << " damage";
    return GameEvent(EventType::TRAP_TRIGGERED, oss.str());
}

GameEvent GameEventFactory::AllySpawn(int x, int y) {
    std::ostringstream oss;
    oss << "Ally spawned at (" << x << ", " << y << ")";
    return GameEvent(EventType::ALLY_SPAWN, oss.str());
}

GameEvent GameEventFactory::SaveGame(const std::string& filename) {
    return GameEvent(EventType::SAVE_GAME, "Game saved to: " + filename);
}

GameEvent GameEventFactory::LoadGame(const std::string& filename) {
    return GameEvent(EventType::LOAD_GAME, "Game loaded from: " + filename);
}

GameEvent GameEventFactory::UpgradeSelected(const std::string& upgrade_name) {
    return GameEvent(EventType::UPGRADE_SELECTED, "Selected upgrade: " + upgrade_name);
}
