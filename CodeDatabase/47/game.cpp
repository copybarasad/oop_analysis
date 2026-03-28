#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game()
    : gameState(nullptr),
      runningFlag(false),
      extraEnemyTurns(0)
{
    std::srand(std::time(0));
}

bool Game::applyCommand(const Command& cmd) {
    if (!gameState) return false;
    Player& player = gameState->getPlayer();

    bool action = false;
    bool endTurn = false;

    switch (cmd.type) {
        case CommandType::MoveUp:    action = tryMovePlayer(0,-1); endTurn = action; break;
        case CommandType::MoveDown:  action = tryMovePlayer(0,1);  endTurn = action; break;
        case CommandType::MoveLeft:  action = tryMovePlayer(-1,0); endTurn = action; break;
        case CommandType::MoveRight: action = tryMovePlayer(1,0);  endTurn = action; break;

        case CommandType::Attack:
            processPlayerAttack();
            action = true;
            endTurn = true;
            break;

        case CommandType::SwitchCombatMode:
            player.switchCombatMode();
            std::cout << "Switched mode\n";
            action = true;
            endTurn = true;
            break;

        case CommandType::CastSpell1:
        case CommandType::CastSpell2:
        case CommandType::CastSpell3: {
            int idx = (cmd.type == CommandType::CastSpell1) ? 0 :
                      (cmd.type == CommandType::CastSpell2) ? 1 : 2;

            if (processSpellCastAtIndex(idx, cmd.paramX, cmd.paramY))
                endTurn = true;
            break;
        }
        case CommandType::SaveGame:
            saveGame();
            return false;

        case CommandType::LoadGame:
            loadGame();
            return false;

        case CommandType::Pause:
            return false;

        case CommandType::Quit:
            runningFlag = false;
            return false;

        default:
            std::cout << "Invalid command\n";
            break;
    }

    if (action)
        player.getSpellHand().restoreMana(1);

    return endTurn;
}

void Game::enemyTurn() {
    if (!gameState) return;
    processEnemyTurn();
}

void Game::performPostEnemyChecks() {
    checkGameState();
    checkTraps();
}

bool Game::isGameOver() const {
    return gameState ? gameState->isGameOver() : true;
}

bool Game::isLevelCompleted() const {
    return gameState ? gameState->isLevelCompleted() : false;
}

int Game::getCurrentLevel() const {
    return gameState ? gameState->getCurrentLevel() : 0;
}

Player& Game::getPlayer() {
    return gameState->getPlayer();
}

GameState& Game::getGameState() {
    return *gameState;
}

int Game::getTotalScore() const {
    return gameState ? gameState->getTotalScore() : 0;
}

int Game::getPlayerPositionX() const {
    return gameState->getPlayer().getPositionX();
}

int Game::getPlayerPositionY() const {
    return gameState->getPlayer().getPositionY();
}

void Game::startNewGame() {
    resetGame();
    gameState = std::make_unique<GameState>(LevelManager::calculateLevelWidth(1),LevelManager::calculateLevelHeight(1),1);

    LevelManager::setupLevel(*gameState, 1);

    runningFlag = true;
}


void Game::initializeLevel(int level) {
    int width = LevelManager::calculateLevelWidth(level);
    int height = LevelManager::calculateLevelHeight(level);
    
    if (gameState) {
        gameState->resetForNewLevel(width, height);
        gameState->setCurrentLevel(level);
    } 
    else {
        gameState = std::make_unique<GameState>(width, height, level);
    }

    LevelManager::setupLevel(*gameState, level);
}

void Game::handleLevelCompletion() {
    int nextLevel = gameState->getCurrentLevel() + 1;
    std::cout << "\n=== LEVEL " << gameState->getCurrentLevel() << " COMPLETED! ===" << std::endl;
    std::cout << "Preparing for level " << nextLevel << "..." << std::endl;
    
    UpgradeSystem::removeRandomSpells(gameState->getPlayer());
    
    UpgradeSystem::showUpgradeMenu(gameState->getPlayer());
    
    gameState->getPlayer().restoreFullHealth();
    gameState->getPlayer().getSpellHand().restoreMana(
        gameState->getPlayer().getSpellHand().getMaxMana() - 
        gameState->getPlayer().getSpellHand().getCurrentMana()
    );
    
    std::cout << "\n=== SUMMARY ===" << std::endl;
    std::cout << "Health: " << gameState->getPlayer().getHealth() << "/" 
              << gameState->getPlayer().getMaxHealth() << std::endl;
    std::cout << "Mana: " << gameState->getPlayer().getSpellHand().getCurrentMana() << "/" 
              << gameState->getPlayer().getSpellHand().getMaxMana() << std::endl;
    std::cout << "Score: " << gameState->getTotalScore() << std::endl;
    std::cout << "Proceeding to level " << nextLevel << "..." << std::endl;
    
    initializeLevel(nextLevel);
}

