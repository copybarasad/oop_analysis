#pragma once
#ifndef GAMEEVENTMANAGER_H
#define GAMEEVENTMANAGER_H

#include "Logger.h"
#include <string>
#include <unordered_map>

class GameEventManager {
private:
    GameEventManager() = default;

public:
    static GameEventManager& getInstance() {
        static GameEventManager instance;
        return instance;
    }

    void logPlayerMove(int fromX, int fromY, int toX, int toY);
    void logPlayerAttack(const std::string& target, int damage);
    void logPlayerDamageTaken(int damage, int remainingHP);
    void logPlayerSpellGained(const std::string& spellName);
    void logPlayerSpellUsed(const std::string& spellName, int targetX, int targetY);
    void logPlayerLevelUp(int newLevel);

    void logEnemySpawned(int x, int y, int health);
    void logEnemyMoved(int fromX, int fromY, int toX, int toY);
    void logEnemyAttack(int damage);
    void logEnemyDefeated(int x, int y);

    void logTowerSpawnEnemy(int x, int y, int enemyHealth);
    void logTowerAttack(int damage);

    void logSpellCast(const std::string& spellName, int damage, int range);
    void logSpellDamage(const std::string& spellName, int damage, int targetX, int targetY);
    void logTrapActivated(int x, int y, const std::string& entityType);

    void logGameStarted();
    void logGameSaved(const std::string& saveName);
    void logGameLoaded(const std::string& saveName);
    void logLevelStarted(int level, const std::string& levelName);
    void logLevelCompleted(int level);
    void logGameOver();
    void logVictory();

    void logEvent(GameEventType type, const std::string& description,
        const std::unordered_map<std::string, std::string>& details = {});

    GameEventManager(const GameEventManager&) = delete;
    GameEventManager& operator=(const GameEventManager&) = delete;
};

#define EVENT_MANAGER GameEventManager::getInstance()

#endif 