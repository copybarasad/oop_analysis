#include "Game.h"
#include "InputReader.h"
#include "GameController.h"
#include "ConsoleRenderer.h"
#include "GameVisualizer.h"

#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include <cctype>

Game::Game()
    : turnCount(0), currentLevel(1), gameRunning(false) {
}

void Game::startGame() {
    std::cout << "=== ДОБРО ПОЖАЛОВАТЬ В ИГРУ! ===\n";

    if (SaveSystem::saveExists()) {
        std::cout << "Обнаружено сохранение игры.\n";
        std::cout << "1. Загрузить сохраненную игру\n";
        std::cout << "2. Начать новую игру\n";
        std::cout << "Выберите действие: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            loadGame();
        } else {
            initializeGame();
        }
    } else {
        initializeGame();
    }

    gameRunning = true;

    InputReader inputReader;
    GameController<InputReader> controller(*this);

    ConsoleRenderer consoleRenderer;
    GameVisualizer<ConsoleRenderer> visualizer(*this);

    while (gameRunning) {
        turnCount++;
        std::cout << "\n=== УРОВЕНЬ " << currentLevel << " | ХОД " << turnCount << " ===\n";

        hand->spellBounty(turnCount);

        visualizer.update();        

        controller.processTurn();   

        if (!gameRunning) break;

        if (checkWinCondition()) {
            showLevelCompleteMenu();
            continue;
        }

        if (checkLoseCondition()) {
            showGameOverMenu();
            continue;
        }

        processEnemiesTurn();

        if (checkLoseCondition()) {
            showGameOverMenu();
            continue;
        }

        processTowersTurn();

        if (checkLoseCondition()) {
            showGameOverMenu();
        }

        std::cout << "----------------------------------------\n";
    }

    std::cout << "\nСпасибо за игру!\n";
}

void Game::movePlayer(char direction) {
    direction = std::toupper(direction);
    if (direction == 'W' || direction == 'A' || direction == 'S' || direction == 'D') {
        player->move(direction, *field);
    }
}

void Game::attackEnemies() {
    player->attack(enemies);
}

void Game::attackTower() {
    auto& towers = field->getTowers();
    if (!towers.empty() && towers[0].isAlive()) {
        player->attackTower(towers[0]);
    }
   
}

void Game::switchPlayerMode() {
    player->switch_attack();
    
}

void Game::useSpell(int spellIndex) {
    if (hand->getSize() == 0 || spellIndex < 1 || spellIndex > hand->getSize()) {
        return;
    }
    hand->useSpell(spellIndex, *player, enemies, *field);
}

void Game::showHelp() const {
    std::cout << "\n=== УПРАВЛЕНИЕ ===\n";
    std::cout << "W/A/S/D — движение\n";
    std::cout << "F — атака врагов\n";
    std::cout << "T — атака башни\n";
    std::cout << "M — сменить режим боя\n";
    std::cout << "Z <номер> — использовать заклинание\n";
    std::cout << "N — сохранить игру\n";
    std::cout << "L — загрузить игру\n";
    std::cout << "H — показать помощь\n";
    std::cout << "Q — выход из игры\n\n";
}

void Game::render() const {
    std::vector<std::pair<int, int>> enemyPositions;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemyPositions.emplace_back(enemy.getX(), enemy.getY());
        }
    }

    field->updateField(player->getX(), player->getY(), enemyPositions);
    field->display();

    if (field->isTrapActive()) {
        std::cout << "На поле активна ловушка!\n";
    }

    std::cout << "=== СТАТУС ИГРОКА ===\n";
    std::cout << "HP: " << player->getHealth()
              << " | Режим: " << (player->isMeleeMode() ? "Ближний бой" : "Дальний бой")
              << " | Урон: " << player->getCurrentDamage() << "\n";

    int aliveEnemies = 0;
    for (const auto& enemy : enemies) if (enemy.isAlive()) aliveEnemies++;
    std::cout << "Врагов осталось: " << aliveEnemies << "/" << enemies.size() << "\n";

    const auto& towers = field->getTowers();
    if (!towers.empty()) {
        const Tower& tower = towers[0];
        std::cout << "Башня: " << (tower.isAlive() ? "ЖИВА (" + std::to_string(tower.getHealth()) + " HP)" : "УНИЧТОЖЕНА") << "\n";
    }

    std::cout << "=== ПРОГРЕСС ===\n";
    std::cout << "Уровень: " << currentLevel << "/5\n";

    hand->showSpells();
}


