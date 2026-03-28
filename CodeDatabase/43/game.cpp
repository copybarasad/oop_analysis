#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>

Game::Game() : gameRunning(false), playerX(0), playerY(0) {
    std::srand(std::time(0));
}

void Game::findPlayerPosition() {
    for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
        for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
            auto entity = field->getEntityAt(x, y);
            if (entity && entity->getType() == "Player") {
                playerX = x;
                playerY = y;
                return;
            }
        }
    }
}

void Game::initializeGame() {
    std::cout << "DEBUG: Starting initializeGame()" << std::endl;
    
    levelManager.setCurrentLevel(1);
    auto config = levelManager.getCurrentLevelConfig();
    
    std::cout << "DEBUG: Creating game field" << std::endl;
    field = std::make_unique<GameField>(15, 15);
    
    std::cout << "DEBUG: Creating player" << std::endl;
    player = std::make_shared<Player>(100, 10);
    
    std::cout << "DEBUG: Placing player on field" << std::endl;
    auto playerPos = field->getRandomEmptyCell();
    field->addEntity(player, playerPos.first, playerPos.second);
    playerX = playerPos.first;
    playerY = playerPos.second;
    
    std::cout << "DEBUG: Spawning enemies" << std::endl;
    spawnEnemiesForCurrentLevel();
    
    std::cout << "DEBUG: Setting up spells" << std::endl;
    player->getSpellHand().clear();
    addSpellToPlayer(std::make_shared<DirectDamageSpell>(15, 3));

    gameRunning = true;
    
    std::cout << "DEBUG: Game initialized successfully" << std::endl;
    std::cout << "Game initialized! Level " << config.levelNumber << std::endl;
    std::cout << "Enemies: " << config.enemyCount << " (HP: " << config.enemyHealth << ", Damage: " << config.enemyDamage << ")" << std::endl;
}

void Game::spawnInitialEntities() {
    auto playerPos = field->getRandomEmptyCell();
    player = std::shared_ptr<Player>(new Player(100, 10));
    field->addEntity(player, playerPos.first, playerPos.second);
    playerX = playerPos.first;
    playerY = playerPos.second;

    for (int i = 0; i < 3; ++i) {
        auto enemyPos = field->getRandomEmptyCell();
        while (abs(playerX - enemyPos.first) < 3 && abs(playerY - enemyPos.second) < 3) {
            enemyPos = field->getRandomEmptyCell();
        }

        auto enemy = std::shared_ptr<Enemy>(new Enemy(50, 8));
        enemies.push_back(enemy);
        field->addEntity(enemy, enemyPos.first, enemyPos.second);
    }
}

void Game::giveNewSpellOnFirstKill() {
    static bool firstEnemyKilled = false;
    if (!firstEnemyKilled) {
        firstEnemyKilled = true;
        auto sunstrike = std::shared_ptr<AreaDamageSpell>(new AreaDamageSpell(10, 4));
        if (player->getSpellHand().addSpell(sunstrike)) {
            std::cout << "⭐ New spell unlocked: " << sunstrike->getName() << "! Press [2] to use it." << std::endl;
        }
    }
}

void Game::movePlayer(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;

    auto targetEntity = field->getEntityAt(newX, newY);
    if (targetEntity && targetEntity->getType() == "Enemy") {
        if (player->getCombatMode()) {
            targetEntity->takeDamage(player->getDamage());
            std::cout << "Melee attack! Enemy took " << player->getDamage() << " damage!" << std::endl;

            if (!targetEntity->isAlive()) {
                field->removeEntity(newX, newY);
                player->addScore(10);
                std::cout << "Enemy defeated! +10 score." << std::endl;
                
                giveNewSpellOnFirstKill();
                
                for (auto it = enemies.begin(); it != enemies.end(); ) {
                    if (it->get() == targetEntity.get()) {
                        it = enemies.erase(it);
                        break;
                    } else {
                        ++it;
                    }
                }
            }
        } else {
            std::cout << "Switch to melee mode to attack in close combat!" << std::endl;
        }
    } else {
        if (field->moveEntity(playerX, playerY, newX, newY)) {
            playerX = newX;
            playerY = newY;
        }
    }
}

void Game::switchCombatMode() {
    player->switchCombatMode();
}

