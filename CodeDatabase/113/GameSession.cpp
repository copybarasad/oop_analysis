#include "GameSession.h"
#include "JSONSaveManager.h"
#include "DirectDamageSpell.h"
#include <iostream>

GameSession::GameSession()
    : m_player(10000, 10, 5, 50, 5)
    , m_spellManager(5)
    , m_isRunning(false)
    , m_saveFileName("game_save.json") {
}

bool GameSession::initializeNewGame() {
    m_isRunning = true;
    m_levelManager.reset();
    m_spellManager.resetCounter();

    if (m_levelManager.initializeLevel(m_gameState, 1)) {
        m_player.setHealth(10000);
        m_player.setPosition(0, 0);
        setupPlayerReferences();

        m_player.addSpellToHand(std::make_unique<DirectDamageSpell>("Fireball", 30, 5, 25));

        std::cout << "New game initialized successfully" << std::endl;
        return true;
    }

    return false;
}

bool GameSession::loadGame() {
    bool success = JSONSaveManager::loadGameState(m_gameState, m_saveFileName);

    if (success) {
        m_isRunning = true;
        m_player.setHealth(m_gameState.playerHealth);
        m_player.setPosition(m_gameState.playerX, m_gameState.playerY);
        setupPlayerReferences();
        m_spellManager.resetCounter();

        std::cout << "Game loaded successfully" << std::endl;
    }
    else {
        std::cout << "Load failed: " << JSONSaveManager::getLastError() << std::endl;
        initializeNewGame();
    }

    return success;
}

bool GameSession::saveGame() {
    // Обновление состояния игрока в GameState
    m_gameState.playerHealth = m_player.getHealth();
    m_gameState.playerX = m_player.getPositionX();
    m_gameState.playerY = m_player.getPositionY();

    bool success = JSONSaveManager::saveGameState(m_gameState, m_saveFileName);

    if (!success) {
        std::cout << "Save failed: " << JSONSaveManager::getLastError() << std::endl;
    }

    return success;
}

void GameSession::processTurn() {
    m_turnManager.processPlayerTurn(this);
    m_spellManager.update(m_player);
}

void GameSession::processEnemyTurns() {
    for (auto& enemy : m_gameState.enemies) {
        if (enemy.isAlive()) {
            enemy.makeMove(m_gameState.field, m_player);
        }
    }

    for (auto& tower : m_gameState.towers) {
        if (tower.isAlive()) {
            tower.makeMove(m_gameState.field, m_player);
        }
    }
}

bool GameSession::isVictory() const {
    for (const auto& enemy : m_gameState.enemies) {
        if (enemy.isAlive()) return false;
    }

    for (const auto& tower : m_gameState.towers) {
        if (tower.isAlive()) return false;
    }

    return true;
}

bool GameSession::isGameOver() const {
    return !m_player.isAlive() || m_gameState.turnCount > 100;
}

void GameSession::advanceToNextLevel() {
    m_levelManager.advanceToNextLevel(m_gameState, m_player);
    m_spellManager.resetCounter();

    m_isRunning = true;
}

void GameSession::setupPlayerReferences() {
    m_player.setEnemiesReference(m_gameState.enemies);
    m_player.setTowersReference(m_gameState.towers);
}