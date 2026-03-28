#include "Game.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include "EventManager.h"
#include "GameEvent.h"

Game::Game(int fieldWidth, int fieldHeight) 
    : currentState(GameState::RUNNING), turnNumber(0), 
      playerStartX(1), playerStartY(1), allyCounter(0),
      currentLevel(1), lastLevelUpScore(0) {
    
    field = std::make_unique<GameField>(fieldWidth, fieldHeight);
    enemyKilledThisTurn = false;
    // Инициализация уровня будет выполнена в startLevel()
}

void Game::initializeGame() {
    // Create player
    player = std::make_unique<Player>(playerStartX, playerStartY);
    
    // Place player on field
    placePlayer();
    
    // Place initial enemies
    placeInitialEnemies();
    
    // Place buildings
    placeBuildings();
}

void Game::placePlayer() {
    field->setPlayerPosition(playerStartX, playerStartY);
}

void Game::placeInitialEnemies() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, field->getWidth() - 1);
    std::uniform_int_distribution<> disY(0, field->getHeight() - 1);
    
    // Place 3-5 initial enemies
    int enemyCount = 3 + (gen() % 3);
    
    // Враги становятся сильнее с каждым уровнем
    // Базовые значения: здоровье 50, урон 10
    // Увеличение: +10 здоровья и +2 урона за каждый уровень после первого
    int baseHealth = 50;
    int baseDamage = 10;
    int healthIncrease = (currentLevel - 1) * 10;
    int damageIncrease = (currentLevel - 1) * 2;
    int enemyHealth = baseHealth + healthIncrease;
    int enemyDamage = baseDamage + damageIncrease;
    
    for (int i = 0; i < enemyCount; ++i) {
        int enemyX, enemyY;
        bool placed = false;
        int attempts = 0;
        
        while (!placed && attempts < 50) {
            enemyX = disX(gen);
            enemyY = disY(gen);
            
            if (field->canMoveTo(enemyX, enemyY) && 
                (enemyX != playerStartX || enemyY != playerStartY)) {
                Enemy newEnemy(enemyX, enemyY, enemyHealth, enemyDamage, i);
                enemies.push_back(newEnemy);
                field->setEnemyPosition(enemyX, enemyY);
                
                // Публикуем событие появления врага
                auto spawnEvent = std::make_shared<EnemySpawnedEvent>(i, enemyX, enemyY, enemyHealth, enemyDamage, turnNumber);
                EventManager::getInstance().publish(spawnEvent);
                
                placed = true;
            }
            attempts++;
        }
    }
}

void Game::placeBuildings() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, field->getWidth() - 1);
    std::uniform_int_distribution<> disY(0, field->getHeight() - 1);
    std::uniform_int_distribution<> disCooldown(3, 7);
    
    // Place 2-3 buildings
    int buildingCount = 2 + (gen() % 2);
    
    for (int i = 0; i < buildingCount; ++i) {
        int buildingX, buildingY;
        bool placed = false;
        int attempts = 0;
        
        while (!placed && attempts < 50) {
            buildingX = disX(gen);
            buildingY = disY(gen);
            
            if (field->canMoveTo(buildingX, buildingY) && 
                (buildingX != playerStartX || buildingY != playerStartY)) {
                int cooldown = disCooldown(gen);
                EnemyBuilding newBuilding(buildingX, buildingY, cooldown, i);
                buildings.push_back(newBuilding);
                field->setBuildingPosition(buildingX, buildingY);
                placed = true;
            }
            attempts++;
        }
    }
}

void Game::run() {
    startGame();
    
    bool continuePlaying = true;
    while (continuePlaying) {
        startLevel();
        
        // Основной игровой цикл уровня
        std::string cmd;
        while (currentState == GameState::RUNNING) {
            displayGameStatus();
            
            std::cout << "Enter command (wasd move, f attack, c cast spell, h show hand, q switch mode, v save, z load, x exit): ";
            if (!(std::cin >> cmd)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input!" << std::endl;
                continue;
            }
            if (cmd.size() != 1) {
                std::cout << "Invalid input! Use single-letter commands." << std::endl;
                continue;
            }
            char input = cmd[0];

            if (input == 'x') {
                return; // Выход из игры
            }
            
            if (input == 'v') {
                // Сохранение игры (v = save)
                std::string filename;
                std::cout << "Enter save filename (default: savegame.txt): ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                if (filename.empty()) {
                    filename = "savegame.txt";
                }
                saveGame(filename);
                continue; // Не тратим ход на сохранение
            }
            
            if (input == 'z') {
                // Загрузка игры (z = load)
                std::string filename;
                std::cout << "Enter load filename (default: savegame.txt): ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                if (filename.empty()) {
                    filename = "savegame.txt";
                }
                if (loadGame(filename)) {
                    std::cout << "Game loaded successfully!" << std::endl;
                } else {
                    std::cout << "Failed to load game!" << std::endl;
                }
                continue; // Не тратим ход на загрузку
            }
            
            processTurn(input);
        }
        
        // Обработка окончания уровня/игры
        continuePlaying = handleGameOver();
    }
}

void Game::startGame() {
    std::cout << "=== Welcome to the Adventure Game! ===" << std::endl;
    std::cout << "Your goal is to survive and eliminate all enemies." << std::endl;
    std::cout << "Buildings will spawn new enemies periodically." << std::endl;
    std::cout << "Be careful of obstacles and slowing cells!" << std::endl;
    std::cout << std::endl;
    displayControls();
}

void Game::startLevel() {
    // Сброс состояния игры для нового уровня
    currentState = GameState::RUNNING;
    turnNumber = 0;
    enemyKilledThisTurn = false;
    
    // Очищаем все объекты предыдущего уровня
    enemies.clear();
    buildings.clear();
    towers.clear();
    allies.clear();
    allyCounter = 0;
    
    // Пересоздаем поле для нового уровня (очищает все позиции и перегенерирует препятствия)
    int fieldWidth = field->getWidth();
    int fieldHeight = field->getHeight();
    field = std::make_unique<GameField>(fieldWidth, fieldHeight);
    
    // Инициализация уровня
    initializeGame();
    
    // Публикуем событие начала игры (только для первого уровня)
    if (currentLevel == 1 && turnNumber == 0) {
        auto startEvent = std::make_shared<GameStartedEvent>(field->getWidth(), field->getHeight());
        EventManager::getInstance().publish(startEvent);
    }
    
    std::cout << "\n=== Level " << currentLevel << " Started ===" << std::endl;
}

void Game::processTurn(char playerInput) {
	processTurnWithAttackDirection(playerInput, '\0');
}