int Game::getValidInput(int minChoice, int maxChoice) {
        int choice;
        bool validChoice = false;
        
        while (!validChoice) {
            std::cin >> choice;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input! Please enter a number between " << minChoice << " and " << maxChoice << "." << std::endl;
                continue;
            }
            
            std::cin.ignore(10000, '\n');
            
            if (choice >= minChoice && choice <= maxChoice) {
                validChoice = true;
            }
            else {
                std::cout << "Invalid choice! Please enter a number between " << minChoice << " and " << maxChoice << "." << std::endl;
            }
        }
        return choice;
}

bool Game::loadGame() {
    try {
        if (!SaveManager::saveExists()) {
            std::cout << "No save game found!" << std::endl;
            return false;
        }
        
        gameState = SaveManager::loadGame();
        std::cout << "Game loaded successfully! Starting level " << gameState->getCurrentLevel() << std::endl;
        return true;
        
    } catch (const GameException& e) {
        std::cout << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

bool Game::loadGameFromMenu() {
    std::cout << "Warning: Loading a game will lose all unsaved progress." << std::endl;
    std::cout << "Are you sure? (1 = Yes, 2 = No): ";
    
    int confirm = getValidInput(1, 2);
    
    if (confirm != 1) {
        std::cout << "Load cancelled." << std::endl;
        return false;
    }
    
    try {
        if (!SaveManager::saveExists()) {
            std::cout << "No save game found!" << std::endl;
            return false;
        }
        
        auto loadedState = SaveManager::loadGame();
        
        if (loadedState) {
            gameState = std::move(loadedState);
            runningFlag = true;
            extraEnemyTurns = 0;
            
            std::cout << "Game loaded successfully!" << std::endl;
            return true;
        }
        
        return false;
        
    } catch (const std::exception& e) {
        std::cout << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

void Game::saveGame() {
    try {
        if (gameState) {
            SaveManager::saveGame(*gameState);
            std::cout << "Game saved successfully!" << std::endl;
        }
    }
    catch (const SaveException& e) {
        std::cout << "Error saving game: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
    }
}

bool Game::tryMovePlayer(int deltaX, int deltaY) {
    Player& player = gameState->getPlayer();
    GameField& field = gameState->getField();
    
    int newX = player.getPositionX() + deltaX;
    int newY = player.getPositionY() + deltaY;
    
    if (!field.isValidPosition(newX, newY)) {
        std::cout << "You cannot move beyond the field boundaries!" << std::endl;
        return false;
    }
    
    if (!field.isPassable(newX, newY)) {
        std::cout << "You cannot move through impassable terrain!" << std::endl;
        return false;
    }
    
    for (const auto& enemy : gameState->getEnemies()) {
        if (enemy.isAlive() && enemy.getPositionX() == newX && enemy.getPositionY() == newY) {
            std::cout << "There is an enemy blocking your path! Fight him!" << std::endl;
            return false;
        }
    }
    
    if ((gameState->getEnemyBuilding().isAlive() && gameState->getEnemyBuilding().getPositionX() == newX && gameState->getEnemyBuilding().getPositionY() == newY) ||(gameState->getEnemyTower().isAlive() && gameState->getEnemyTower().getPositionX() == newX && gameState->getEnemyTower().getPositionY() == newY)) {
        std::cout << "There is a building blocking your path! Damage it!" << std::endl;
        return false;
    }
    
    player.move(deltaX, deltaY);
    std::cout << "Moved to (" << newX << ", " << newY << ")" << std::endl;
    
    if (field.getCellType(newX, newY) == CellType::SLOWING) {
        std::cout << "Slowing cell! You are skipping turn!" << std::endl;
        extraEnemyTurns = 1;
        return true;
    }
    
    return true;
}

void Game::processPlayerAttack() {
    Player& player = gameState->getPlayer();
    bool hitSomething = false;
    
    for (auto& enemy : gameState->getEnemies()) {
        if (enemy.isAlive() && player.canAttack(enemy.getPositionX(), enemy.getPositionY())) {
            enemy.takeDamage(player.getDamage());
            std::cout << "You attacked an enemy for " << player.getDamage() << " damage!" << std::endl;
            hitSomething = true;
            
            if (!enemy.isAlive()) {
                gameState->setTotalScore(gameState->getTotalScore() + 100);
                std::cout << "Enemy defeated! +100 points" << std::endl;
            }
        }
    }
    
    EnemyBuilding& building = gameState->getEnemyBuilding();
    if (building.isAlive() && player.canAttack(building.getPositionX(), building.getPositionY())) {
        building.takeDamage(player.getDamage());
        std::cout << "You attacked the enemy building for " << player.getDamage() << " damage!" << std::endl;
        hitSomething = true;
        
        if (!building.isAlive()) {
            gameState->setTotalScore(gameState->getTotalScore() + 300);
            std::cout << "Enemy building destroyed! +300 points" << std::endl;
        }
    }
    
    EnemyTower& tower = gameState->getEnemyTower();
    if (tower.isAlive() && player.canAttack(tower.getPositionX(), tower.getPositionY())) {
        tower.takeDamage(player.getDamage());
        std::cout << "You attacked the enemy tower for " << player.getDamage() << " damage!" << std::endl;
        hitSomething = true;
        
        if (!tower.isAlive()) {
            gameState->setTotalScore(gameState->getTotalScore() + 200);
            std::cout << "Enemy tower destroyed! +200 points" << std::endl;
        }
    }
    
    if (!hitSomething) {
        std::cout << "No targets in attack range!" << std::endl;
    }
}

bool Game::processSpellCast(char spellKey) {
    int spellIndex = spellKey - '1';
    Player& player = gameState->getPlayer();
    
    if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
        std::cout << "Invalid spell slot!" << std::endl;
        return false;
    }
    
    Spell* spell = player.getSpellHand().getSpells()[spellIndex].get();
    std::cout << "Preparing to cast: " << spell->getName() << std::endl;
    std::cout << "Mana cost: " << spell->getManaCost() << ", Range: " << spell->getRange() << std::endl;
    
    if (!player.getSpellHand().canCastSpell(spellIndex)) {
        std::cout << "Not enough mana! You need " << spell->getManaCost() << " mana, but you have " << player.getSpellHand().getCurrentMana() << std::endl;
        return false;
    }
    
    int targetX, targetY;
    std::cout << "Enter target coordinates (X Y): ";
    std::cin >> targetX >> targetY;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid coordinates!" << std::endl;
        return false;
    }
    
    return player.getSpellHand().castSpell(spellIndex, *this, targetX, targetY);
}

bool Game::damageEnemyAtPosition(int x, int y, int damage) {
    for (auto& enemy : gameState->getEnemies()) {
        if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) {
            enemy.takeDamage(damage);
            std::cout << "Enemy took " << damage << " damage from spell!" << std::endl;
            
            if (!enemy.isAlive()) {
                gameState->setTotalScore(gameState->getTotalScore() + 100);
            }
            return true;
        }
    }
    return false;
}

bool Game::damageBuildingAtPosition(int x, int y, int damage) {
    bool hit = false;
    
    EnemyBuilding& building = gameState->getEnemyBuilding();
    if (building.isAlive() && building.getPositionX() == x && building.getPositionY() == y) {
        building.takeDamage(damage);
        std::cout << "Building took " << damage << " damage from spell!" << std::endl;
        hit = true;
        
        if (!building.isAlive()) {
            gameState->setTotalScore(gameState->getTotalScore() + 300);
        }
    }
    
    EnemyTower& tower = gameState->getEnemyTower();
    if (tower.isAlive() && tower.getPositionX() == x && tower.getPositionY() == y) {
        tower.takeDamage(damage);
        std::cout << "Tower took " << damage << " damage from spell!" << std::endl;
        hit = true;
        
        if (!tower.isAlive()) {
            gameState->setTotalScore(gameState->getTotalScore() + 200);
        }
    }
    
    return hit;
}

void Game::tryAddNewSpell() {
    if (gameState->getEnemiesDefeated() >= 3 && !gameState->getPlayer().getSpellHand().isFull()) {
        gameState->getPlayer().getSpellHand().addRandomSpell();
        gameState->setEnemiesDefeated(0);
    }
}

bool Game::placeTrap(int x, int y, int damage) {
    if (gameState->getField().isValidPosition(x, y) && gameState->getField().isPassable(x, y) && !isPositionOccupied(x, y)) {
        gameState->getTraps().push_back(Trap(x, y, damage));
        return true;
    }
    return false;
}

void Game::processEnemyTurn() {
    std::cout << "\n--- Enemy Turn ---" << std::endl;
    
    Player& player = gameState->getPlayer();
    GameField& field = gameState->getField();
    
    for (auto& enemy : gameState->getEnemies()) {
        if (enemy.isAlive()) {
            bool attacked = enemy.moveTowardsPlayer(player.getPositionX(), player.getPositionY(), field, gameState->getEnemies(),gameState->getEnemyBuilding(),gameState->getEnemyTower());
            
            if (attacked) {
                player.takeDamage(enemy.getDamage());
                std::cout << "An enemy attacked you for " << enemy.getDamage() << " damage!" << std::endl;
            }
        }
    }
    
    gameState->getEnemyBuilding().update();
    if (gameState->getEnemyBuilding().canSpawnEnemy()) {
        spawnEnemyFromBuilding();
        gameState->getEnemyBuilding().resetCooldown();
    }
    
    gameState->getEnemyTower().update();
    gameState->getEnemyTower().performAttack(*this);
    
    checkTraps();
}

void Game::checkGameState() {
    auto& enemies = gameState->getEnemies();
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->isAlive()) {
            gameState->setEnemiesDefeated(gameState->getEnemiesDefeated() + 1);
            tryAddNewSpell();
            gameState->getPlayer().heal();
            std::cout << "Enemy defeated! You gained 1 health." << std::endl;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::checkTraps() {
    auto& traps = gameState->getTraps();
    auto& enemies = gameState->getEnemies();
    
    for (auto& trap : traps) {
        if (!trap.isActive()) continue;
        
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && enemy.getPositionX() == trap.getPositionX() && enemy.getPositionY() == trap.getPositionY()) {
                enemy.takeDamage(trap.getDamage());
                trap.trigger();
                std::cout << "Trap triggered! Enemy took " << trap.getDamage() << " damage!" << std::endl;
                break;
            }
        }
    }
}

