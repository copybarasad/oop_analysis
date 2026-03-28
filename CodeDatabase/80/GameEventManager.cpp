#include "GameEventManager.h"

void GameEventManager::logPlayerMove(int fromX, int fromY, int toX, int toY) {
    auto details = Logger::createMoveDetails(fromX, fromY, toX, toY);
    LOG_EVENT(GameEventType::PLAYER_MOVE,
        "Player moved to new position", details);
}

void GameEventManager::logPlayerAttack(const std::string& target, int damage) {
    auto details = Logger::createDamageDetails("Player", target, damage);
    LOG_EVENT(GameEventType::PLAYER_ATTACK,
        "Player attacked " + target, details);
}

void GameEventManager::logEvent(GameEventType type, const std::string& description,
    const std::unordered_map<std::string, std::string>& details) {
    Logger::getInstance().logEvent(type, description, details);
}

void GameEventManager::logPlayerDamageTaken(int damage, int remainingHP) {
    std::unordered_map<std::string, std::string> details = {
        {"damage_taken", std::to_string(damage)},
        {"remaining_hp", std::to_string(remainingHP)}
    };
    LOG_EVENT(GameEventType::PLAYER_DAMAGE_TAKEN,
        "Player took damage", details);
}

void GameEventManager::logPlayerSpellGained(const std::string& spellName) {
    std::unordered_map<std::string, std::string> details = {
        {"spell", spellName}
    };
    LOG_EVENT(GameEventType::PLAYER_SPELL_GAINED,
        "Player gained new spell: " + spellName, details);
}

void GameEventManager::logPlayerSpellUsed(const std::string& spellName, int targetX, int targetY) {
    std::unordered_map<std::string, std::string> details = {
        {"spell", spellName},
        {"target", "(" + std::to_string(targetX) + "," + std::to_string(targetY) + ")"}
    };
    LOG_EVENT(GameEventType::PLAYER_SPELL_USED,
        "Player used spell: " + spellName, details);
}

void GameEventManager::logPlayerLevelUp(int newLevel) {
    std::unordered_map<std::string, std::string> details = {
        {"new_level", std::to_string(newLevel)}
    };
    LOG_EVENT(GameEventType::PLAYER_LEVEL_UP,
        "Player leveled up to level " + std::to_string(newLevel), details);
}

void GameEventManager::logEnemySpawned(int x, int y, int health) {
    auto details = Logger::createEntityDetails("Enemy", x, y, health);
    LOG_EVENT(GameEventType::ENEMY_SPAWNED,
        "Enemy spawned", details);
}

void GameEventManager::logEnemyMoved(int fromX, int fromY, int toX, int toY) {
    auto details = Logger::createMoveDetails(fromX, fromY, toX, toY);
    LOG_EVENT(GameEventType::ENEMY_MOVED,
        "Enemy moved", details);
}

void GameEventManager::logEnemyAttack(int damage) {
    std::unordered_map<std::string, std::string> details = {
        {"damage", std::to_string(damage)}
    };
    LOG_EVENT(GameEventType::ENEMY_ATTACK,
        "Enemy attacked player", details);
}

void GameEventManager::logEnemyDefeated(int x, int y) {
    std::unordered_map<std::string, std::string> details = {
        {"position", "(" + std::to_string(x) + "," + std::to_string(y) + ")"}
    };
    LOG_EVENT(GameEventType::ENEMY_DEFEATED,
        "Enemy defeated", details);
}

void GameEventManager::logTowerSpawnEnemy(int x, int y, int enemyHealth) {
    std::unordered_map<std::string, std::string> details = {
        {"tower_position", "(" + std::to_string(x) + "," + std::to_string(y) + ")"},
        {"enemy_health", std::to_string(enemyHealth)}
    };
    LOG_EVENT(GameEventType::TOWER_SPAWN_ENEMY,
        "Tower spawned enemy", details);
}

void GameEventManager::logTowerAttack(int damage) {
    std::unordered_map<std::string, std::string> details = {
        {"damage", std::to_string(damage)}
    };
    LOG_EVENT(GameEventType::TOWER_ATTACK,
        "Tower attacked", details);
}

void GameEventManager::logSpellCast(const std::string& spellName, int damage, int range) {
    auto details = Logger::createSpellDetails(spellName, damage, range);
    LOG_EVENT(GameEventType::SPELL_CAST,
        "Spell cast: " + spellName, details);
}

void GameEventManager::logSpellDamage(const std::string& spellName, int damage, int targetX, int targetY) {
    std::unordered_map<std::string, std::string> details = {
        {"spell", spellName},
        {"damage", std::to_string(damage)},
        {"target", "(" + std::to_string(targetX) + "," + std::to_string(targetY) + ")"}
    };
    LOG_EVENT(GameEventType::SPELL_DAMAGE,
        "Spell damage dealt", details);
}

void GameEventManager::logTrapActivated(int x, int y, const std::string& entityType) {
    std::unordered_map<std::string, std::string> details = {
        {"position", "(" + std::to_string(x) + "," + std::to_string(y) + ")"},
        {"entity", entityType}
    };
    LOG_EVENT(GameEventType::SPELL_TRAP_ACTIVATED,
        "Trap activated by " + entityType, details);
}

void GameEventManager::logGameStarted() {
    LOG_EVENT(GameEventType::GAME_STARTED, "Game started", {});
}

void GameEventManager::logGameSaved(const std::string& saveName) {
    std::unordered_map<std::string, std::string> details = {
        {"save_name", saveName}
    };
    LOG_EVENT(GameEventType::GAME_SAVED, "Game saved", details);
}

void GameEventManager::logGameLoaded(const std::string& saveName) {
    std::unordered_map<std::string, std::string> details = {
        {"save_name", saveName}
    };
    LOG_EVENT(GameEventType::GAME_LOADED, "Game loaded", details);
}

void GameEventManager::logLevelStarted(int level, const std::string& levelName) {
    std::unordered_map<std::string, std::string> details = {
        {"level", std::to_string(level)},
        {"name", levelName}
    };
    LOG_EVENT(GameEventType::LEVEL_STARTED, "Level started", details);
}

void GameEventManager::logLevelCompleted(int level) {
    std::unordered_map<std::string, std::string> details = {
        {"level", std::to_string(level)}
    };
    LOG_EVENT(GameEventType::LEVEL_COMPLETED, "Level completed", details);
}

void GameEventManager::logGameOver() {
    LOG_EVENT(GameEventType::GAME_OVER, "Game Over", {});
}

void GameEventManager::logVictory() {
    LOG_EVENT(GameEventType::VICTORY, "Victory!", {});
}