void Game::processTurnWithAttackDirection(char playerInput, char attackDirection) {
    // Проверяем специальные команды, которые не тратят ход
    if (playerInput == 'h') {
        // Show hand - не тратит ход
        if (player->getHand()) {
            player->getHand()->displayHand();
        } else {
            std::cout << "No hand available!" << std::endl;
        }
        return; // Выходим без увеличения turnNumber
    }
    
    turnNumber++;
    
    // Публикуем событие начала хода
    auto turnEvent = std::make_shared<TurnStartedEvent>(turnNumber);
    EventManager::getInstance().publish(turnEvent);
    
    // reset per-turn flags
    enemyKilledThisTurn = false;

    // start-of-turn effects
    player->startTurn();
    
    // Check trap triggers before player move
    checkTrapTriggers();

    // Process player input
    if (playerInput == 'f' && attackDirection != '\0') {
        // Атака с предустановленным направлением (из ControlConfig)
        if (handlePlayerAttack(attackDirection)) {
            enemyKilledThisTurn = true;
        }
    } else {
        processPlayerInput(playerInput);
    }
    
    // Check if player died
    if (!player->isAlive()) {
        currentState = GameState::PLAYER_DIED;
        return;
    }
    
    // Process ally turns
    processAllyTurns();
    
    // Process enemy turns unless enemy was killed by player's attack this turn
    if (!enemyKilledThisTurn) {
        processEnemyTurns();
    }
    
    // Check trap triggers after enemy moves
    checkTrapTriggers();
    
    // Process tower attacks
    processTowerAttacks();
    
    // Process building spawns
    processBuildingSpawns();
    
    // Check game end conditions
    checkGameEndConditions();
    
    // Handle slowing cell effects
    handleSlowingCellEffect();
    
    // End player turn
    player->endTurn();
}

void Game::processPlayerInput(char input) {
    int currentX = player->getPositionX();
    int currentY = player->getPositionY();
    int newX = currentX;
    int newY = currentY;
    
    switch (input) {
        case 'w':
            newY--;
            break;
        case 's':
            newY++;
            break;
        case 'a':
            newX--;
            break;
        case 'd':
            newX++;
            break;
        case 'f': {
            // Атака теперь обрабатывается через GameController::handleAttackCommand
            // Этот код используется только при прямом вызове Game::run() (старый способ)
            std::string dirToken;
            std::cout << "Attack direction (w/a/s/d): ";
            if (!(std::cin >> dirToken)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid direction." << std::endl;
                return;
            }
            if (dirToken.size() != 1) {
                std::cout << "Invalid direction." << std::endl;
                return;
            }
            char dir = dirToken[0];
            if (handlePlayerAttack(dir)) {
                enemyKilledThisTurn = true;
            }
            return;
        }
        case 'q':
            player->switchCombatMode();
            std::cout << "Switching combat mode..." << std::endl;
            return;
        case 'c': {
            // Cast spell
            if (player->getHand() && player->getHand()->getSpellCount() > 0) {
                // Показываем список заклинаний
                std::cout << "\nAvailable spells:" << std::endl;
                for (size_t i = 0; i < player->getHand()->getSpellCount(); ++i) {
                    Spell* s = player->getHand()->getSpell(i);
                    if (s) {
                        std::cout << "[" << i << "] " << s->getName() << " - " << s->getDescription() << std::endl;
                    }
                }
                
                int spellIndex;
                std::cout << "Select spell index: ";
                if (!(std::cin >> spellIndex)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid spell index." << std::endl;
                    return;
                }
                
                Spell* spell = player->getHand()->getSpell(spellIndex);
                if (!spell) {
                    std::cout << "Invalid spell index!" << std::endl;
                    return;
                }
                
                // Enhancement применяется сразу без координат
                if (spell->getName() == "Enhancement") {
                    handlePlayerSpell(spellIndex, 0, 0); // Координаты не важны для Enhancement
                } else {
                    // Для остальных заклинаний нужны координаты
                    int targetX, targetY;
                    std::cout << "Enter target X: ";
                    if (!(std::cin >> targetX)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid target X." << std::endl;
                        return;
                    }
                    std::cout << "Enter target Y: ";
                    if (!(std::cin >> targetY)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid target Y." << std::endl;
                        return;
                    }
                    handlePlayerSpell(spellIndex, targetX, targetY);
                }
            } else {
                std::cout << "No spells available!" << std::endl;
            }
            return;
        }
        // Команда 'h' обрабатывается в processTurn перед увеличением turnNumber
        // Здесь не нужно обрабатывать
        default:
            std::cout << "Invalid input!" << std::endl;
            return;
    }
    
    if (isValidPlayerMove(newX, newY)) {
        // Clear old position
        field->clearPosition(currentX, currentY);
        
        // Move player
        player->moveTo(newX, newY);
        field->setPlayerPosition(newX, newY);
        
        // Публикуем событие перемещения
        auto moveEvent = std::make_shared<PlayerMovedEvent>(currentX, currentY, newX, newY, turnNumber);
        EventManager::getInstance().publish(moveEvent);
        
        // If stepped on slowing cell, apply slow for the next turn only
        if (field->getCell(newX, newY).getType() == CellType::SLOWING) {
            player->applySlowingNextTurn();
        }
        
        std::cout << "Player moved to (" << newX << ", " << newY << ")" << std::endl;
    } else {
        std::cout << "Cannot move there!" << std::endl;
    }
}

bool Game::handlePlayerAttack(char direction) {
    int px = player->getPositionX();
    int py = player->getPositionY();
    int damage = player->getDamage();
    bool hit = false;
    bool killed = false;

    int dx = 0, dy = 0;
    switch (direction) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        default:
            std::cout << "Invalid direction." << std::endl;
            return false;
    }

    if (player->getCombatMode() == CombatMode::MELEE) {
        // Ближний бой: бьем ровно соседнюю клетку по заданному направлению
        int tx = px + dx;
        int ty = py + dy;
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            if (enemy.getPositionX() == tx && enemy.getPositionY() == ty) {
                // Мечевой удар добивает цель гарантированно
                int enemyId = enemy.getId();
                int enemyHealth = enemy.getHealth();
                enemy.takeDamage(std::max(damage, enemy.getHealth()));
                std::cout << "Melee hit! Enemy defeated. +10 score." << std::endl;
                hit = true;
                field->clearPosition(enemy.getPositionX(), enemy.getPositionY());
                
                // Публикуем события атаки и убийства
                auto attackEvent = std::make_shared<PlayerAttackedEvent>(damage, tx, ty, "Enemy", true, turnNumber);
                EventManager::getInstance().publish(attackEvent);
                auto killEvent = std::make_shared<EnemyKilledEvent>(enemyId, tx, ty, 10, turnNumber);
                EventManager::getInstance().publish(killEvent);
                
                int oldScore = player->getScore();
                player->addScore(10);
                publishScoreChange(oldScore, player->getScore());
                killed = true;
                break;
            }
        }
    } else {
        // Дальний бой: луч до 5 клеток по направлению, поражает всех врагов на линии
        // Луч останавливается при встрече с препятствием (стеной)
        const int range = 5;
        for (int step = 1; step <= range; ++step) {
            int tx = px + dx * step;
            int ty = py + dy * step;
            
            // Проверяем, что позиция валидна
            if (!field->isValidPosition(tx, ty)) {
                break; // Выход за пределы поля - останавливаем луч
            }
            
            // Проверяем на препятствие (стену) - луч останавливается
            const Cell& cell = field->getCell(tx, ty);
            if (!cell.isPassable()) {
                break; // Стена блокирует луч
            }
            
            // Проверяем врагов на этой клетке
            for (auto& enemy : enemies) {
                if (!enemy.isAlive()) continue;
                if (enemy.getPositionX() == tx && enemy.getPositionY() == ty) {
                    // Гарантированное убийство цели дальним выстрелом
                    int enemyId = enemy.getId();
                    int enemyHealth = enemy.getHealth();
                    enemy.takeDamage(std::max(damage, enemy.getHealth()));
                    std::cout << "Ranged hit for " << damage << " damage." << std::endl;
                    hit = true;
                    
                    // Публикуем событие атаки
                    auto attackEvent = std::make_shared<PlayerAttackedEvent>(damage, tx, ty, "Enemy", !enemy.isAlive(), turnNumber);
                    EventManager::getInstance().publish(attackEvent);
                    
                    if (!enemy.isAlive()) {
                        field->clearPosition(enemy.getPositionX(), enemy.getPositionY());
                        int oldScore = player->getScore();
                        player->addScore(10);
                        std::cout << "Enemy defeated! +10 score." << std::endl;
                        
                        // Публикуем событие убийства
                        auto killEvent = std::make_shared<EnemyKilledEvent>(enemyId, tx, ty, 10, turnNumber);
                        EventManager::getInstance().publish(killEvent);
                        
                        publishScoreChange(oldScore, player->getScore());
                        killed = true;
                    }
                    break; // на одной клетке не может быть более одного врага
                }
            }
        }
    }

    if (!hit) {
        std::cout << "No enemy in attack range." << std::endl;
    }

    return killed;
}

