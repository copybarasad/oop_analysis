#include "Game.h"
#include "GameSaveSystem.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

Player& Game::getPlayer() { return mainPlayer; }
const Player& Game::getPlayer() const { return mainPlayer; }
const std::vector<Enemy>& Game::getEnemies() const { return enemyList; }
std::vector<Enemy>& Game::getEnemies() { return enemyList; }

Game::Game() : gameField(15, 15), mainPlayer(7, 7), isGameActive(true), currentTurn(0), lastSpellScore(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    setupGame();
}

void Game::setupGame() {
    createEnemies();
    initializeSpells();
    lastSpellScore = 0;
}

void Game::initializeSpells() {
    std::vector<std::unique_ptr<Spell>> startingSpells;
    startingSpells.push_back(std::unique_ptr<Spell>(new SingleTargetSpell("Dragon's Breath", 25, 3)));
    startingSpells.push_back(std::unique_ptr<Spell>(new AreaSpell("Arcane Pulse", 15, 2)));
    
    int randomIndex = std::rand() % startingSpells.size();
    mainPlayer.getSpellHand().addSpell(std::move(startingSpells[randomIndex]));
}

void Game::giveRandomSpellToPlayer() {
    if (mainPlayer.getSpellHand().isFull()) {
        return;
    }
    
    std::vector<std::unique_ptr<Spell>> availableSpells;
    availableSpells.push_back(std::unique_ptr<Spell>(new SingleTargetSpell("Lightning Strike", 30, 4)));
    availableSpells.push_back(std::unique_ptr<Spell>(new AreaSpell("Frost Vortex", 20, 3)));
    availableSpells.push_back(std::unique_ptr<Spell>(new SingleTargetSpell("Shadow Bolt", 35, 3)));
    availableSpells.push_back(std::unique_ptr<Spell>(new AreaSpell("Earthquake", 25, 2)));
    
    int randomIndex = std::rand() % availableSpells.size();
    mainPlayer.getSpellHand().addSpell(std::move(availableSpells[randomIndex]));
}

void Game::checkForNewSpell() {
    int currentScore = mainPlayer.getTotalScore();
    
    if (currentScore >= lastSpellScore + 15) {
        lastSpellScore = currentScore;
        giveRandomSpellToPlayer();
        std::cout << "New spell available!" << std::endl;
    }
}

bool Game::castSpell(Spell* spell, int targetX, int targetY) {
    if (SingleTargetSpell* stSpell = dynamic_cast<SingleTargetSpell*>(spell)) {
        return castSingleTargetSpell(stSpell, targetX, targetY);
    } else if (AreaSpell* aSpell = dynamic_cast<AreaSpell*>(spell)) {
        return castAreaSpell(aSpell, targetX, targetY);
    }
    return false;
}

bool Game::castSingleTargetSpell(SingleTargetSpell* spell, int targetX, int targetY) {
    if (!isValidSingleTarget(targetX, targetY, spell->getRange())) {
        return false;
    }
    
    for (auto& enemy : enemyList) {
        if (enemy.isAlive() && enemy.isOnPosition(targetX, targetY)) {
            std::cout << "Casting " << spell->getName() << "! -" << spell->getDamage() << " damage" << std::endl;
            enemy.receiveDamage(spell->getDamage());
            
            if (!enemy.isAlive()) {
                mainPlayer.addToScore(20);
                std::cout << "Enemy defeated! +20 points" << std::endl;
            }
            
            removeDeadEnemies();
            return true;
        }
    }
    
    std::cout << "No enemy at target position!" << std::endl;
    return false;
}

bool Game::castAreaSpell(AreaSpell* spell, int targetX, int targetY) {
    if (!isValidAreaTarget(targetX, targetY, spell->getRange())) {
        return false;
    }
    
    std::cout << "Casting " << spell->getName() << "! Area damage: " << spell->getDamage() << std::endl;
    
    int enemiesHit = 0;
    int centerX = mainPlayer.getCoordX();
    int centerY = mainPlayer.getCoordY();
    int areaSize = spell->getAreaSize();
    
    for (int dx = -areaSize/2; dx <= areaSize/2; ++dx) {
        for (int dy = -areaSize/2; dy <= areaSize/2; ++dy) {
            int currentX = centerX + dx;
            int currentY = centerY + dy;
            
            if (currentX == centerX && currentY == centerY) continue;
            
            for (auto& enemy : enemyList) {
                if (enemy.isAlive() && enemy.isOnPosition(currentX, currentY)) {
                    std::cout << "Hit enemy at (" << currentX << ", " << currentY << ")!" << std::endl;
                    enemy.receiveDamage(spell->getDamage());
                    enemiesHit++;
                    
                    if (!enemy.isAlive()) {
                        mainPlayer.addToScore(20);
                        std::cout << "Enemy defeated! +20 points" << std::endl;
                    }
                }
            }
        }
    }
    
    removeDeadEnemies();
    
    if (enemiesHit == 0) {
        std::cout << "Spell hit no enemies." << std::endl;
    }
    
    return true;
}