void Game::initializeGame() {
    currentLevel = 1;
    turnCount = 0;

    field = std::make_unique<Field>(10, 10);

    player = std::make_unique<Player>(1, 1, 100, 20, 15);
    hand = std::make_unique<Hand>(5);

    field->addTower(5, 5, 3, 60);

    enemies.clear();
    enemies = {
        Enemy(2, 2, 40, 8),
        Enemy(7, 3, 40, 8),
        Enemy(4, 7, 40, 8)
    };

    field->generate_block(currentLevel, player->getX(), player->getY(), enemies, field->getTowers());

    std::cout << "\n=== НОВАЯ ИГРА НАЧАЛАСЬ ===\n";
    std::cout << "Уровень 1: Уничтожьте " << enemies.size() << " врагов и башню!\n";
    std::cout << "Управление: H - помощь, N - сохранить игру, L - загрузить игру\n";
}


void Game::nextLevel() {
    currentLevel++;
    turnCount = 0;
    
    std::cout << "\n=== ЗАГРУЗКА УРОВНЯ " << currentLevel << " ===" << std::endl;
    
    int fieldSize = 8 + currentLevel * 2;
    
    field = std::make_unique<Field>(fieldSize, fieldSize);
    
    player = std::make_unique<Player>(1, 1, 100, 20, 15);
    
    if (hand) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int currentSize = hand->getSize();
    int cardsToRemove = currentSize / 2;
    
    for (int i = 0; i < cardsToRemove; ++i) {
        if (hand->getSize() > 0) {
            std::uniform_int_distribution<> dist(1, hand->getSize());
            int randomIndex = dist(gen);
            hand->removeSpell(randomIndex);
            }
        }   
    std::cout << "Удалено " << cardsToRemove << " карточек заклинаний." << std::endl;
    }

    
    int center = fieldSize / 2;
    int towerHealth = 50 + currentLevel * 15;
    int towerRange = 3 + currentLevel / 2;
    field->addTower(center, center, towerRange, towerHealth);
    
    enemies.clear();
    int enemyCount = 2 + currentLevel * 2;
    int enemyHealth = 30 + currentLevel * 10;
    int enemyDamage = 5 + currentLevel * 3;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(2, fieldSize - 2);
    
    for (int i = 0; i < enemyCount; ++i) {
        int x, y;
        bool validPosition = false;
        int attempts = 0;
        
        while (!validPosition && attempts < 20) {
            x = dist(gen);
            y = dist(gen);
            int playerDist = abs(x - 1) + abs(y - 1);
            int towerDist = abs(x - center) + abs(y - center);
            validPosition = (playerDist > 3 && towerDist > 2);
            attempts++;
        }
        
        if (validPosition) {
            enemies.emplace_back(x, y, enemyHealth, enemyDamage);
        }
    }
    
    field->generate_block(currentLevel, player->getX(), player->getY(), enemies, field->getTowers());
    
    std::cout << "Уровень " << currentLevel << " загружен!" << std::endl;
    std::cout << "Врагов: " << enemies.size() << " | Здоровье башни: " << towerHealth << std::endl;
    std::cout << "Поле: " << fieldSize << "x" << fieldSize << std::endl;
}

void Game::showLevelCompleteMenu() {
    std::cout << "\n=== УРОВЕНЬ " << currentLevel << " ПРОЙДЕН! ===" << std::endl;
    std::cout << "Поздравляем! Все враги и башня уничтожены!" << std::endl;
    
    int defeatedEnemies = 0;
    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) defeatedEnemies++;
    }
    std::cout << "Побеждено врагов: " << defeatedEnemies << "/" << enemies.size() << std::endl;
    std::cout << "Ходов затрачено: " << turnCount << std::endl;
    
    player->heal(30);
    std::cout << "Игрок восстановил 30 HP!" << std::endl;
    
    if (currentLevel < 5) {
        std::cout << "\nПерейти на уровень " << (currentLevel + 1) << "? (Y/N): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 'Y' || choice == 'y') {
            nextLevel();
        } else {
            std::cout << "Игра завершена. Вы прошли " << currentLevel << " уровней!" << std::endl;
            SaveSystem::deleteSave();
            gameRunning = false;
        }
    } else {
        std::cout << "\n=== ПОБЕДА! ===" << std::endl;
        std::cout << "Вы прошли все 5 уровней! Игра завершена." << std::endl;
        SaveSystem::deleteSave();
        gameRunning = false;
    }
}