void Game::processEnemyTurns() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            // Проверяем, есть ли союзники рядом с врагом для атаки
            bool attackedAlly = false;
            for (auto& ally : allies) {
                if (!ally.isAlive()) continue;
                
                int dx = std::abs(enemy.getPositionX() - ally.getPositionX());
                int dy = std::abs(enemy.getPositionY() - ally.getPositionY());
                
                // Melee атака союзника: только если на соседней клетке и не по диагонали
                if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                    ally.takeDamage(enemy.getDamage());
                    std::cout << "Enemy attacked ally for " << enemy.getDamage() << " damage!" << std::endl;
                    attackedAlly = true;
                    
                    if (!ally.isAlive()) {
                        // Очищаем позицию мертвого союзника
                        field->clearPosition(ally.getPositionX(), ally.getPositionY());
                        std::cout << "Ally died!" << std::endl;
                    }
                    break; // Атакуем только одного союзника за ход
                }
            }
            
            // Если не атаковали союзника, двигаемся/атакуем игрока
            if (!attackedAlly) {
                int oldHealth = player->getHealth();
                enemy.makeMove(*field, *player);
                // Проверяем, был ли нанесен урон игроку
                if (player->getHealth() < oldHealth) {
                    int damage = oldHealth - player->getHealth();
                    auto damageEvent = std::make_shared<PlayerDamagedEvent>(
                        damage, player->getHealth(), player->getMaxHealth(), "Enemy", turnNumber);
                    EventManager::getInstance().publish(damageEvent);
                }
            }
        }
    }
}

void Game::processBuildingSpawns() {
    // Вычисляем параметры врагов для текущего уровня
    int baseHealth = 50;
    int baseDamage = 10;
    int healthIncrease = (currentLevel - 1) * 10;
    int damageIncrease = (currentLevel - 1) * 2;
    int enemyHealth = baseHealth + healthIncrease;
    int enemyDamage = baseDamage + damageIncrease;
    
    for (auto& building : buildings) {
        building.updateCooldown();
        // Создаем врага с усиленными параметрами для текущего уровня
        size_t enemiesBefore = enemies.size();
        if (building.trySpawnEnemy(*field, enemies, enemyHealth, enemyDamage)) {
            std::cout << "New enemy spawned!" << std::endl;
            // Публикуем событие появления врага (последний добавленный)
            if (enemies.size() > enemiesBefore) {
                const Enemy& newEnemy = enemies.back();
                auto spawnEvent = std::make_shared<EnemySpawnedEvent>(
                    newEnemy.getId(), newEnemy.getPositionX(), newEnemy.getPositionY(),
                    newEnemy.getHealth(), newEnemy.getDamage(), turnNumber);
                EventManager::getInstance().publish(spawnEvent);
            }
        }
    }
}

void Game::checkGameEndConditions() {
    // Проверка перехода на следующий уровень (каждые 200 очков)
    int currentScore = player->getScore();
    if (currentScore >= lastLevelUpScore + 200) {
        advanceToNextLevel();
        return;
    }
    
    // Win if score threshold reached
    if (player->getScore() >= 1000) {
        currentState = GameState::PLAYER_WON;
        return;
    }

    // Check if all enemies are dead
    bool allEnemiesDead = std::all_of(enemies.begin(), enemies.end(),
        [](const Enemy& enemy) { return !enemy.isAlive(); });
    
    if (allEnemiesDead && enemies.size() > 0) {
        currentState = GameState::PLAYER_WON;
    }
}

void Game::handleSlowingCellEffect() {
    int playerX = player->getPositionX();
    int playerY = player->getPositionY();
    
    // No-op: slowing is applied only when stepping onto a slowing cell
}

void Game::advanceToNextLevel() {
    currentLevel++;
    lastLevelUpScore = (player->getScore() / 200) * 200; // Округляем до ближайшего кратного 200
    
    // Публикуем событие повышения уровня
    auto levelEvent = std::make_shared<LevelUpEvent>(currentLevel, turnNumber);
    EventManager::getInstance().publish(levelEvent);
    
    std::cout << "\n=== LEVEL UP! ===\n";
    std::cout << "Advancing to Level " << currentLevel << "!" << std::endl;
    
    // Увеличиваем размер поля на 1 по x и y
    int newWidth = field->getWidth() + 1;
    int newHeight = field->getHeight() + 1;
    
    // Сохраняем текущую позицию игрока
    int playerX = player->getPositionX();
    int playerY = player->getPositionY();
    
    // Создаем новое поле большего размера
    field = std::make_unique<GameField>(newWidth, newHeight);
    
    // Обновляем стартовую позицию игрока (оставляем в том же месте, если возможно)
    if (playerX < newWidth && playerY < newHeight) {
        playerStartX = playerX;
        playerStartY = playerY;
    } else {
        // Если старая позиция выходит за границы, ставим игрока в начало
        playerStartX = 1;
        playerStartY = 1;
    }
    
    // Показываем меню улучшений ПЕРЕД восстановлением здоровья
    showUpgradeMenu();
    
    // Восстанавливаем здоровье игрока до максимума (после улучшений)
    int maxHealth = player->getMaxHealth();
    int oldHealth = player->getHealth();
    player->heal(maxHealth); // heal восстанавливает до максимума, если переданное значение больше maxHealth
    
    // Публикуем событие лечения игрока
    if (player->getHealth() > oldHealth) {
        int healAmount = player->getHealth() - oldHealth;
        auto healEvent = std::make_shared<PlayerHealedEvent>(
            healAmount, player->getHealth(), player->getMaxHealth(), turnNumber);
        EventManager::getInstance().publish(healEvent);
    }
    
    // Удаляем половину заклинаний случайным образом
    if (player->getHand() && player->getHand()->getSpellCount() > 0) {
        size_t spellCount = player->getHand()->getSpellCount();
        size_t spellsToRemove = spellCount / 2; // Половина заклинаний
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Создаем вектор индексов для случайного удаления
        std::vector<size_t> indices;
        for (size_t i = 0; i < spellCount; ++i) {
            indices.push_back(i);
        }
        
        // Перемешиваем индексы
        std::shuffle(indices.begin(), indices.end(), gen);
        
        // Удаляем заклинания в обратном порядке индексов, чтобы не нарушить порядок
        std::sort(indices.begin(), indices.begin() + spellsToRemove, std::greater<size_t>());
        
        for (size_t i = 0; i < spellsToRemove; ++i) {
            player->getHand()->removeSpell(indices[i]);
        }
        
        std::cout << "Removed " << spellsToRemove << " spell(s) from your hand." << std::endl;
    }
    
    // Очищаем все объекты предыдущего уровня
    enemies.clear();
    buildings.clear();
    towers.clear();
    allies.clear();
    allyCounter = 0;
    
    // Перемещаем игрока на новую стартовую позицию
    player->moveTo(playerStartX, playerStartY);
    
    // Размещаем игрока на поле (не пересоздаем его!)
    placePlayer();
    
    // Размещаем врагов и здания (не пересоздаем игрока!)
    placeInitialEnemies();
    placeBuildings();
    
    std::cout << "Field size increased to " << newWidth << "x" << newHeight << std::endl;
    std::cout << "Enemies are now stronger!" << std::endl;
    std::cout << "Your health has been restored to full!" << std::endl;
    std::cout << "\n=== Level " << currentLevel << " Started ===" << std::endl;
}