bool Game::isValidSingleTarget(int targetX, int targetY, int range) const {
    int playerX = mainPlayer.getCoordX();
    int playerY = mainPlayer.getCoordY();
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    
    if (distance > range) {
        std::cout << "Target out of range! Max range: " << range << std::endl;
        return false;
    }
    
    for (const auto& enemy : enemyList) {
        if (enemy.isAlive() && enemy.isOnPosition(targetX, targetY)) {
            return true;
        }
    }
    
    std::cout << "No enemy at target position!" << std::endl;
    return false;
}

bool Game::isValidAreaTarget(int targetX, int targetY, int range) const {
    int playerX = mainPlayer.getCoordX();
    int playerY = mainPlayer.getCoordY();
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    
    if (distance > range) {
        std::cout << "Target out of range! Max range: " << range << std::endl;
        return false;
    }
    
    return true;
}

void Game::handleSaveGame() {
    std::string filename;
    std::cout << "Enter save file name (without extension): ";
    std::cin >> filename;
    filename += ".save";
    
    try {
        if (GameSaveSystem::saveGame(*this, filename)) {
            std::cout << "Game saved successfully!" << std::endl;
        } else {
            std::cout << "Failed to save game" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Save error: " << e.what() << std::endl;
    }
}

bool Game::processSpellCast(int spellIndex, int targetX, int targetY) {
    if (mainPlayer.getSpellHand().getSpellCount() == 0) {
        std::cout << "You have no spells!" << std::endl;
        return false;
    }
    
    if (spellIndex < 0 || spellIndex >= mainPlayer.getSpellHand().getSpellCount()) {
        std::cout << "Invalid spell choice!" << std::endl;
        return false;
    }

    Spell* selectedSpell = mainPlayer.getSpellHand().getSpells()[spellIndex].get();
    
    if (selectedSpell->getType() == SpellType::AREA_EFFECT) {
        targetX = mainPlayer.getCoordX();
        targetY = mainPlayer.getCoordY();
    }
    
    return mainPlayer.getSpellHand().useSpell(spellIndex, *this, targetX, targetY);
}

// Замена handlePlayerTurn
bool Game::executeCommand(const Command& command) {
    if (command.type == CommandType::QUIT) {
        isGameActive = false;
        return true;
    }
    
    if (command.type == CommandType::INVALID) {
        std::cout << "Invalid command!" << std::endl;
        return false;
    }

    if (command.type == CommandType::SWITCH_STYLE) {
        mainPlayer.toggleCombatStyle();
        return true;
    }

    if (command.type == CommandType::SAVE_GAME) {
        handleSaveGame();
        return false;
    }

    if (command.type == CommandType::CAST_SPELL) {
        return processSpellCast(command.value, command.targetX, command.targetY);
    }

    if (command.type == CommandType::ATTACK_RANGED) {
        if (mainPlayer.getCurrentCombatStyle() != RANGED_STYLE) {
            std::cout << "Switch to ranged mode first! Press 'M'" << std::endl;
            return false;
        }
        applyRangedAttack(command.targetX, command.targetY);
        removeDeadEnemies();
        return true;
    }

    if (command.type == CommandType::MOVE) {
        int newX = mainPlayer.getCoordX() + command.targetX; // dx
        int newY = mainPlayer.getCoordY() + command.targetY; // dy

        // Проверка атаки ближнего боя
        for (auto& enemy : enemyList) {
            if (enemy.isAlive() && enemy.isOnPosition(newX, newY)) {
                applyMeleeAttack(mainPlayer, enemy);
                removeDeadEnemies();
                return true;
            }
        }

        // Проверка перемещения
        if (gameField.isPositionWithinBounds(newX, newY) && gameField.isCellPassable(newX, newY)) {
            mainPlayer.changePosition(newX, newY);
            return true;
        } else {
            std::cout << "Cannot move there!" << std::endl;
            return false;
        }
    }

    return false;
}

void Game::showGameOverStats() const {
    std::cout << "\n=== GAME OVER ===" << std::endl;
    if (hasPlayerWon()) {
        std::cout << "VICTORY! You survived!" << std::endl;
    } else {
        std::cout << "DEFEAT! You were defeated." << std::endl;
    }
    std::cout << "Final score: " << mainPlayer.getTotalScore() << std::endl;
    std::cout << "Total turns: " << currentTurn << std::endl;
}

void Game::applyMeleeAttack(Player& attacker, Enemy& target) {
    int damage = attacker.getAttackDamage();
    std::cout << "Melee attack! -" << damage << " damage" << std::endl;
    
    target.receiveDamage(damage);
    
    if (!target.isAlive()) {
        attacker.addToScore(20);
        std::cout << "Enemy defeated! +20 points" << std::endl;
    }
}

void Game::applyRangedAttack(int targetX, int targetY) {
    if (!validateRangedAttack(targetX, targetY)) {
        return;
    }
    
    int damage = mainPlayer.getAttackDamage();
    bool enemyHit = false;
    
    for (auto& enemy : enemyList) {
        if (enemy.isAlive() && enemy.isOnPosition(targetX, targetY)) {
            std::cout << "Ranged attack hit! -" << damage << " damage" << std::endl;
            enemy.receiveDamage(damage);
            enemyHit = true;
            
            if (!enemy.isAlive()) {
                mainPlayer.addToScore(20);
                std::cout << "Enemy defeated! +20 points" << std::endl;
            }
            break;
        }
    }
    
    if (!enemyHit) {
        std::cout << "Ranged attack missed! No enemy at target position." << std::endl;
    }
}

bool Game::validateRangedAttack(int targetX, int targetY) {
    if (mainPlayer.getCurrentCombatStyle() != RANGED_STYLE) {
        return false;
    }
    
    if (!mainPlayer.isWithinRangedDistance(targetX, targetY)) {
        std::cout << "Target out of range! Max range: 3 cells" << std::endl;
        return false;
    }
    
    int deltaX = targetX - mainPlayer.getCoordX();
    int deltaY = targetY - mainPlayer.getCoordY();
    if (deltaX != 0 && deltaY != 0) {
        std::cout << "Ranged attack must be horizontal or vertical only!" << std::endl;
        return false;
    }
    
    int currentX = mainPlayer.getCoordX();
    int currentY = mainPlayer.getCoordY();
    
    while (currentX != targetX || currentY != targetY) {
        if (currentX < targetX) currentX++;
        else if (currentX > targetX) currentX--;
        if (currentY < targetY) currentY++;
        else if (currentY > targetY) currentY--;
        
        if ((currentX != targetX || currentY != targetY) && 
            !gameField.isCellPassable(currentX, currentY)) {
            std::cout << "Obstacle in the way!" << std::endl;
            return false;
        }
    }
    
    return true;
}

void Game::applyEnemyAttack(Enemy& attacker, Player& target) {
    int damage = attacker.getAttackDamage();
    std::cout << "Enemy attacked you! -" << damage << " HP" << std::endl;
    target.receiveDamage(damage);
}

void Game::removeDeadEnemies() {
    for (auto it = enemyList.begin(); it != enemyList.end(); ) {
        if (!it->isAlive()) {
            it = enemyList.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::handleEnemyTurns() {
    for (auto& enemy : enemyList) {
        if (enemy.isAlive()) {
            int nextX, nextY;
            enemy.determineNextMove(mainPlayer, gameField, nextX, nextY);
            
            if (mainPlayer.isOnPosition(nextX, nextY)) {
                applyEnemyAttack(enemy, mainPlayer);
                continue;
            }
            
            bool positionOccupied = false;
            for (const auto& otherEnemy : enemyList) {
                if (&otherEnemy != &enemy && otherEnemy.isAlive() && 
                    otherEnemy.isOnPosition(nextX, nextY)) {
                    positionOccupied = true;
                    break;
                }
            }
            
            if (positionOccupied) {
                continue;
            }
            
            if (gameField.isPositionWithinBounds(nextX, nextY) && gameField.isCellPassable(nextX, nextY)) {
                enemy.changePosition(nextX, nextY);
            }
        }
    }
    removeDeadEnemies();
}

void Game::checkGameCompletion() {
    if (hasPlayerWon() || hasPlayerLost()) {
        isGameActive = false;
    }
}

bool Game::hasPlayerWon() const {
    for (const auto& enemy : enemyList) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return mainPlayer.isAlive();
}

bool Game::hasPlayerLost() const {
    return !mainPlayer.isAlive();
}

void Game::createEnemies() {
    enemyList.clear();
    
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distX(1, gameField.getFieldWidth() - 2);
    std::uniform_int_distribution<int> distY(1, gameField.getFieldHeight() - 2);
    
    for (int i = 0; i < 3; ++i) {
        int spawnX, spawnY;
        bool validSpawn = false;
        int spawnAttempts = 0;
        
        while (!validSpawn && spawnAttempts < 50) {
            spawnX = distX(generator);
            spawnY = distY(generator);
            
            int playerX = mainPlayer.getCoordX();
            int playerY = mainPlayer.getCoordY();
            int distance = std::abs(playerX - spawnX) + std::abs(playerY - spawnY);
            
            if (distance >= 3 && gameField.isCellPassable(spawnX, spawnY)) {
                validSpawn = true;
            }
            spawnAttempts++;
        }
        
        if (validSpawn) {
            enemyList.emplace_back(spawnX, spawnY);
        }
    }
}

void Game::restartGame() {
    enemyList.clear();
    mainPlayer = Player(7, 7);
    isGameActive = true;
    currentTurn = 0;
    setupGame();
}