void Game::spawnEnemyFromBuilding() {
    EnemyBuilding& building = gameState->getEnemyBuilding();
    int buildingX = building.getPositionX();
    int buildingY = building.getPositionY();
    
    int directions[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    
    for (int i = 0; i < 8; ++i) {
        int newX = buildingX + directions[i][0];
        int newY = buildingY + directions[i][1];
        
        if (gameState->getField().isValidPosition(newX, newY) && gameState->getField().isPassable(newX, newY) && !isPositionOccupied(newX, newY)) {
            Enemy newEnemy(newX, newY);
            newEnemy.setHealth(LevelManager::calculateEnemyHealth(gameState->getCurrentLevel()));
            gameState->getEnemies().push_back(newEnemy);
            std::cout << "Enemy building spawned a new enemy!" << std::endl;
            return;
        }
    }
    
    std::cout << "Enemy building could not find space to spawn!" << std::endl;
}

void Game::damagePlayer(int damage) {
    gameState->getPlayer().takeDamage(damage);
}

void Game::drawField() {
    GameField& field = gameState->getField();
    Player& player = gameState->getPlayer();
    
    std::cout << "\n";
    
    std::cout << "    ";
    for (int x = 0; x < field.getWidth(); ++x) {
        if (x < 10) std::cout << " " << x << " ";
        else std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << "   +";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "---";
    }
    std::cout << "+" << std::endl;
    
    for (int y = 0; y < field.getHeight(); ++y) {
        if (y < 10) std::cout << " " << y << " |";
        else std::cout << y << " |";
        
        for (int x = 0; x < field.getWidth(); ++x) {
            if (x == player.getPositionX() && y == player.getPositionY()) {
                std::cout << " P ";
            }
            else {
                bool drawn = false;
                
                for (const auto& enemy : gameState->getEnemies()) {
                    if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) {
                        std::cout << " E ";
                        drawn = true;
                        break;
                    }
                }
                
                if (!drawn) {
                    for (const auto& trap : gameState->getTraps()) {
                        if (trap.isActive() && trap.getPositionX() == x && trap.getPositionY() == y) {
                            std::cout << " T ";
                            drawn = true;
                            break;
                        }
                    }
                }
                
                if (!drawn) {
                    if (gameState->getEnemyBuilding().isAlive() && gameState->getEnemyBuilding().getPositionX() == x && gameState->getEnemyBuilding().getPositionY() == y) {
                        std::cout << " B ";
                    }
                    else if (gameState->getEnemyTower().isAlive() && gameState->getEnemyTower().getPositionX() == x && gameState->getEnemyTower().getPositionY() == y) {
                        std::cout << " W ";
                    }
                    else {
                        switch (field.getCellType(x, y)) {
                            case CellType::EMPTY: std::cout << " . "; break;
                            case CellType::IMPASSABLE: std::cout << " # "; break;
                            case CellType::SLOWING: std::cout << " ~ "; break;
                        }
                    }
                }
            }
        }
        std::cout << "|" << std::endl;
    }
    
    std::cout << "   +";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "---";
    }
    std::cout << "+" << std::endl;
    
    std::cout << "Legend: P=Player, E=Enemy, B=Building, W=Tower, T=Trap, #=Wall, ~=Slowing\n";
}