void Game::showUpgradeMenu() {
    std::cout << "\n=== UPGRADE MENU ===" << std::endl;
    std::cout << "Choose an upgrade:" << std::endl;
    std::cout << "1. Increase Max Health (+20)" << std::endl;
    std::cout << "2. Increase Melee Damage (+5)" << std::endl;
    std::cout << "3. Increase Ranged Damage (+5)" << std::endl;
    
    // Показываем доступные заклинания для улучшения
    if (player->getHand() && player->getHand()->getSpellCount() > 0) {
        std::cout << "\nSpell Upgrades:" << std::endl;
        for (size_t i = 0; i < player->getHand()->getSpellCount(); ++i) {
            Spell* spell = player->getHand()->getSpell(i);
            if (spell) {
                std::cout << (4 + i) << ". Upgrade " << spell->getName() 
                          << " (Damage +10, Radius +1)" << std::endl;
            }
        }
    }
    
    std::cout << "Enter your choice: ";
    
    int choice = 0;
    std::string input;
    
    try {
        if (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Failed to read input in upgrade menu");
        }
        
        try {
            choice = std::stoi(input);
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Invalid choice format. Expected a number.");
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Choice value is out of range.");
        }
        
        size_t maxSpellIndex = 0;
        if (player->getHand()) {
            maxSpellIndex = player->getHand()->getSpellCount();
        }
        int maxChoice = 3 + static_cast<int>(maxSpellIndex);
        
        if (choice < 1 || choice > maxChoice) {
            throw std::out_of_range("Choice must be between 1 and " + std::to_string(maxChoice));
        }
        
        // Обрабатываем выбор
        if (choice == 1) {
            try {
                player->upgradeMaxHealth(20);
                std::cout << "Max Health increased by 20! New max health: " 
                          << player->getMaxHealth() << std::endl;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error upgrading max health: " << e.what() << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Error upgrading max health: " << e.what() << std::endl;
            }
        } else if (choice == 2) {
            try {
                player->upgradeMeleeDamage(5);
                std::cout << "Melee Damage increased by 5! New melee damage: " 
                          << player->getMeleeDamage() << std::endl;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error upgrading melee damage: " << e.what() << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Error upgrading melee damage: " << e.what() << std::endl;
            }
        } else if (choice == 3) {
            try {
                player->upgradeRangedDamage(5);
                std::cout << "Ranged Damage increased by 5! New ranged damage: " 
                          << player->getRangedDamage() << std::endl;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error upgrading ranged damage: " << e.what() << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Error upgrading ranged damage: " << e.what() << std::endl;
            }
        } else {
            // Улучшение заклинания
            size_t spellIndex = static_cast<size_t>(choice - 4);
            if (player->getHand() && spellIndex < player->getHand()->getSpellCount()) {
                try {
                    Spell* spell = player->getHand()->getSpell(spellIndex);
                    if (spell) {
                        spell->applyEnhancement(1, 10, 0, 0); // +1 радиус, +10 урон
                        std::cout << spell->getName() << " upgraded! Damage +10, Radius +1" << std::endl;
                    } else {
                        throw std::runtime_error("Spell at index " + std::to_string(spellIndex) + " is null");
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error upgrading spell: " << e.what() << std::endl;
                }
            } else {
                throw std::out_of_range("Invalid spell index: " + std::to_string(spellIndex));
            }
        }
        
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input in upgrade menu: " << e.what() << std::endl;
        std::cout << "No upgrade applied." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Choice out of range in upgrade menu: " << e.what() << std::endl;
        std::cout << "No upgrade applied." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error in upgrade menu: " << e.what() << std::endl;
        std::cout << "No upgrade applied." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error in upgrade menu: " << e.what() << std::endl;
        std::cout << "No upgrade applied." << std::endl;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool Game::isValidPlayerMove(int newX, int newY) const {
    if (!player->canMove()) {
        return false;
    }
    
    if (!field->canMoveTo(newX, newY)) {
        return false;
    }
    
    // Check if there's an enemy at the target position
    if (field->getCell(newX, newY).hasEnemyOnCell()) {
        return false;
    }
    
    return true;
}

GameState Game::getGameState() const {
    return currentState;
}

int Game::getTurnNumber() const {
    return turnNumber;
}

const Player& Game::getPlayer() const {
    if (!player) {
        throw std::runtime_error("Player not initialized. Call startLevel() first.");
    }
    return *player;
}

const GameField& Game::getField() const {
    return *field;
}

const std::vector<Enemy>& Game::getEnemies() const {
    return enemies;
}

void Game::displayGameStatus() const {
    std::cout << "\n=== Turn " << turnNumber << " ===" << std::endl;
    std::cout << "Player Health: " << player->getHealth() << "/" << player->getMaxHealth() << std::endl;
    std::cout << "Player Score: " << player->getScore() << std::endl;
    std::cout << "Combat Mode: " << (player->getCombatMode() == CombatMode::MELEE ? "Melee" : "Ranged") << std::endl;
    std::cout << "Player Position: (" << player->getPositionX() << ", " << player->getPositionY() << ")" << std::endl;
    
    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            aliveEnemies++;
        }
    }
    std::cout << "Alive Enemies: " << aliveEnemies << std::endl;
    
    int aliveAllies = 0;
    for (const auto& ally : allies) {
        if (ally.isAlive()) {
            aliveAllies++;
        }
    }
    std::cout << "Alive Allies: " << aliveAllies << std::endl;
    
    if (player->getHand()) {
        std::cout << "Spells in hand: " << player->getHand()->getSpellCount() << "/" 
                  << player->getHand()->getMaxSize() << std::endl;
    }
    
    std::cout << "\nGame Field:" << std::endl;
    field->displayField();
}

void Game::displayControls() const {
    std::cout << "=== Game Controls ===" << std::endl;
    std::cout << "w - Move up" << std::endl;
    std::cout << "s - Move down" << std::endl;
    std::cout << "a - Move left" << std::endl;
    std::cout << "d - Move right" << std::endl;
    std::cout << "f - Attack, then choose direction (w/a/s/d). Melee: 1 tile, Ranged: up to 3 tiles" << std::endl;
    std::cout << "c - Cast spell (select spell and target)" << std::endl;
    std::cout << "h - Show hand (spells)" << std::endl;
    std::cout << "q - Switch combat mode (takes 1 turn)" << std::endl;
    std::cout << "v - Save game" << std::endl;
    std::cout << "z - Load game" << std::endl;
    std::cout << "x - Exit game" << std::endl;
    std::cout << "\nLegend:" << std::endl;
    std::cout << "P - Player" << std::endl;
    std::cout << "E - Enemy" << std::endl;
    std::cout << "B - Building" << std::endl;
    std::cout << "A - Ally" << std::endl;
    std::cout << "T - Trap" << std::endl;
    std::cout << "# - Obstacle" << std::endl;
    std::cout << "~ - Slowing cell" << std::endl;
    std::cout << ". - Empty cell" << std::endl;
    std::cout << "=====================" << std::endl;
}

bool Game::isGameOver() const {
    return currentState != GameState::RUNNING;
}

// Implement new methods
void Game::processAllyTurns() {
    for (auto& ally : allies) {
        if (ally.isAlive()) {
            ally.makeMove(*field, *player, enemies);
        }
    }
}

void Game::processTowerAttacks() {
    for (auto& tower : towers) {
        tower.update();
        int oldHealth = player->getHealth();
        bool attacked = tower.tryAttackPlayer(*this, *player);
        // Проверяем, был ли нанесен урон игроку
        if (attacked && player->getHealth() < oldHealth) {
            int damage = oldHealth - player->getHealth();
            auto damageEvent = std::make_shared<PlayerDamagedEvent>(
                damage, player->getHealth(), player->getMaxHealth(), "Tower", turnNumber);
            EventManager::getInstance().publish(damageEvent);
        }
    }
}

void Game::checkTrapTriggers() {
    // Check if enemies stepped on traps
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        
        int ex = enemy.getPositionX();
        int ey = enemy.getPositionY();
        
        if (field->isValidPosition(ex, ey)) {
            Cell& cell = field->getCell(ex, ey);
            if (cell.hasTrapOnCell()) {
                int trapDamage = cell.getTrapDamage();
                int enemyId = enemy.getId();
                int enemyHealth = enemy.getHealth();
                enemy.takeDamage(trapDamage);
                cell.clearTrap();
                std::cout << "Enemy triggered a trap at (" << ex << ", " << ey << ")! Took " << trapDamage << " damage!" << std::endl;
                
                // Публикуем событие срабатывания ловушки
                auto trapEvent = std::make_shared<TrapTriggeredEvent>(ex, ey, trapDamage, "Enemy", turnNumber);
                EventManager::getInstance().publish(trapEvent);
                
                if (!enemy.isAlive()) {
                    // Очищаем позицию мертвого врага
                    field->clearPosition(enemy.getPositionX(), enemy.getPositionY());
                    
                    // Публикуем событие убийства врага
                    auto killEvent = std::make_shared<EnemyKilledEvent>(enemyId, ex, ey, 10, turnNumber);
                    EventManager::getInstance().publish(killEvent);
                    
                    onEnemyKilled();
                }
            }
        }
    }
}

