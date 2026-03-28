#include "GameController.h"
#include "SpellFactory.h"
#include <iostream>
#include <random>

GameController::GameController(int width, int height, bool loadFromSave)
    : turnCount(0), gameRunning(true), wandererCanMove(true), wandererOnSlowCell(false), 
      field(width, height), wanderer(), wandererPosition(), currentLevel(1), renderer() {
    
    try {
        beastManager = std::make_unique<BeastManager>(field, wandererPosition, wanderer);
        if (!beastManager) {
            throw InitializationException("BeastManager", "memory allocation failed");
        }
        
        buildingManager = std::make_unique<BuildingManager>(field, *beastManager, currentLevel);
        if (!buildingManager) {
            throw InitializationException("BuildingManager", "memory allocation failed");
        }
        
        combatManager = std::make_unique<CombatManager>(wanderer, *beastManager, *buildingManager, field, wandererPosition);
        if (!combatManager) {
            throw InitializationException("CombatManager", "memory allocation failed");
        }
        
        inputHandler = std::make_unique<InputHandler>(wanderer, field, wandererPosition, *combatManager, *beastManager, wandererCanMove, wandererOnSlowCell, gameRunning);
        if (!inputHandler) {
            throw InitializationException("InputHandler", "memory allocation failed");
        }

        if (!loadFromSave) {
            if (!spawnWanderer()) {
                throw InitializationException("GameController", "failed to spawn wanderer");
            }
            
            spawnInitialBeasts();
            buildingManager->spawnBuildings();
            buildingManager->spawnTowers();
            
            SpellFactory factory;
            auto spell = factory.createRandomSpell();
            if (spell) {
                if (!wanderer.learnSpell(std::move(spell))) {
                    throw InitializationException("GameController", "failed to learn initial spell");
                }
            }
        }
        
        std::cout << "GameController initialized successfully\n";
        
    } catch (const InitializationException& e) {
        throw InitializationException("GameController components", e.what());
    } catch (const std::exception& e) {
        throw InitializationException("GameController", std::string("unexpected error: ") + e.what());
    }
}

bool GameController::spawnWanderer() {
    try {
        int x, y;
        if (field.findEmptyPosition(x, y)) {
            wandererPosition = Position(x, y);
            field.getCell(x, y).setHasWanderer(true);
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        throw GameLogicException("spawnWanderer", e.what());
    }
}
        
void GameController::spawnInitialBeasts() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    int beastCount = dis(gen);
    int beastHealth = 20;
    int beastDamage = 8;

    for (int i = 0; i < beastCount; ++i) {
        int x, y;
        if (field.findEmptyPosition(x,y)) {
            Beast beast(beastHealth, beastDamage);
            beastManager->addBeast(Position(x, y), beast);
        }
    }
}

void GameController::spawnStrongerBeasts() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int baseBeastCount = 1 + currentLevel / 2;
    int beastHealth = 20 + currentLevel * 5;
    int beastDamage = 8 + currentLevel * 2;
    
    std::uniform_int_distribution<> countDis(baseBeastCount, baseBeastCount + 1);
    int beastCount = countDis(gen);
    
    std::cout << "Spawning " << beastCount << " stronger beasts (health: " << beastHealth << ", damage: " << beastDamage << ")\n";
    
    for (int i = 0; i < beastCount; ++i) {
        int x, y;
        if (field.findEmptyPosition(x, y)) {
            Beast beast(beastHealth, beastDamage);
            beastManager->addBeast(Position(x, y), beast);
        }
    }
}

std::vector<std::vector<char>> GameController::prepareFieldData() const {
    int width = field.getWidth();
    int height = field.getHeight();
    
    std::vector<std::vector<char>> fieldData(height, std::vector<char>(width, '.'));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Cell& cell = field.getCell(x, y);
            char symbol = '.';
            
            if (cell.getHasWanderer()) symbol = 'W';
            else if (cell.getHasBeast()) symbol = 'B';
            else if (cell.getHasBuilding()) {
                bool isTower = false;
                if (buildingManager) {
                    for (const auto& tower : buildingManager->getTowers()) {
                        if (tower.first.x == x && tower.first.y == y) {
                            symbol = 'X';
                            isTower = true;
                            break;
                        }
                    }
                }
                if (!isTower) {
                    symbol = 'T';
                }
            }
            else if (cell.getType() == CellType::OBSTACLE) symbol = '#';
            else if (cell.getType() == CellType::SLOW) symbol = '~';
            else if (cell.getHasTrap()) symbol = '^';
            
            fieldData[y][x] = symbol;
        }
    }
    
    return fieldData;
}