bool Game::processSpellCastAtIndex(int spellIndex, int targetX, int targetY) {
    Player& player = gameState->getPlayer();

    if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
        std::cout << "Invalid spell slot!" << std::endl;
        return false;
    }

    Spell* spell = player.getSpellHand().getSpells()[spellIndex].get();
    std::cout << "Preparing to cast: " << spell->getName() << std::endl;
    std::cout << "Mana cost: " << spell->getManaCost() << ", Range: " << spell->getRange() << std::endl;

    if (!player.getSpellHand().canCastSpell(spellIndex)) {
        std::cout << "Not enough mana! You need " << spell->getManaCost() << " mana, but you have " << player.getSpellHand().getCurrentMana() << std::endl;
        return false;
    }

    if (targetX == 0 && targetY == 0) {
        std::cout << "Enter target coordinates (X Y): ";
        std::cin >> targetX >> targetY;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid coordinates!" << std::endl;
            return false;
        }
    }

    return player.getSpellHand().castSpell(spellIndex, *this, targetX, targetY);
}

bool Game::isPositionValid(int x, int y) const {
    return gameState && gameState->getField().isValidPosition(x, y);
}

bool Game::isPositionPassable(int x, int y) const {
    return gameState && gameState->getField().isPassable(x, y);
}

bool Game::hasEnemyOrBuildingAt(int x, int y) const {
    if (!gameState) return false;
    for (const auto& enemy : gameState->getEnemies()) {
        if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) return true;
    }
    if ((gameState->getEnemyBuilding().isAlive() && gameState->getEnemyBuilding().getPositionX() == x && gameState->getEnemyBuilding().getPositionY() == y) ||
        (gameState->getEnemyTower().isAlive() && gameState->getEnemyTower().getPositionX() == x && gameState->getEnemyTower().getPositionY() == y)) {
        return true;
    }
    return false;
}


bool Game::isPositionOccupied(int x, int y) const {
    if (!gameState) return false;
    const Player& player = gameState->getPlayer();
    if (player.getPositionX() == x && player.getPositionY() == y) return true;
    for (const auto& enemy : gameState->getEnemies()) {
        if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) return true;
    }
    if ((gameState->getEnemyBuilding().isAlive() && gameState->getEnemyBuilding().getPositionX() == x && gameState->getEnemyBuilding().getPositionY() == y) ||
        (gameState->getEnemyTower().isAlive() && gameState->getEnemyTower().getPositionX() == x && gameState->getEnemyTower().getPositionY() == y)) {
        return true;
    }
    return false;
}

void Game::resetGame() {
    gameState = nullptr;
    runningFlag = false;
    extraEnemyTurns = 0;
}