bool Game::handlePlayerSpell(int spellIndex, int targetX, int targetY) {
    if (!player->getHand()) {
        return false;
    }
    
    Spell* spell = player->getHand()->getSpell(spellIndex);
    if (!spell) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }
    
    // Получаем накопленные улучшения
    Enhancement pending = EnhancementSpell::getPendingEnhancement();
    bool isEnhancementSpell = (spell->getName() == "Enhancement");
    
    // Применяем улучшения к заклинанию (кроме EnhancementSpell)
    if (!isEnhancementSpell) {
        if (pending.radiusBonus != 0 || pending.damageBonus != 0 || 
            pending.areaBonus != 0 || pending.summonBonus != 0) {
            spell->applyEnhancement(pending.radiusBonus, pending.damageBonus, 
                                   pending.areaBonus, pending.summonBonus);
            EnhancementSpell::clearPendingEnhancement();
        }
    }
    
    // Применяем заклинание
    SpellResult result = spell->use(*this, targetX, targetY);
    
    std::cout << result.message << std::endl;
    
    // Публикуем событие применения заклинания
    auto spellEvent = std::make_shared<SpellCastEvent>(spell->getName(), targetX, targetY, result.success, turnNumber);
    EventManager::getInstance().publish(spellEvent);
    
    // Удаляем заклинание из руки после использования (всегда тратит ход)
    if (result.turnUsed) {
        player->getHand()->removeSpell(spellIndex);
    }
    
    return result.success;
}

bool Game::placeTrap(int x, int y, int damage) {
    if (!field->isValidPosition(x, y)) {
        std::cout << "Trap placement failed: Invalid position!" << std::endl;
        return false;
    }
    
    Cell& cell = field->getCell(x, y);
    
    // Проверяем, что клетка проходима
    if (!cell.isPassable()) {
        std::cout << "Trap placement failed: Cell is not passable (wall)!" << std::endl;
        return false;
    }
    
    // Проверяем, что на клетке нет ловушки
    if (cell.hasTrapOnCell()) {
        std::cout << "Trap placement failed: Trap already exists here!" << std::endl;
        return false;
    }
    
    // Проверяем, что клетка не занята игроком, врагом или зданием
    // Союзник НЕ блокирует размещение ловушки - можно ставить ловушку на клетку с союзником
    if (cell.hasPlayerOnCell() || cell.hasEnemyOnCell() || cell.hasBuildingOnCell()) {
        std::cout << "Trap placement failed: Cell is occupied by player, enemy, or building!" << std::endl;
        return false;
    }
    
    // Союзник на клетке не мешает - ловушка устанавливается
    
    // Устанавливаем ловушку
    field->setTrap(x, y, damage);
    std::cout << "Trap successfully placed at (" << x << ", " << y << ") with " << damage << " damage!" << std::endl;
    return true;
}

bool Game::summonAlly(int x, int y) {
    if (!field->isValidPosition(x, y)) {
        return false;
    }
    
    Cell& cell = field->getCell(x, y);
    // Проверяем, что клетка проходима, пуста и не занята игроком
    if (!cell.isPassable() || !cell.isEmpty() || cell.hasPlayerOnCell()) {
        return false;
    }
    
    int newAllyId = allyCounter++;
    Ally newAlly(x, y, 30, 5, newAllyId);
    allies.push_back(newAlly);
    field->setAllyPosition(x, y);
    
    // Публикуем событие призыва союзника
    auto allyEvent = std::make_shared<AllySummonedEvent>(newAllyId, x, y, turnNumber);
    EventManager::getInstance().publish(allyEvent);
    
    return true;
}

void Game::onEnemyKilled() {
    player->onEnemyKilled();
    enemyKilledThisTurn = true;
}

