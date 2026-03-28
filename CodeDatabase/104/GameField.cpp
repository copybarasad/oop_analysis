#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

// Конструктор игрового поля
GameField::GameField(int width, int height, bool generate_obstacles)
    : width_(width), height_(height), player_(nullptr),
    random_generator_(std::random_device{}()) {
    if (generate_obstacles) {
        generateObstacles();
    }
}
// Перемещение игрока по полю
bool GameField::movePlayer(int target_x, int target_y) {
    if (!player_ || !isPositionValid(target_x, target_y) || !isPassable(target_x, target_y)) {
        return false;
    }

    // Проверка ловушек для игрока
    if (hasTrapAt(target_x, target_y)) {
        player_->takeDamage(30); 
        for (auto it = trap_positions_.begin(); it != trap_positions_.end(); ++it) {
            if (it->getX() == target_x && it->getY() == target_y) {
                trap_positions_.erase(it);
                std::cout << "Player triggered a trap! -30 health\n";
                break;
            }
        }
    }

    player_->setPosition(target_x, target_y);
    return true;
}

// Получение позиции игрока
const Position& GameField::getPlayerPosition() const {
    static Position default_pos(0, 0);
    return player_ ? player_->getPosition() : default_pos;
}

// Проверка валидности позиции
bool GameField::isPositionValid(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

// Проверка занятости позиции
bool GameField::isPositionOccupied(int x, int y) const {
    if (player_ && player_->getPosition().getX() == x && player_->getPosition().getY() == y) {
        return true;
    }

    for (const auto& enemy : enemies_) {
        int enemy_x, enemy_y;
        enemy->GetPosition(enemy_x, enemy_y);
        if (enemy_x == x && enemy_y == y && enemy->IsAlive()) {
            return true;
        }
    }

    for (const auto& tower : towers_) {
        if (tower->getPositionX() == x && tower->getPositionY() == y) {
            return true;
        }
    }

    return false;
}

// Проверка наличия ловушки в позиции
bool GameField::hasTrapAt(int x, int y) const {
    for (const auto& trap : trap_positions_) {
        if (trap.getX() == x && trap.getY() == y) {
            return true;
        }
    }
    return false;
}

// Проверка наличия препятствия в позиции
bool GameField::hasObstacleAt(int x, int y) const {
    for (const auto& obstacle : obstacles_) {
        if (obstacle.getX() == x && obstacle.getY() == y) {
            return true;
        }
    }
    return false;
}

// Проверка проходимости позиции
bool GameField::isPassable(int x, int y) const {
    return isPositionValid(x, y) && !hasObstacleAt(x, y) && !isPositionOccupied(x, y);
}

// Создание врагов на поле
void GameField::spawnEnemies(int count, int base_health, int base_damage) {
    enemies_.clear();
    for (int i = 0; i < count; ++i) {
        Position pos = getRandomEmptyPositionFarFromPlayer(3);
        auto enemy = std::make_unique<Enemy>(base_health, base_damage);
        enemy->SetPosition(pos.getX(), pos.getY());
        enemies_.push_back(std::move(enemy));
    }
}

// Проверка и активация ловушек для врагов
void GameField::checkEnemyTraps() {
    for (auto& enemy : enemies_) {
        if (!enemy->IsAlive()) continue;

        int enemy_x, enemy_y;
        enemy->GetPosition(enemy_x, enemy_y);

        // Проверяем, стоит ли враг на ловушке
        for (auto it = trap_positions_.begin(); it != trap_positions_.end(); ) {
            if (it->getX() == enemy_x && it->getY() == enemy_y) {
                // Враг активировал ловушку! Наносим 30 урона
                enemy->TakeDamage(30);
                std::cout << "Enemy triggered a trap! -30 health\n";

                // Удаляем активированную ловушку
                it = trap_positions_.erase(it);

                // Проверяем, не убила ли ловушка врага
                if (!enemy->IsAlive()) {
                    std::cout << "Trap killed the enemy!\n";
                }
                break; // Враг может активировать только одну ловушку за ход
            }
            else {
                ++it;
            }
        }
    }
}

// Обновляем метод updateEnemies чтобы проверять ловушки
void GameField::updateEnemies() {
    // Сначала двигаем врагов
    for (auto& enemy : enemies_) {
        if (enemy->IsAlive()) {
            moveEnemyTowardsPlayer(*enemy);
        }
    }

    // Затем проверяем, не наступили ли враги на ловушки после движения
    checkEnemyTraps();

    // Затем обрабатываем атаки
    for (auto& enemy : enemies_) {
        if (enemy->IsAlive()) {
            handleEnemyAttack(*enemy);
        }
    }

    // Удаление мертвых врагов
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const std::unique_ptr<Enemy>& enemy) { return !enemy->IsAlive(); }),
        enemies_.end()
    );
}
// Получение количества живых врагов
int GameField::getEnemyCount() const {
    int count = 0;
    for (const auto& enemy : enemies_) {
        if (enemy->IsAlive()) {
            count++;
        }
    }
    return count;
}

// Проверка поражения всех врагов
bool GameField::areAllEnemiesDefeated() const {
    return getEnemyCount() == 0;
}

// Проверка наличия врага в позиции
bool GameField::hasEnemyAt(int x, int y) const {
    for (const auto& enemy : enemies_) {
        int enemy_x, enemy_y;
        enemy->GetPosition(enemy_x, enemy_y);
        if (enemy_x == x && enemy_y == y && enemy->IsAlive()) {
            return true;
        }
    }
    return false;
}