void GameController::renderGame() {
    renderer.renderLevelInfo(turnCount, currentLevel);
    renderer.renderPlayerStats(wanderer.getHealth(), wanderer.getShield(), wanderer.getMaxShield(),
                              wanderer.getMana(), wanderer.getMaxMana(), wanderer.getPoints(),
                              (wanderer.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED"));
    
    std::vector<std::vector<char>> fieldData = prepareFieldData();
    renderer.renderField(field.getWidth(), field.getHeight(), fieldData);
    
    renderer.renderSymbolsLegend();
    renderer.renderCombatInfo(wanderer.getCombatMode() == CombatMode::MELEE);
    renderer.renderControls();
}

bool GameController::isLevelComplete() const {
    const auto& beasts = beastManager->getBeasts();
    const auto& buildings = buildingManager->getBuildings();
    const auto& towers = buildingManager->getTowers();
    return beasts.empty() && buildings.empty() && towers.empty();
}

void GameController::loadNextLevel() {

    std::cout << "\nLevel " << currentLevel << " completed!\n";
    std::cout << "Moving to next level...\n";
    currentLevel++;
    
    wanderer.setHealthDirectly(wanderer.getMaxHealth());
    wanderer.setManaDirectly(wanderer.getMaxMana());
    wanderer.setShieldDirectly(wanderer.getMaxShield());
    
    Hand& hand = wanderer.getHand();
    int spellCount = hand.getSpellCount();
    if (spellCount > 0) {
        int spellsToRemove = spellCount / 2;
        std::cout << "Removing " << spellsToRemove << " spells...\n";
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (int i = 0; i < spellsToRemove; ++i) {
            if (hand.getSpellCount() > 0) {
                std::uniform_int_distribution<> dis(0, hand.getSpellCount() - 1);
                int indexToRemove = dis(gen);
                hand.removeSpell(indexToRemove);
            }
        }
    }
    int newWidth = 15 + currentLevel;
    int newHeight = 15 + currentLevel;
    field = GameField(newWidth, newHeight);

    beastManager->clearBeasts();
    
    auto& buildings = buildingManager->getBuildings();
    auto& towers = buildingManager->getTowers();
    
    buildings.clear();
    towers.clear();
    
    beastManager = std::make_unique<BeastManager>(field, wandererPosition, wanderer);
    buildingManager = std::make_unique<BuildingManager>(field, *beastManager, currentLevel);
    combatManager = std::make_unique<CombatManager>(wanderer, *beastManager, *buildingManager, field, wandererPosition);
    inputHandler = std::make_unique<InputHandler>(wanderer, field, wandererPosition, *combatManager, *beastManager, wandererCanMove, wandererOnSlowCell, gameRunning);

    Position oldPos = wandererPosition;
    field.getCell(oldPos.x, oldPos.y).setHasWanderer(false);
    
    spawnWanderer();
    spawnStrongerBeasts();

    buildingManager->spawnBuildings();
    buildingManager->spawnTowers();
    
    turnCount = 0;

    std::cout << "Ready for level " << currentLevel << "!\n";
    std::cout << "Enemies are stronger now! Be careful!\n";
}

void GameController::runGame() {
    std::cout << "=== Game Started ===\n";
    
    while (gameRunning && wanderer.getIsAlive()) {
        turnCount++;
        renderGame();
        
        wanderer.getHand().resetSpells();

        std::cout << "Enter command: ";
        char input;
        std::cin >> input;

        if (input == 'p' || input == 'P') {
            gameRunning = false;
            std::cout << "Game paused and saved. Returning to main menu...\n";
            break;
        }
        
        if (!gameRunning || !wanderer.getIsAlive()) {
            break;
        }

        if (input == 'i' || input == 'I') {
            wanderer.displayStatus();
            turnCount--;
            continue;
        }
        
        if (input == 'm' || input == 'M') {
            wanderer.getHand().displayHand();
            std::cout << "Mana: " << wanderer.getMana() << "/" << wanderer.getMaxMana() << "\n";
            turnCount--;
            continue;
        }

        inputHandler->processInput(input);
        
        if (!wanderer.getIsAlive()) {
            std::cout << "Game Over! You died.\n";
            break;
        }
        
        if (!gameRunning) {
            break;
        }
        
        if (wanderer.getIsAlive() && turnCount % 3 == 0) {
            int oldShield = wanderer.getShield();
            wanderer.regenerateShield();
            wanderer.regenerateMana();

            if (wanderer.getShield() > oldShield) {
                std::cout << "Shield regenerated to " << wanderer.getShield() << "/" << wanderer.getMaxShield() << "!\n";
            }
        }

        if (wanderer.getIsAlive()) {
            beastManager->moveBeasts();
            buildingManager->updateBuildings();
            buildingManager->updateTowers(wandererPosition, wanderer);
            combatManager->checkCombat();
        }

        if (gameRunning && isLevelComplete()) {
            loadNextLevel();
        }
    }
    if (!wanderer.getIsAlive()) {
        std::cout << "Final Score: " << wanderer.getPoints() << "\n";
    }
}

GameController::~GameController() {
    if (inputHandler) inputHandler.reset();
    if (combatManager) combatManager.reset();
    if (buildingManager) buildingManager.reset();
    if (beastManager) beastManager.reset();
}