Player& Game::getPlayer() {
    if (!player) {
        throw std::runtime_error("Player not initialized. Call startLevel() first.");
    }
    return *player;
}

GameField& Game::getField() {
    return *field;
}

std::vector<Enemy>& Game::getEnemies() {
    return enemies;
}

const std::vector<Ally>& Game::getAllies() const {
    return allies;
}

std::vector<Ally>& Game::getAllies() {
    return allies;
}

std::vector<EnemyBuilding>& Game::getBuildings() {
    return buildings;
}

const std::vector<EnemyBuilding>& Game::getBuildings() const {
    return buildings;
}

bool Game::handleGameOver() {
    // Публикуем событие окончания игры
    bool playerWon = (currentState == GameState::PLAYER_WON);
    int finalScore = player ? player->getScore() : 0;
    auto endEvent = std::make_shared<GameEndedEvent>(playerWon, finalScore, turnNumber);
    EventManager::getInstance().publish(endEvent);
    
    if (currentState == GameState::PLAYER_DIED) {
        std::cout << "\n=== Game Over! Player died! ===" << std::endl;
    } else if (currentState == GameState::PLAYER_WON) {
        std::cout << "\n=== Congratulations! You won! ===" << std::endl;
    }
    
    // Предлагаем начать заново или выйти
    std::cout << "\nWhat would you like to do?" << std::endl;
    std::cout << "1 - Start new game" << std::endl;
    std::cout << "2 - Exit" << std::endl;
    std::cout << "Your choice: ";
    
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    
    if (choice == 1) {
        // Очищаем текущее состояние для новой игры
        enemies.clear();
        buildings.clear();
        towers.clear();
        allies.clear();
        allyCounter = 0;
        
        // Пересоздаем поле и игрока
        field = std::make_unique<GameField>(field->getWidth(), field->getHeight());
        player = std::make_unique<Player>(playerStartX, playerStartY);
        
        std::cout << "\nStarting new game..." << std::endl;
        return true; // Продолжаем игру
    } else {
        std::cout << "Thank you for playing! Goodbye!" << std::endl;
        return false; // Выходим из игры
    }
}

