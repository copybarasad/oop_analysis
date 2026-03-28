#include "GameLogic.h"
#include <iostream>
#include "InputHandler.h"
#include "Command.h"
#include "CommandType.h"

GameLogic::GameLogic(GameState& state, ActionProcessor& processor, ShopSystem& shop)
    : gameState(state), actionProcessor(processor), shopSystem(shop),
    gameRunning(false), playerTurn(true) {
}

void GameLogic::startGame() {
    gameRunning = true;
    playerTurn = true;
    std::cout << "Game started!" << std::endl;
}

void GameLogic::stopGame() {
    gameRunning = false;
    std::cout << "Game stopped." << std::endl;
}

bool GameLogic::isGameRunning() const {
    return gameRunning;
}

bool GameLogic::processPlayerAction(InputHandler& inputHandler) {
    if (!playerTurn) {
        std::cout << "Not your turn!" << std::endl;
        return false;
    }

    // НОВЫЙ КОД: получаем тип команды и создаем объект
    CommandType commandType = inputHandler.getCommandType();

    if (commandType == CommandType::INVALID) {
        std::cout << "Invalid command!" << std::endl;
        return false;
    }

    auto command = inputHandler.createCommand(commandType);
    if (!command) {
        std::cout << "Failed to create command!" << std::endl;
        return false;
    }

    // Выполняем команду
    bool actionSuccess = command->execute(gameState);
    if (actionSuccess) {
        playerTurn = false;
        std::cout << "Player turn ended." << std::endl;

        // Если это команда выхода, останавливаем игру
        if (commandType == CommandType::QUIT_GAME) {
            gameRunning = false;
        }
    }

    return actionSuccess;
}

void GameLogic::processEnemyTurn() {
    if (playerTurn) {
        return;
    }

    std::cout << "=== ENEMY TURN ===" << std::endl;
    actionProcessor.processEnemyMoves(gameState);

    if (!isPlayerAlive()) {
        std::cout << "Player was defeated by enemies!" << std::endl;
        gameRunning = false;
    }
    else {
        playerTurn = true;
        std::cout << "Enemy turn ended. Player's turn." << std::endl;
    }
}

bool GameLogic::isPlayerTurn() const {
    return playerTurn;
}

bool GameLogic::isPlayerAlive() const {
    return gameState.getPlayer().isAlive();
}

bool GameLogic::checkPlayerVictory() const {
    const auto& enemies = gameState.getEnemies();
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return true;
}

bool GameLogic::isGameOver() const {
    return !isPlayerAlive() || checkPlayerVictory();
}

GameState& GameLogic::getGameState() {
    return gameState;
}

const GameState& GameLogic::getGameState() const {
    return gameState;
}

int GameLogic::getPlayerScore() const {
    return gameState.getPlayer().getScore();
}

int GameLogic::getPlayerHealth() const {
    return gameState.getPlayer().getHealth();
}

int GameLogic::getPlayerMana() const {
    return gameState.getPlayer().getMana();
}

void GameLogic::reset() {
    playerTurn = true;
    gameRunning = false;
}