void Game::rangedAttack() {
    if (player->getCombatMode()) {
        std::cout << "Switch to ranged mode to shoot!" << std::endl;
        return;
    }

    bool hit = false;

    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        int enemyX = -1, enemyY = -1;
        for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
            for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
                auto entity = field->getEntityAt(x, y);
                if (entity.get() == enemy.get()) {
                    enemyX = x;
                    enemyY = y;
                    break;
                }
            }
            if (enemyX != -1) break;
        }

        if (enemyX == -1) continue;

        int distanceX = abs(playerX - enemyX);
        int distanceY = abs(playerY - enemyY);

        if ((playerX == enemyX && distanceY <= 5) ||
            (playerY == enemyY && distanceX <= 5)) {

            if (field->hasClearLineOfSight(playerX, playerY, enemyX, enemyY)) {
                enemy->takeDamage(player->getDamage());
                std::cout << "Ranged attack hit! Enemy took " << player->getDamage() << " damage!" << std::endl;
                hit = true;

                if (!enemy->isAlive()) {
                    field->removeEntity(enemyX, enemyY);
                    player->addScore(10);
                    std::cout << "Enemy defeated! +10 score." << std::endl;
                    
                    giveNewSpellOnFirstKill();
                    
                    for (auto it = enemies.begin(); it != enemies.end(); ) {
                        if (it->get() == enemy.get()) {
                            it = enemies.erase(it);
                            break;
                        } else {
                            ++it;
                        }
                    }
                }
                break;
            }
        }
    }

    if (!hit) {
        std::cout << "Ranged attack missed! No valid target in range." << std::endl;
    }
}

void Game::castSpell(int index) {
    handleSpellCastOnNearestEnemy(index);
}

void Game::handleSpellCastOnNearestEnemy(int spellIndex) {
    auto spell = player->getSpellHand().getSpell(spellIndex);
    if (!spell) {
        std::cout << "No spell in slot " << (spellIndex + 1) << "!" << std::endl;
        return;
    }
    
    std::shared_ptr<Entity> nearestEnemy = nullptr;
    int minDistance = 1000;
    int targetX = -1, targetY = -1;
    
    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        int enemyX = -1, enemyY = -1;
        for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
            for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
                auto entity = field->getEntityAt(x, y);
                if (entity.get() == enemy.get()) {
                    enemyX = x;
                    enemyY = y;
                    break;
                }
            }
            if (enemyX != -1) break;
        }
        
        if (enemyX == -1) continue;
        
        int distance = abs(playerX - enemyX) + abs(playerY - enemyY);
        if (distance < minDistance) {
            minDistance = distance;
            nearestEnemy = enemy;
            targetX = enemyX;
            targetY = enemyY;
        }
    }
    
    if (nearestEnemy && spell->canCast(playerX, playerY, targetX, targetY)) {
        if (player->castSpell(spellIndex, *this, targetX, targetY)) {
            std::cout << spell->getName() << " cast on enemy at (" << targetX << "," << targetY << ")!" << std::endl;
            removeDeadEnemies();
        }
    } else {
        std::cout << "No valid target in range for " << spell->getName() << "!" << std::endl;
    }
}

void Game::save() {
    saveGame("savegame.dat");
}

void Game::load() {
    loadGame("savegame.dat");
}

void Game::restart() {
    restartGame();
}

void Game::quit() {
    gameRunning = false;
}

void Game::displayMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void Game::removeDeadEnemies() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->isAlive()) {
            for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
                for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
                    auto entity = field->getEntityAt(x, y);
                    if (entity && entity.get() == it->get()) {
                        field->removeEntity(x, y);
                        break;
                    }
                }
            }
            
            player->addScore(10);
            std::cout << "Enemy defeated! +10 score." << std::endl;
            
            // ← ТОЛЬКО ЭТА СТРОКА вместо 11 строк дублирования
            giveNewSpellOnFirstKill();
            
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}
void Game::handleEnemyTurns() {
    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        int enemyX = -1, enemyY = -1;
        for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
            for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
                auto entity = field->getEntityAt(x, y);
                if (entity.get() == enemy.get()) {
                    enemyX = x;
                    enemyY = y;
                    break;
                }
            }
            if (enemyX != -1) break;
        }

        if (enemyX == -1) continue;

        bool isAdjacent = (abs(playerX - enemyX) == 1 && playerY == enemyY) || 
                         (abs(playerY - enemyY) == 1 && playerX == enemyX);

        if (isAdjacent) {
            player->takeDamage(enemy->getDamage());
            std::cout << "Enemy hit you for " << enemy->getDamage() << " damage!" << std::endl;
        } else {
            int distX = abs(playerX - enemyX);
            int distY = abs(playerY - enemyY);
            
            int dx = (playerX > enemyX) ? 1 : (playerX < enemyX) ? -1 : 0;
            int dy = (playerY > enemyY) ? 1 : (playerY < enemyY) ? -1 : 0;

            if (distX >= distY) {
                if (dx != 0 && field->moveEntity(enemyX, enemyY, enemyX + dx, enemyY)) {
                } else if (dy != 0 && field->moveEntity(enemyX, enemyY, enemyX, enemyY + dy)) {
                } else {
                    tryAlternativeDirections(enemyX, enemyY);
                }
            } else {
                if (dy != 0 && field->moveEntity(enemyX, enemyY, enemyX, enemyY + dy)) {
                } else if (dx != 0 && field->moveEntity(enemyX, enemyY, enemyX + dx, enemyY)) {
                } else {
                    tryAlternativeDirections(enemyX, enemyY);
                }
            }
        }
    }
}

