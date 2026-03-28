#include "game.h"
#include "game_initializer.h"
#include "game_turn_processor.h"
#include "game_end_handler.h"
#include "game_state_manager.h"
#include "game_input_handler.h"
#include "game_persistence.h"
#include "level_manager.h"
#include "upgrade_system.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game(size_t fieldWidth, size_t fieldHeight)
    : field(fieldWidth, fieldHeight), gameOver(false), playerSkippingTurn(false),
      player(7, 7), enhancementLevel(0), currentLevel(1), playerTurnCounter(0) {
    std::srand(std::time(nullptr));
    combatSystem = std::make_unique<CombatSystem>(player, enemies, buildings);
    enemyManager = std::make_unique<EnemyManager>(enemies, field, player, allies);
    entityManager = std::make_unique<EntityManager>(enemies, allies, buildings, towers, field, player);
    spellManager = std::make_unique<SpellManager>(3);
    renderer = std::make_unique<Renderer>();
    stateManager = std::make_unique<GameStateManager>(*this);
}

Game::~Game() {
}

void Game::initializeGame() {
    GameInitializer::initializeNewGame(*this);
}

void Game::nextLevel() {
    LevelManager::advanceToNextLevel(*this);
}

std::pair<int, int> Game::generateRandomValidPosition() const {
    int x, y;
    do {
        x = rand() % field.getWidth();
        y = rand() % field.getHeight();
    } while (!field.isPassable(x, y) || 
             (x == player.getPosition().first && y == player.getPosition().second));
    return {x, y};
}

void Game::run() {
    while (true) {
        std::cout << "=== NEW GAME / LOAD OR START ===\n";
        
        if (GameInputHandler::getYesNoChoice("Load saved game? (y/n): ")) {
            std::string fname = GameInputHandler::getFilename("Enter save filename: ");
            if (!stateManager->loadGame(fname)) {
                std::cout << "Failed to load: " << getLastError() << "\n";
                std::cout << "Starting new game instead.\n";
                initializeGame();
            } else {
                std::cout << "Loaded " << fname << "\n";
            }
        } else {
            std::cout << "Starting new game...\n";
            initializeGame();
        }

        GameInitializer::displayControls();
        stateManager->displayGameField();

        while (!isGameOver()) {
            stateManager->displayPlayerStatus();
            stateManager->displaySpellList();

            if (!player.isAlive()) {
                gameOver = true;
                break;
            }

            char command = GameInputHandler::getPlayerCommand();

            if (command == 'q' || command == 'Q') {
                return;
            }

            if (command == 'z' || command == 'Z') {
                std::string fname = GameInputHandler::getFilename("Enter save filename: ");
                if (stateManager->saveGame(fname)) {
                    std::cout << "Game saved to " << fname << "\n";
                } else {
                    std::cout << "Failed to save game to " << fname << ": " << getLastError() << "\n";
                }
                continue;
            }

            if (command == 'l' || command == 'L') {
                std::string fname = GameInputHandler::getFilename("Enter load filename: ");
                if (stateManager->loadGame(fname)) {
                    std::cout << "Game loaded from " << fname << "\n";
                    stateManager->displayGameField();
                } else {
                    std::cout << "Failed to load game from " << fname << ": " << getLastError() << "\n";
                }
                continue;
            }

            if (!GameTurnProcessor::processFullTurn(*this, command)) {
                break;
            }

            stateManager->displayGameField();

            if (GameEndHandler::checkPlayerWon(*this)) {
                std::cout << "Congratulations! You cleared level " << currentLevel << "!\n";
                std::cout << "Final score for this level: " << player.getScore() << " points\n";

                UpgradeSystem::displayUpgradeMenu(*this);

                LevelManager::advanceToNextLevel(*this);
                stateManager->displayGameField();
                continue;
            }
        }

        if (!GameEndHandler::handleGameEnd(*this)) {
            break;
        }
    }
}

bool Game::isGameOver() const {
    return gameOver;
}

bool Game::playerWon() const {
    return enemies.empty() && player.isAlive();
}

int Game::getPlayerScore() const {
    return player.getScore();
}

void Game::setLastError(const std::string& e) {
    lastError = e;
    try {
        std::ofstream log("last_error.log", std::ios::app);
        if (log.is_open()) {
            log << "[" << currentLevel << "] " << e << "\n";
        }
    } catch (...) {
    }
}

void Game::displayField() const {
    const_cast<Game*>(this)->stateManager->displayGameField();
}

void Game::displayPlayerStatus() const {
    const_cast<Game*>(this)->stateManager->displayPlayerStatus();
}

void Game::displaySpells() const {
    const_cast<Game*>(this)->stateManager->displaySpellList();
}

bool Game::saveToFile(const std::string& filename) const {
    return const_cast<Game*>(this)->stateManager->saveGame(filename);
}

bool Game::loadFromFile(const std::string& filename) {
    return stateManager->loadGame(filename);
}

void Game::processPlayerTurn(char command) {
    GameTurnProcessor::processPlayerTurn(*this, command);
}

void Game::processEnemyTurns() {
    GameTurnProcessor::processEnemyTurns(*this);
}

void Game::processAlliesTurn() {
    GameTurnProcessor::processAlliesTurn(*this);
}

void Game::processBuildingTurns() {
    GameTurnProcessor::processBuildingTurns(*this);
}

void Game::processTowerTurns() {
    GameTurnProcessor::processTowerTurns(*this);
}

void Game::saveToStream(std::ostream& out) const {
    GamePersistence::saveToStream(*this, out);
}

void Game::loadFromStream(std::istream& in) {
    GamePersistence::loadFromStream(*this, in);
}

void Game::cleanupDestroyedBuildings() {
    entityManager->cleanupDestroyedBuildings();
}

void Game::cleanupDestroyedTowers() {
    entityManager->cleanupDestroyedTowers();
}

void Game::cleanupDestroyedEntities(std::vector<EnemyBuilding>& entities) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), 
        [](const EnemyBuilding& e) { return !e.isAlive(); }), entities.end());
}

void Game::cleanupDestroyedEntities(std::vector<EnemyTower>& entities) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), 
        [](const EnemyTower& e) { return !e.isAlive(); }), entities.end());
}