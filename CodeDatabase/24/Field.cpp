#include "Field.hpp"
#include "EnemyManager.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>

// Инициализация игровой сетки
void Field::initializeGrid() {
    // Инициализация пустого поля
    grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    
    // Добавление препятствий по сетке
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Пропускаем стартовую позицию игрока
            if (x == playerPos.getX() && y == playerPos.getY()) {
                continue;
            }
            
            // 5% вероятность создания препятствия
            if (dis(gen) < 0.05) {
                grid[y][x] = CellType::OBSTACLE;
            }
        }
    }
}

// Методы для загрузки игры

// Установка позиции игрока
void Field::setPlayerPosition(const Pos& newPos) {
    if (!isValidPosition(newPos)) {
        throw std::invalid_argument("Invalid player position");
    }
    playerPos = newPos;
}

// Установка типа клетки
void Field::setCellType(const Pos& pos, CellType type) {
    if (!isValidPosition(pos)) {
        throw std::invalid_argument("Invalid position for setting cell type");
    }
    grid[pos.getY()][pos.getX()] = type;
}

// Очистка врагов
void Field::clearEnemies() {
    enemyManager = std::make_unique<EnemyManager>();
}

// Изменение размера сетки
void Field::resizeGrid(int newWidth, int newHeight) {
    if (!Config::Constraints::validateFieldSize(newWidth) || !Config::Constraints::validateFieldSize(newHeight)) {
        throw std::invalid_argument("Invalid field size for resize");
    }
    
    width = newWidth;
    height = newHeight;
    
    // Пересоздаем сетку с новыми размерами
    grid.clear();
    initializeGrid();
}

// Перемещение игрока
bool Field::movePlayer(const Pos& direction) {
    Pos newPos = playerPos + direction;

    // Проверка возможности перемещения
    if (!isValidPosition(newPos)) {
        std::cout << "Cannot move: out of bounds." << std::endl;
        return true;
    }

    if (!isCellPassable(newPos)) {
        std::cout << "Cannot move: cell is impassable." << std::endl;
        return true;
    }

    // Проверка на столкновение с врагом
    if (enemyManager->getEnemyAt(newPos)) {
        std::cout << "Cannot move: cell is blocked by an enemy." << std::endl;
        return true;
    }

    playerPos = newPos;
    std::cout << "Player moved to (" << newPos.getX() << ", " << newPos.getY() << ")" << std::endl;
    return true;
}

// Перемещение врагов
void Field::moveEnemies() {
    // EnemyManager вычисляет куда хотят двигаться враги
    auto proposals = enemyManager->calculateMoveProposals(playerPos);
    
    std::vector<Pos> movedEnemies;

    // Field решает могут ли они туда двигаться и выполняет действия
    for (const auto& proposal : proposals) {
        // Проверяем, перемещался ли уже этот враг
        bool alreadyMoved = false;
        for (const auto& movedPos : movedEnemies) {
            if (movedPos == proposal.enemyCurrentPos) {
                alreadyMoved = true;
                break;
            }
        }
        if (alreadyMoved) continue;

        Enemy* enemy = enemyManager->getEnemyAt(proposal.enemyCurrentPos);
        if (!enemy) continue;
        
        if (proposal.shouldAttackPlayer) {
            // Атака игрока
            int dmg = enemy->getDamage();
            player.takeDamage(dmg);
            movedEnemies.push_back(proposal.enemyCurrentPos);
            std::cout << "Enemy attacks: player takes " << dmg << " damage." << std::endl;
        } else {
            // Проверяем возможность перемещения
            if (isValidPosition(proposal.suggestedNewPos) && 
                isCellPassable(proposal.suggestedNewPos) && 
                !enemyManager->getEnemyAt(proposal.suggestedNewPos)) {
                
                // Перемещаем врага
                enemyManager->updateEnemyPosition(proposal.enemyCurrentPos, proposal.suggestedNewPos);

                movedEnemies.push_back(proposal.enemyCurrentPos);
            }
        }
    }
    
    // Удаляем мертвых врагов
    enemyManager->removeDeadEnemies();
}

// Добавление врага на поле
void Field::addEnemy(std::unique_ptr<Enemy> enemy, const Pos& position) {
    // Проверка возможности добавления врага
    if (!isValidPosition(position) || !isCellPassable(position) || enemyManager->getEnemyAt(position) || position == playerPos) {
        throw std::invalid_argument("Cannot add enemy at this position");
    }
    
    // Проверка параметров врага
    if (!Config::Constraints::validateHealth(enemy->getHealth()) || 
        !Config::Constraints::validateDamage(enemy->getDamage())) {
        throw std::invalid_argument("Enemy parameters exceed config constraints");
    }
    
    enemyManager->addEnemy(std::move(enemy), position);
}

// Получение списка врагов с их позициями
std::vector<std::pair<Enemy, Pos>> Field::getEnemiesWithPositions() const {
    std::vector<std::pair<Enemy, Pos>> result;
    const auto& enemies = enemyManager->getEnemies();
    for (const auto& enemyPair : enemies) {
        result.emplace_back(*enemyPair.first, enemyPair.second);
    }
    return result;
}

// Проверка валидности позиции
bool Field::isValidPosition(const Pos& pos) const {
    return pos.isValid(width, height);
}

// Проверка проходимости клетки
bool Field::isCellPassable(const Pos& pos) const {
    if (!isValidPosition(pos)) return false;
    return grid[pos.getY()][pos.getX()] != CellType::OBSTACLE;
}

// Проверка наличия врага в позиции
bool Field::isEnemyAt(const Pos& pos) const {
    return enemyManager->getEnemyAt(pos) != nullptr;
}

// Геттеры
int Field::getWidth() const { return width; }
int Field::getHeight() const { return height; }

CellType Field::getCellType(const Pos& pos) const { 
    return isValidPosition(pos) ? grid[pos.getY()][pos.getX()] : CellType::OBSTACLE; 
}

Pos Field::getPlayerPosition() const { return playerPos; }
const Player& Field::getPlayer() const { return player; }
Player& Field::getPlayer() { return player; }

// Получение врага по позиции — фасад на enemyManager
Enemy* Field::getEnemyAt(const Pos& pos) const {
    return enemyManager->getEnemyAt(pos);
}

// Удаление мертвых врагов — фасад
void Field::removeDeadEnemies() {
    enemyManager->removeDeadEnemies();
}