void Game::tryAlternativeDirections(int enemyX, int enemyY) {
    std::vector<std::pair<int, int>> alternatives = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    int startIndex = std::rand() % 4;
    for (int i = 0; i < 4; ++i) {
        int index = (startIndex + i) % 4;
        const auto& alt = alternatives[index];
        int altX = enemyX + alt.first;
        int altY = enemyY + alt.second;
        if (field->moveEntity(enemyX, enemyY, altX, altY)) {
            break;
        }
    }
}

void Game::updateGame() {
    handleEnemyTurns();
    removeDeadEnemies();
    findPlayerPosition(); 
}

void Game::checkGameConditions() {
    if (!player->isAlive()) {
        std::cout << "GAME OVER! You were defeated." << std::endl;
        std::cout << "Final score: " << player->getScore() << std::endl;
        std::cout << "Reached level: " << levelManager.getCurrentLevel() << std::endl;
        
        std::cout << "Press 'R' to restart or 'Q' to quit" << std::endl;
        char choice;
        std::cin >> choice;
        
        if (choice == 'r' || choice == 'R') {
            restartGame();
        } else {
            gameRunning = false;
        }
        return;
    }

    bool enemiesAlive = false;
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            enemiesAlive = true;
            break;
        }
    }
    
    if (!enemiesAlive) {
        std::cout << "VICTORY! You defeated all enemies!" << std::endl;
        std::cout << "Current level: " << levelManager.getCurrentLevel() << std::endl;
        std::cout << "Score: " << player->getScore() << std::endl;
        
        std::cout << "Preparing next level..." << std::endl;
        goToNextLevel();
    }
}

void Game::goToNextLevel() {
    levelManager.goToNextLevel();
    auto config = levelManager.getCurrentLevelConfig();
    
    std::cout << "=== LEVEL " << config.levelNumber << " ===" << std::endl;
    std::cout << "Enemies: " << config.enemyCount << " (HP: " << config.enemyHealth << ", Damage: " << config.enemyDamage << ")" << std::endl;
    
    field = std::make_unique<GameField>(15, 15);
    
    auto playerPos = field->getRandomEmptyCell();
    field->addEntity(player, playerPos.first, playerPos.second);
    playerX = playerPos.first;
    playerY = playerPos.second;
    
    int healAmount = player->getMaxHealth() / 2;
    int newHealth = std::min(player->getHealth() + healAmount, player->getMaxHealth());
    player->setHealth(newHealth);
    std::cout << "Health restored to: " << newHealth << "/" << player->getMaxHealth() << std::endl;
    
    player->setMana(player->getMaxMana());
    std::cout << "Mana fully restored" << std::endl;
    
    spawnEnemiesForCurrentLevel();
    
    std::cout << "Level " << config.levelNumber << " ready! Good luck!" << std::endl;
}

void Game::spawnEnemiesForCurrentLevel() {
    enemies.clear();
    auto config = levelManager.getCurrentLevelConfig();
    
    for (int i = 0; i < config.enemyCount; ++i) {
        auto enemyPos = field->getRandomEmptyCell();
        auto enemy = std::make_shared<Enemy>(config.enemyHealth, config.enemyDamage);
        enemies.push_back(enemy);
        field->addEntity(enemy, enemyPos.first, enemyPos.second);
    }
}

void Game::saveGame(const std::string& filename) {
    try {
        GameState state = createGameState();
        state.saveToFile(filename);
    } catch (const std::exception& e) {
        std::cout << "Save failed: " << e.what() << std::endl;
    }
}