void Game::initializeFromSave(const SaveGame& saveData) {
    turnCount = saveData.turnCount;
    currentLevel = saveData.currentLevel;
    
    std::cout << "Восстанавливаем игру с хода " << turnCount << " уровня " << currentLevel << std::endl;
    
    field = std::make_unique<Field>(saveData.fieldWidth, saveData.fieldHeight, false);
    
    std::cout << "Восстанавливаем " << saveData.walls.size() << " стен..." << std::endl;
    for (const auto& wall : saveData.walls) {
        bool isOccupied = false;
        
        if (wall.x == saveData.playerX && wall.y == saveData.playerY) {
            isOccupied = true;
            std::cout << "Предупреждение: стена на игроке в (" << wall.x << "," << wall.y << ")" << std::endl;
        }
        
        for (const auto& enemyData : saveData.enemies) {
            if (enemyData.alive && wall.x == enemyData.x && wall.y == enemyData.y) {
                isOccupied = true;
                std::cout << "Предупреждение: стена на враге в (" << wall.x << "," << wall.y << ")" << std::endl;
                break;
            }
        }
        
        for (const auto& towerData : saveData.towers) {
            if (towerData.alive && wall.x == towerData.x && wall.y == towerData.y) {
                isOccupied = true;
                std::cout << "Предупреждение: стена на башне в (" << wall.x << "," << wall.y << ")" << std::endl;
                break;
            }
        }
        
        if (!isOccupied && field->valid_pos(wall.x, wall.y)) {
            field->setWall(wall.x, wall.y);
        }
    }
    
    if (saveData.hasActiveTrap) {
        field->setTrap(saveData.trapX, saveData.trapY, saveData.trapDamage);
    }
    
    auto& towers = field->getTowers();
    towers.clear();
    for (const auto& towerData : saveData.towers) {
        std::cout << "Восстанавливаем башню в (" << towerData.x << "," << towerData.y << ")" << std::endl;
        towers.emplace_back(towerData.x, towerData.y, towerData.range, towerData.health, towerData.cooldown);
        Tower& tower = towers.back();
        if (!towerData.alive) {
            tower.takeDamage(towerData.health + 100);
        }
    }
    
    std::cout << "Восстанавливаем игрока в (" << saveData.playerX << "," << saveData.playerY << ")" << std::endl;
    player = std::make_unique<Player>(
        saveData.playerX, saveData.playerY,
        saveData.playerHealth, saveData.playerMeleeDamage, saveData.playerRangedDamage
    );
    if (!saveData.playerAlive) {
        player->takeDamage(player->getHealth() + 100);
    }
    if (!saveData.playerIsMelee) {
        player->switch_attack(); 
    }

    enemies.clear();
    for (const auto& enemyData : saveData.enemies) {
        std::cout << "Восстанавливаем врага в (" << enemyData.x << "," << enemyData.y << ")" << std::endl;
        Enemy enemy(enemyData.x, enemyData.y, enemyData.health, enemyData.damage);
        if (!enemyData.alive) {
            enemy.takeDamage(enemyData.health + 100);
        }
        enemies.push_back(enemy);
    }
    
    hand = std::make_unique<Hand>(5);
    
    std::cout << "\n=== ИГРА ЗАГРУЖЕНА ===" << std::endl;
    std::cout << "Продолжаем с хода " << turnCount << " на уровне " << currentLevel << std::endl;
}