bool Game::saveGame(const std::string& filename) const {
    // Проверка на пустое имя файла
    if (filename.empty()) {
        try {
            throw FileSaveException(filename, "Filename cannot be empty", "Game::saveGame");
        } catch (const FileSaveException& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
    
    // Попытка открыть файл для записи (RAII - файл автоматически закроется при выходе из области видимости)
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        try {
            throw FileSaveException(filename, "Failed to open file (insufficient permissions, disk full, or invalid path)", "Game::saveGame");
        } catch (const FileSaveException& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
    
    try {
        // Сохраняем общее состояние игры
        file << "[GameState]\n";
        if (file.fail()) {
            throw FileSaveException(filename, "Failed to write game state section header", "Game::saveGame");
        }
        
        file << "state=" << static_cast<int>(currentState) << "\n";
        file << "turnNumber=" << turnNumber << "\n";
        file << "playerStartX=" << playerStartX << "\n";
        file << "playerStartY=" << playerStartY << "\n";
        file << "allyCounter=" << allyCounter << "\n";
        file << "currentLevel=" << currentLevel << "\n";
        file << "lastLevelUpScore=" << lastLevelUpScore << "\n";
        
        if (file.fail()) {
            throw FileSaveException(filename, "Failed to write game state data", "Game::saveGame");
        }
        
        // Сохраняем поле (serialize уже содержит заголовок [GameField])
        file << "\n";
        std::string fieldData = field->serialize();
        file << fieldData;
        
        if (file.fail()) {
            throw FileSaveException(filename, "Failed to write game field data", "Game::saveGame");
        }
        
        // Сохраняем игрока
        file << "\n[Player]\n";
        if (player) {
            std::string playerData = player->serialize();
            file << playerData;
        } else {
            file << "NO_PLAYER\n";
        }
        
        if (file.fail()) {
            throw FileSaveException(filename, "Failed to write player data", "Game::saveGame");
        }
        
        // Сохраняем врагов
        file << "\n[Enemies]\n";
        file << "count=" << enemies.size() << "\n";
        for (size_t i = 0; i < enemies.size(); ++i) {
            file << "enemy_" << i << "=" << enemies[i].serialize();
            if (file.fail()) {
                throw FileSaveException(filename, "Failed to write enemy " + std::to_string(i), "Game::saveGame");
            }
        }
        
        // Сохраняем союзников
        file << "\n[Allies]\n";
        file << "count=" << allies.size() << "\n";
        for (size_t i = 0; i < allies.size(); ++i) {
            file << "ally_" << i << "=" << allies[i].serialize();
            if (file.fail()) {
                throw FileSaveException(filename, "Failed to write ally " + std::to_string(i), "Game::saveGame");
            }
        }
        
        // Сохраняем здания
        file << "\n[Buildings]\n";
        file << "count=" << buildings.size() << "\n";
        for (size_t i = 0; i < buildings.size(); ++i) {
            file << "building_" << i << "=" << buildings[i].serialize();
            if (file.fail()) {
                throw FileSaveException(filename, "Failed to write building " + std::to_string(i), "Game::saveGame");
            }
        }
        
        // Сохраняем башни
        file << "\n[Towers]\n";
        file << "count=" << towers.size() << "\n";
        for (size_t i = 0; i < towers.size(); ++i) {
            file << "tower_" << i << "=" << towers[i].serialize();
            if (file.fail()) {
                throw FileSaveException(filename, "Failed to write tower " + std::to_string(i), "Game::saveGame");
            }
        }
        
        // Проверяем финальное состояние файла
        if (file.fail() || !file.good()) {
            throw FileSaveException(filename, "File write operation failed - save may be incomplete", "Game::saveGame");
        }
        
        // Файл автоматически закроется при выходе из области видимости (RAII)
        std::cout << "Game saved successfully to " << filename << std::endl;
        return true;
        
    } catch (const FileSaveException& e) {
        std::cout << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (const std::exception& e) {
        std::cout << "Error: Exception occurred while saving game: " << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (...) {
        std::cout << "Error: Unknown exception occurred while saving game." << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    }
}

bool Game::loadGame(const std::string& filename) {
    // Проверка на пустое имя файла
    if (filename.empty()) {
        try {
            throw FileLoadException(filename, "Filename cannot be empty", "Game::loadGame");
        } catch (const FileLoadException& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
    
    // Попытка открыть файл для чтения (RAII - файл автоматически закроется при выходе из области видимости)
    std::ifstream file(filename);
    if (!file.is_open()) {
        try {
            throw FileLoadException(filename, "Failed to open file (file does not exist, insufficient permissions, or invalid path)", "Game::loadGame");
        } catch (const FileLoadException& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
    
    // Проверка на пустой файл
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        try {
            throw CorruptedSaveFileException(filename, "Save file is empty", "Game::loadGame");
        } catch (const CorruptedSaveFileException& e) {
            std::cout << e.what() << std::endl;
            // Файл автоматически закроется при выходе из области видимости (RAII)
            return false;
        }
    }
    file.seekg(0, std::ios::beg);
    
    try {
        std::string line;
        std::string currentSection;
        std::stringstream fieldData;
        std::stringstream playerData;
        std::vector<std::string> enemyData;
        std::vector<std::string> allyData;
        std::vector<std::string> buildingData;
        std::vector<std::string> towerData;
        
        int enemyCount = 0;
        int allyCount = 0;
        int buildingCount = 0;
        int towerCount = 0;
        
        bool hasGameState = false;
        bool hasGameField = false;
        bool hasPlayer = false;
        
        // Читаем файл
        while (std::getline(file, line)) {
            // Обрабатываем секции
            if (!line.empty() && line[0] == '[' && line.back() == ']') {
                std::string sectionName = line.substr(1, line.length() - 2);
                // Если это секция GameField, добавляем заголовок в fieldData и устанавливаем currentSection
                if (sectionName == "GameField") {
                    currentSection = sectionName;
                    hasGameField = true;
                    fieldData << line << "\n";
                } else {
                    currentSection = sectionName;
                    if (sectionName == "GameState") {
                        hasGameState = true;
                    } else if (sectionName == "Player") {
                        hasPlayer = true;
                    }
                }
                continue;
            }
            
            if (currentSection == "GameState") {
                size_t pos = line.find('=');
                if (pos == std::string::npos) continue;
                
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                try {
                    if (key == "state") {
                        int stateValue = std::stoi(value);
                        if (stateValue < 0 || stateValue > 2) {
                            throw InvalidDataException("GameState", "Invalid game state value: " + std::to_string(stateValue), "Game::loadGame");
                        }
                        currentState = static_cast<GameState>(stateValue);
                    } else if (key == "turnNumber") {
                        turnNumber = std::stoi(value);
                        if (turnNumber < 0) {
                            throw InvalidDataException("TurnNumber", "Invalid turn number: " + std::to_string(turnNumber), "Game::loadGame");
                        }
                    } else if (key == "playerStartX") {
                        playerStartX = std::stoi(value);
                    } else if (key == "playerStartY") {
                        playerStartY = std::stoi(value);
                    } else if (key == "allyCounter") {
                        allyCounter = std::stoi(value);
                        if (allyCounter < 0) {
                            throw InvalidDataException("AllyCounter", "Invalid ally counter: " + std::to_string(allyCounter), "Game::loadGame");
                        }
                    } else if (key == "currentLevel") {
                        currentLevel = std::stoi(value);
                        if (currentLevel < 1) {
                            throw InvalidDataException("CurrentLevel", "Invalid current level: " + std::to_string(currentLevel), "Game::loadGame");
                        }
                    } else if (key == "lastLevelUpScore") {
                        lastLevelUpScore = std::stoi(value);
                        if (lastLevelUpScore < 0) {
                            throw InvalidDataException("LastLevelUpScore", "Invalid last level up score: " + std::to_string(lastLevelUpScore), "Game::loadGame");
                        }
                    }
                } catch (const InvalidDataException& e) {
                    throw;
                } catch (const std::invalid_argument& e) {
                    throw InvalidDataException("GameState", "Invalid number format: " + key + "=" + value, "Game::loadGame");
                } catch (const std::out_of_range& e) {
                    throw InvalidDataException("GameState", "Number out of range: " + key + "=" + value, "Game::loadGame");
                }
            } else if (currentSection == "GameField") {
                fieldData << line << "\n";
            } else if (currentSection == "Player") {
                playerData << line << "\n";
            } else if (currentSection == "Enemies") {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    
                    try {
                        if (key == "count") {
                            enemyCount = std::stoi(value);
                            if (enemyCount < 0) {
                                throw InvalidDataException("Enemies", "Invalid enemy count: " + std::to_string(enemyCount), "Game::loadGame");
                            }
                        } else if (key.find("enemy_") == 0) {
                            enemyData.push_back(value);
                        }
                    } catch (const InvalidDataException& e) {
                        throw;
                    } catch (const std::invalid_argument& e) {
                        throw InvalidDataException("Enemies", "Invalid number format: " + key + "=" + value, "Game::loadGame");
                    } catch (const std::out_of_range& e) {
                        throw InvalidDataException("Enemies", "Number out of range: " + key + "=" + value, "Game::loadGame");
                    }
                }
            } else if (currentSection == "Allies") {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    
                    try {
                        if (key == "count") {
                            allyCount = std::stoi(value);
                            if (allyCount < 0) {
                                throw InvalidDataException("Allies", "Invalid ally count: " + std::to_string(allyCount), "Game::loadGame");
                            }
                        } else if (key.find("ally_") == 0) {
                            allyData.push_back(value);
                        }
                    } catch (const InvalidDataException& e) {
                        throw;
                    } catch (const std::invalid_argument& e) {
                        throw InvalidDataException("Allies", "Invalid number format: " + key + "=" + value, "Game::loadGame");
                    } catch (const std::out_of_range& e) {
                        throw InvalidDataException("Allies", "Number out of range: " + key + "=" + value, "Game::loadGame");
                    }
                }
            } else if (currentSection == "Buildings") {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    
                    try {
                        if (key == "count") {
                            buildingCount = std::stoi(value);
                            if (buildingCount < 0) {
                                throw InvalidDataException("Buildings", "Invalid building count: " + std::to_string(buildingCount), "Game::loadGame");
                            }
                        } else if (key.find("building_") == 0) {
                            buildingData.push_back(value);
                        }
                    } catch (const InvalidDataException& e) {
                        throw;
                    } catch (const std::invalid_argument& e) {
                        throw InvalidDataException("Buildings", "Invalid number format: " + key + "=" + value, "Game::loadGame");
                    } catch (const std::out_of_range& e) {
                        throw InvalidDataException("Buildings", "Number out of range: " + key + "=" + value, "Game::loadGame");
                    }
                }
            } else if (currentSection == "Towers") {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    
                    try {
                        if (key == "count") {
                            towerCount = std::stoi(value);
                            if (towerCount < 0) {
                                throw InvalidDataException("Towers", "Invalid tower count: " + std::to_string(towerCount), "Game::loadGame");
                            }
                        } else if (key.find("tower_") == 0) {
                            towerData.push_back(value);
                        }
                    } catch (const InvalidDataException& e) {
                        throw;
                    } catch (const std::invalid_argument& e) {
                        throw InvalidDataException("Towers", "Invalid number format: " + key + "=" + value, "Game::loadGame");
                    } catch (const std::out_of_range& e) {
                        throw InvalidDataException("Towers", "Number out of range: " + key + "=" + value, "Game::loadGame");
                    }
                }
            }
        }
        
        // Файл автоматически закроется при выходе из области видимости (RAII)
        
        // Проверка на наличие обязательных секций
        if (!hasGameState) {
            throw CorruptedSaveFileException(filename, "Missing [GameState] section", "Game::loadGame");
        }
        
        if (!hasGameField) {
            throw CorruptedSaveFileException(filename, "Missing [GameField] section", "Game::loadGame");
        }
        
        // Валидация количества объектов (предупреждения, но не критичные ошибки)
        if (enemyCount != static_cast<int>(enemyData.size())) {
            std::cout << "Warning: Enemy count mismatch. Expected " << enemyCount << ", found " << enemyData.size() << std::endl;
            std::cout << "Attempting to load with available data..." << std::endl;
        }
        
        if (allyCount != static_cast<int>(allyData.size())) {
            std::cout << "Warning: Ally count mismatch. Expected " << allyCount << ", found " << allyData.size() << std::endl;
            std::cout << "Attempting to load with available data..." << std::endl;
        }
        
        if (buildingCount != static_cast<int>(buildingData.size())) {
            std::cout << "Warning: Building count mismatch. Expected " << buildingCount << ", found " << buildingData.size() << std::endl;
            std::cout << "Attempting to load with available data..." << std::endl;
        }
        
        if (towerCount != static_cast<int>(towerData.size())) {
            std::cout << "Warning: Tower count mismatch. Expected " << towerCount << ", found " << towerData.size() << std::endl;
            std::cout << "Attempting to load with available data..." << std::endl;
        }
        
        // Загружаем поле (это восстановит все типы клеток: OBSTACLE, SLOWING, EMPTY)
        if (!field->deserialize(fieldData.str())) {
            throw CorruptedSaveFileException(filename, "Failed to deserialize game field", "Game::loadGame");
        }
        
        // Загружаем игрока
        std::string playerStr = playerData.str();
        if (playerStr != "NO_PLAYER\n" && !playerStr.empty()) {
            if (!player) {
                player = std::make_unique<Player>(playerStartX, playerStartY);
            }
            if (!player->deserialize(playerStr)) {
                throw CorruptedSaveFileException(filename, "Failed to deserialize player data", "Game::loadGame");
            }
        } else if (!hasPlayer && playerStr.empty()) {
            std::cout << "Warning: Player section is missing or empty. Creating default player." << std::endl;
            if (!player) {
                player = std::make_unique<Player>(playerStartX, playerStartY);
            }
        }
        
        // Очищаем только объекты на клетках (hasPlayer, hasEnemy, etc.), но НЕ тип клетки
        // Тип клетки (OBSTACLE, SLOWING, EMPTY) уже восстановлен при загрузке поля
        for (int y = 0; y < field->getHeight(); ++y) {
            for (int x = 0; x < field->getWidth(); ++x) {
                // clearPosition() вызывает clearOccupants(), который не трогает тип клетки
                field->clearPosition(x, y);
            }
        }
        
        // Восстанавливаем позицию игрока на поле (делаем это первым)
        if (player) {
            int playerX = player->getPositionX();
            int playerY = player->getPositionY();
            if (field->isValidPosition(playerX, playerY)) {
                field->setPlayerPosition(playerX, playerY);
            } else {
                std::cout << "Warning: Player position (" << playerX << ", " << playerY << ") is invalid. Resetting to start position." << std::endl;
                // Позиция будет установлена через moveTo, но если это не сработает, 
                // то игрок останется на невалидной позиции (это крайний случай)
                if (player->moveTo(playerStartX, playerStartY)) {
                    field->setPlayerPosition(playerStartX, playerStartY);
                } else {
                    throw CorruptedSaveFileException(filename, "Failed to reset player position - game state may be inconsistent", "Game::loadGame");
                }
            }
        }
        
        // Загружаем врагов
        enemies.clear();
        for (const auto& data : enemyData) {
            Enemy enemy(0, 0);
            if (enemy.deserialize(data)) {
                enemies.push_back(enemy);
                // Восстанавливаем позицию на поле (только для живых врагов)
                if (enemy.isAlive()) {
                    int enemyX = enemy.getPositionX();
                    int enemyY = enemy.getPositionY();
                    if (field->isValidPosition(enemyX, enemyY)) {
                        field->setEnemyPosition(enemyX, enemyY);
                    }
                }
            }
        }
        
        // Загружаем союзников
        allies.clear();
        for (const auto& data : allyData) {
            Ally ally(0, 0);
            if (ally.deserialize(data)) {
                allies.push_back(ally);
                // Восстанавливаем позицию на поле (только для живых союзников)
                if (ally.isAlive()) {
                    int allyX = ally.getPositionX();
                    int allyY = ally.getPositionY();
                    if (field->isValidPosition(allyX, allyY)) {
                        field->setAllyPosition(allyX, allyY);
                    }
                }
            }
        }
        
        // Загружаем здания
        buildings.clear();
        for (const auto& data : buildingData) {
            EnemyBuilding building(0, 0, 5);
            if (building.deserialize(data)) {
                buildings.push_back(building);
                // Восстанавливаем позицию на поле (только для неразрушенных зданий)
                if (!building.isDestroyed()) {
                    int buildingX = building.getPositionX();
                    int buildingY = building.getPositionY();
                    if (field->isValidPosition(buildingX, buildingY)) {
                        field->setBuildingPosition(buildingX, buildingY);
                    }
                }
            }
        }
        
        // Загружаем башни
        towers.clear();
        for (const auto& data : towerData) {
            EnemyTower tower(0, 0);
            if (tower.deserialize(data)) {
                towers.push_back(tower);
                // Восстанавливаем позицию на поле (только для неразрушенных башен)
                if (!tower.isDestroyed()) {
                    int towerX = tower.getPositionX();
                    int towerY = tower.getPositionY();
                    if (field->isValidPosition(towerX, towerY)) {
                        field->setBuildingPosition(towerX, towerY);
                    }
                }
            }
        }
        
        std::cout << "Game loaded successfully from " << filename << std::endl;
        return true;
        
    } catch (const CorruptedSaveFileException& e) {
        std::cout << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (const InvalidDataException& e) {
        std::cout << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (const FileLoadException& e) {
        std::cout << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (const GameException& e) {
        std::cout << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (const std::exception& e) {
        std::cout << "Error: Exception occurred while loading game: " << e.what() << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    } catch (...) {
        std::cout << "Error: Unknown exception occurred while loading game." << std::endl;
        // Файл автоматически закроется при выходе из области видимости (RAII)
        return false;
    }
}

void Game::publishScoreChange(int oldScore, int newScore) {
    int scoreChange = newScore - oldScore;
    if (scoreChange != 0) {
        // Публикуем событие изменения счета
        auto scoreEvent = std::make_shared<ScoreChangedEvent>(newScore, scoreChange, turnNumber);
        EventManager::getInstance().publish(scoreEvent);
        
        // Проверяем, было ли добавлено заклинание (через checkSpellReward в addScore)
        // Проверяем, есть ли новое заклинание в hand
        if (player && player->getHand()) {
            size_t spellCount = player->getHand()->getSpellCount();
            if (spellCount > 0) {
                // Получаем последнее заклинание (предполагаем, что оно было добавлено недавно)
                Spell* lastSpell = player->getHand()->getSpell(spellCount - 1);
                if (lastSpell) {
                    // Проверяем, соответствует ли счет порогу получения заклинания (каждые 50 очков)
                    int spellScoreThreshold = (newScore / 50) * 50;
                    if (spellScoreThreshold > (oldScore / 50) * 50 && spellScoreThreshold >= 50) {
                        // Публикуем событие получения заклинания
                        auto spellEvent = std::make_shared<SpellObtainedEvent>(lastSpell->getName(), newScore, turnNumber);
                        EventManager::getInstance().publish(spellEvent);
                    }
                }
            }
        }
    }
}