void Game::loadGame(const std::string& filename) {
    try {
        GameState state;
        state.loadFromFile(filename);
        applyGameState(state);
    } catch (const std::exception& e) {
        std::cout << "Load failed: " << e.what() << std::endl;
    }
}

GameState Game::createGameState() const {
    GameState state;
    
    state.currentLevel = levelManager.getCurrentLevel();
    state.width = field->getWidth();
    state.height = field->getHeight();
    state.grid.resize(state.height, std::vector<int>(state.width));
    
    for (int y = 0; y < state.height; ++y) {
        for (int x = 0; x < state.width; ++x) {
            auto cellType = field->getCellType(x, y);
            state.grid[y][x] = (cellType == CellType::Wall) ? 1 : 0;
        }
    }
    
    state.playerX = playerX;
    state.playerY = playerY;
    state.playerHealth = player->getHealth();
    state.playerMana = player->getMana();
    state.playerScore = player->getScore();
    state.playerCombatMode = player->getCombatMode();
    
    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        int enemyX = -1, enemyY = -1;
        for (int y = 0; y < static_cast<int>(field->getHeight()); ++y) {
            for (int x = 0; x < static_cast<int>(field->getWidth()); ++x) {
                auto entity = field->getEntityAt(x, y);
                if (entity.get() == enemy.get()) {
                    enemyX = x;
                    enemyY = y;
                    break;
                }
            }
            if (enemyX != -1) break;
        }
        
        if (enemyX != -1) {
            GameState::EnemyData enemyData;
            enemyData.x = enemyX;
            enemyData.y = enemyY;
            enemyData.health = enemy->getHealth();
            enemyData.isAlive = enemy->isAlive();
            state.enemies.push_back(enemyData);
        }
    }
    
    return state;
}

void Game::applyGameState(const GameState& state) {
    levelManager.setCurrentLevel(state.currentLevel);
    auto config = levelManager.getCurrentLevelConfig();
    
    field = std::make_unique<GameField>(state.width, state.height);
    
    for (int y = 0; y < state.height; ++y) {
        for (int x = 0; x < state.width; ++x) {
            CellType cellType = (state.grid[y][x] == 1) ? CellType::Wall : CellType::Empty;
            field->setCellType(x, y, cellType);
        }
    }
    
    enemies.clear();
    

    player = std::make_shared<Player>(100, 10); // максимальное HP
    player->setHealth(state.playerHealth);      // текущее HP
    
    playerX = state.playerX;
    playerY = state.playerY;
    
    player->setMana(state.playerMana);
    player->setScore(state.playerScore);
    player->setCombatMode(state.playerCombatMode);
    
    try {
        field->removeEntity(playerX, playerY);
    } catch (...) {
    }
    field->addEntity(player, playerX, playerY);
    
    for (const auto& enemyData : state.enemies) {
        if (enemyData.isAlive) {
            auto enemy = std::make_shared<Enemy>(enemyData.health, config.enemyDamage);
            try {
                field->removeEntity(enemyData.x, enemyData.y);
            } catch (...) {
            }
            field->addEntity(enemy, enemyData.x, enemyData.y);
            enemies.push_back(enemy);
        }
    }
    
    player->getSpellHand().clear();
    addSpellToPlayer(std::make_shared<DirectDamageSpell>(15, 3));
    if (state.playerScore > 0) {
        addSpellToPlayer(std::make_shared<AreaDamageSpell>(10, 4));
    }
    
    gameRunning = true;
    std::cout << "Game state applied successfully! Level " << config.levelNumber << std::endl;
}

void Game::restartGame() {
    std::cout << "Restarting game..." << std::endl;
    initializeGame();
}

bool Game::isGameRunning() const { 
    return gameRunning; 
}

void Game::quitToMenu() {
    gameRunning = false;
    // Не завершаем программу, только выходим из игрового цикла
}

void Game::quitGame() {
    quitToMenu();  // делаем то же самое
}

int Game::getEnemiesDefeated() const {
    // Простая реализация - можно доработать
    static int defeated = 0;
    return defeated;
}

int Game::getTurnNumber() const {
    // Простая реализация - можно доработать
    static int turn = 0;
    return turn;
}

void Game::addSpellToPlayer(std::shared_ptr<Spell> spell) {
    if (player->getSpellHand().addSpell(spell)) {
        std::cout << "Added spell: " << spell->getName() << " to hand" << std::endl;
    } else {
        std::cout << "Hand is full! Cannot add spell: " << spell->getName() << std::endl;
    }
}