SaveGame Game::createSaveData() const {
    SaveGame data;
    
    data.playerX = player->getX();
    data.playerY = player->getY();
    data.playerHealth = player->getHealth();
    data.playerAlive = player->isAlive();
    data.playerMeleeDamage = 20;
    data.playerRangedDamage = 15;
    data.playerIsMelee = player->isMeleeMode();
    
    data.fieldWidth = field->getWidth();
    data.fieldHeight = field->getHeight();
    data.hasActiveTrap = field->isTrapActive();
    
    data.trapX = 0;
    data.trapY = 0;
    data.trapDamage = 0;
    
    data.currentLevel = currentLevel;
    data.turnCount = turnCount;
    
    for (const auto& enemy : enemies) {
        SaveGame::EnemyData enemyData;
        enemyData.x = enemy.getX();
        enemyData.y = enemy.getY();
        enemyData.health = enemy.getHealth();
        enemyData.damage = enemy.getDamage();
        enemyData.alive = enemy.isAlive();
        data.enemies.push_back(enemyData);
    }
    
    const auto& towers = field->getTowers();
    for (const auto& tower : towers) {
        SaveGame::TowerData towerData;
        towerData.x = tower.getX();
        towerData.y = tower.getY();
        towerData.range = tower.getRange();
        towerData.health = tower.getHealth();
        towerData.cooldown = 3;
        towerData.currentCooldown = 0;
        towerData.alive = tower.isAlive();
        data.towers.push_back(towerData);
    }
    
    data.walls.clear();
    auto allWalls = field->getAllWalls();
    
    for (const auto& wall : allWalls) {
        bool isOccupied = false;
    
        if (wall.first == player->getX() && wall.second == player->getY()) {
            isOccupied = true;
        }
        
        for (const auto& enemy : enemies) {
            if (enemy.isAlive() && wall.first == enemy.getX() && wall.second == enemy.getY()) {
                isOccupied = true;
                break;
            }
        }
        
        for (const auto& tower : towers) {
            if (tower.isAlive() && wall.first == tower.getX() && wall.second == tower.getY()) {
                isOccupied = true;
                break;
            }
        }
        
        if (!isOccupied) {
            SaveGame::WallData wallData;
            wallData.x = wall.first;
            wallData.y = wall.second;
            data.walls.push_back(wallData);
        }
    }
    
    std::cout << "Сохранено стен: " << data.walls.size() << " (фильтровано занятые позиции)" << std::endl;
    
    return data;
}

void Game::saveGame() {
    SaveGame saveData = createSaveData();
    if (SaveSystem::saveGame(saveData)) {
        std::cout << "Игра успешно сохранена! Уровень: " << currentLevel << ", Ход: " << turnCount << std::endl;
    } else {
        std::cout << "Не удалось сохранить игру!" << std::endl;
    }
}

void Game::loadGame() {
    SaveGame saveData;
    if (SaveSystem::loadGame(saveData)) {
        initializeFromSave(saveData);
    } else {
        std::cout << "Не удалось загрузить сохранение. Начинаем новую игру." << std::endl;
        initializeGame();
    }
}

void Game::processEnemiesTurn() {
    std::cout << "\n--- ХОД ВРАГОВ ---" << std::endl;
    
    int activeEnemies = 0;
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.move(*player, *field, enemies);
            activeEnemies++;
            
            if (!player->isAlive()) {
                return;
            }
        }
    }
    
    if (activeEnemies == 0) {
        std::cout << "Все враги уничтожены!" << std::endl;
    }
}

void Game::processTowersTurn() {
    std::cout << "\n--- ХОД БАШЕН ---" << std::endl;
    field->updateTowers(*player, enemies);
}

bool Game::checkWinCondition() const {
    bool allEnemiesDead = true;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            allEnemiesDead = false;
            break;
        }
    }
    
    const auto& towers = field->getTowers();
    bool towerDestroyed = towers.empty() || !towers[0].isAlive();
    
    return allEnemiesDead && towerDestroyed;
}

bool Game::checkLoseCondition() const {
    return !player->isAlive();
}

void Game::showGameOverMenu() {
    std::cout << "\n=== ПОРАЖЕНИЕ ===" << std::endl;
    std::cout << "Игрок погиб на уровне " << currentLevel << "!" << std::endl;
    
    int defeatedEnemies = 0;
    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) defeatedEnemies++;
    }
    std::cout << "Всего ходов: " << turnCount << std::endl;
    
    SaveSystem::deleteSave();
    
    std::cout << "\nХотите начать заново? (Y/N): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice == 'Y' || choice == 'y') {
        initializeGame();
    } else {
        gameRunning = false;
    }
}