void GameField::spawnTowers(int count) {
    for (int i = 0; i < count; ++i) {
        Position pos = getRandomEmptyPositionFarFromPlayer(2);
        addTower(std::make_unique<EnemyTower>(pos.getX(), pos.getY(), 4, 20));
    }
}

void GameField::updateTowers() {
    for (auto& tower : towers_) {
        tower->update();
        if (player_) {
            tower->tryAttackPlayer(*player_);
        }
    }
}

int GameField::getTowerCount() const {
    return towers_.size();
}

bool GameField::hasTowerAt(int x, int y) const {
    for (const auto& tower : towers_) {
        if (tower->getPositionX() == x && tower->getPositionY() == y) {
            return true;
        }
    }
    return false;
}
// Проверка возможности размещения ловушки
bool GameField::canPlaceTrapAt(int x, int y) const {
    return isPositionValid(x, y) && !isPositionOccupied(x, y) && !hasTrapAt(x, y) && !hasObstacleAt(x, y);
}

// Размещение ловушки на поле
void GameField::placeTrap(const Position& position, int damage) {
    if (canPlaceTrapAt(position.getX(), position.getY())) {
        trap_positions_.push_back(position);
    }
}

// Нанесение урона в указанной позиции
void GameField::damageAtPosition(const Position& position, int damage) {
    int x = position.getX();
    int y = position.getY();
    bool hit_something = false;

    // Поиск врагов в позиции
    for (auto& enemy : enemies_) {
        int enemy_x, enemy_y;
        enemy->GetPosition(enemy_x, enemy_y);
        if (enemy_x == x && enemy_y == y && enemy->IsAlive()) {
            enemy->TakeDamage(damage);
            std::cout << "Enemy hit for " << damage << " damage!\n";
            hit_something = true;
            if (!enemy->IsAlive()) {
                std::cout << "Enemy defeated!\n";
            }
        }
    }

    // Поиск башен в позиции
    for (auto it = towers_.begin(); it != towers_.end(); ) {
        if ((*it)->getPositionX() == x && (*it)->getPositionY() == y) {
            std::cout << "Tower destroyed!\n";
            it = towers_.erase(it);
            hit_something = true;
        }
        else {
            ++it;
        }
    }
}

// Генерация случайной пустой позиции
Position GameField::getRandomEmptyPosition() const {
    std::uniform_int_distribution<int> dist_x(0, width_ - 1);
    std::uniform_int_distribution<int> dist_y(0, height_ - 1);

    Position pos(0, 0);
    int attempts = 0;
    const int max_attempts = 100;

    do {
        int x = dist_x(random_generator_);
        int y = dist_y(random_generator_);
        pos = Position(x, y);
        attempts++;

        if (attempts >= max_attempts) {
            break;
        }
    } while (!isPassable(pos.getX(), pos.getY()));

    return pos;
}

// Генерация позиции вдали от игрока
Position GameField::getRandomEmptyPositionFarFromPlayer(int min_distance) const {
    std::uniform_int_distribution<int> dist_x(0, width_ - 1);
    std::uniform_int_distribution<int> dist_y(0, height_ - 1);

    Position player_pos = getPlayerPosition();
    Position pos(0, 0);
    int attempts = 0;
    const int max_attempts = 200;

    do {
        int x = dist_x(random_generator_);
        int y = dist_y(random_generator_);
        pos = Position(x, y);
        attempts++;

        if (attempts >= max_attempts) {
            return getRandomEmptyPosition();
        }
    } while (!isPassable(pos.getX(), pos.getY()) ||
        pos.isWithinRange(player_pos, min_distance - 1));

    return pos;
}

// Генерация препятствий на поле
void GameField::generateObstacles() {
    obstacles_.clear();
    std::uniform_int_distribution<int> dist_x(0, width_ - 1);
    std::uniform_int_distribution<int> dist_y(0, height_ - 1);

    int obstacle_count = (width_ * height_) / 10;

    for (int i = 0; i < obstacle_count; ++i) {
        Position pos = getRandomEmptyPosition();
        Position center(width_ / 2, height_ / 2);
        if (!pos.isWithinRange(center, 2)) {
            obstacles_.push_back(pos);
        }
    }
}

// Движение врага к игроку
void GameField::moveEnemyTowardsPlayer(Enemy& enemy) {
    if (!player_) return;

    int enemy_x, enemy_y;
    enemy.GetPosition(enemy_x, enemy_y);
    Position player_pos = getPlayerPosition();

    int dx = 0, dy = 0;
    if (player_pos.getX() > enemy_x) dx = 1;
    else if (player_pos.getX() < enemy_x) dx = -1;

    if (player_pos.getY() > enemy_y) dy = 1;
    else if (player_pos.getY() < enemy_y) dy = -1;

    int new_x = enemy_x + dx;
    int new_y = enemy_y + dy;

    if (isPassable(new_x, new_y)) {
        enemy.SetPosition(new_x, new_y);
    }
}

// Атака врага на игрока
void GameField::handleEnemyAttack(Enemy& enemy) {
    if (!player_) return;

    int enemy_x, enemy_y;
    enemy.GetPosition(enemy_x, enemy_y);
    Position player_pos = getPlayerPosition();

    if (std::abs(enemy_x - player_pos.getX()) <= 1 &&
        std::abs(enemy_y - player_pos.getY()) <= 1) {
        player_->takeDamage(enemy.GetDamage());
        std::cout << "Enemy attacked you for " << enemy.GetDamage() << " damage!\n";
    }
}