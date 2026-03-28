#include "GameController.h"
#include <cmath> // для sqrt


GameController::GameController(Player& player,
    Hand& hand,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<std::unique_ptr<SpawnerTower>>& towers,
    unsigned int seed,
    int levelIndex,
    EventBus* bus): 
    _player(player),
    _hand(hand),
    _enemies(enemies),
    _towers(towers),
    _seed(seed),
    _levelIndex(levelIndex),
    _bus(bus){
        static bool initialized = false;
        if (!initialized) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        initialized = true;
        }
}

bool GameController::startLevel() {
    

    // Очищаем только то, чем владеет контроллер: поле
    _field.reset();

    // Определяем размер поля и seed
    int size = 10 + _levelIndex;

    _field = std::make_unique<GameField>(size, _seed);
    _seed = _field->getSeed();

    if (_enemies.empty() && _towers.empty()) {

        generateDefaultLevelContent();

        if (_hand.size() > 1) {
            size_t half_size = _hand.size() / 2;
            while (_hand.size() > half_size) {
                _hand.getSpell(0)->consume();
                _hand.removeUsedSpells();
            }
        } else if (_hand.size() == 0) {
            _hand.addRandomSpell();
        }

        Position playerPos(_field->getRowCount() / 2, _field->getColumnCount() / 2);
        _player.setPosition(playerPos);
    } else {
        // Просто расставляем по полю то, что уже есть
        int rows = _field->getRowCount();
        int cols = _field->getColumnCount();

        // Враги
        for (auto& enemyPtr : _enemies) {
            if (!enemyPtr) continue;
            Position pos = enemyPtr->getPosition();
            if (!pos.isValid(rows, cols)) continue;
            _field->getCell(pos).setOccupant(enemyPtr.get());
        }

        // Башни
        for (auto& towerPtr : _towers) {
            if (!towerPtr) continue;
            Position pos = towerPtr->getPosition();
            if (!pos.isValid(rows, cols)) continue;
            _field->getCell(pos).setOccupant(towerPtr.get());
        }
    }

    _field->getCell(_player.getPosition()).setOccupant(&_player);
    //_field->display();
    return true;
}

// Генерация врагов и башен по умолчанию (новый уровень, без сохранения)
void GameController::generateDefaultLevelContent() {
    if (!_field) return;

    int size = _field->getRowCount();

    int enemyCount = 5 + (_levelIndex - 1);
    int enemyHP = 20 + 3 * (_levelIndex - 1);
    int enemyDamage = 10 + 3 * (_levelIndex - 1);

    // Враги
    for (int i = 0; i < enemyCount; ++i) {
        auto enemy = std::make_unique<Enemy>(enemyHP, enemyDamage);

        int x = std::rand() % size;
        int y = std::rand() % size;

        Position pos(x, y);
        enemy->setPosition(pos);
        _field->getCell(pos).setOccupant(enemy.get());
        _enemies.push_back(std::move(enemy));
    }

    // Одна башня по умолчанию
    auto tower = std::make_unique<SpawnerTower>(3);
    Position towerPos(size / 3, size / 3);
    tower->setPosition(towerPos);
    _field->getCell(towerPos).setOccupant(tower.get());
    _towers.push_back(std::move(tower));
}


std::vector<Enemy*> GameController::getEnemiesInRadius(const Position& center, int radius) {
    std::vector<Enemy*> result;

    int cx = center.getRow();
    int cy = center.getColumn();

    for (auto& enemyPtr : _enemies) {
        if (!enemyPtr) continue;

        int ex = enemyPtr->getPosition().getRow();
        int ey = enemyPtr->getPosition().getColumn();

        int dx = ex - cx;
        int dy = ey - cy;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= radius) {
            result.push_back(enemyPtr.get());
        }
    }

    return result;
}

//движение игрока
void GameController::turnPlayer(Direction dir) {
    // Двигаем игрока, если возможно
    _field->moveUnit(&_player, dir);
    //_field->display();
    _player.addScore(cleanupDeadUnits()*100);
    std::cout << "\n";
}
//атака или заклинание
void GameController::turnPlayer(const Position& targetPos, const std::string& actionType, int i_spell) {
    if (actionType == "Far") {
        _field->getCell(targetPos).getOccupant()->takeDamage(_player.get_damage());
    }
    else if (actionType == "DS") {
        _field->getCell(targetPos).getOccupant()->takeDamage(_hand.getSpell(i_spell)->damage());
        _hand.getSpell(i_spell)->consume();
        _hand.removeUsedSpells();

    }
    else if (actionType == "AS") {
        auto enemiesInArea = getEnemiesInRadius(_player.getPosition(), _hand.getSpell(i_spell)->radius());
        for (auto* e : enemiesInArea) {
            e->takeDamage(_hand.getSpell(i_spell)->damage());
        }
        _hand.getSpell(i_spell)->consume();
        _hand.removeUsedSpells();
    }

    _player.addScore(cleanupDeadUnits()*100);
    //_field->display();
    std::cout << "\n";
}

//убираем мёртвых юнитов
int GameController::cleanupDeadUnits() {
    int removedCount = 0;

    if (!_field) return 0;

    // ---  Удаляем мёртвых врагов ---
    for (size_t i = 0; i < _enemies.size(); ) {
        Enemy* e = _enemies[i].get();
        if (e == nullptr || e->get_health() <= 0) {
            if (e) {
                Position pos = e->getPosition();
                auto& cell = _field->getCell(pos);
                if (cell.getOccupant() == e) {
                    cell.setOccupant(nullptr);
                }
            }
            _enemies.erase(_enemies.begin() + i);
            ++removedCount;
        } else {
            ++i;
        }
    }

    // --- Удаляем разрушенные башни 
    for (size_t i = 0; i < _towers.size(); ) {
        SpawnerTower* t = _towers[i].get();
        if (t == nullptr || t->get_health() <= 0) {
            if (t) {
                Position pos = t->getPosition();
                auto& cell = _field->getCell(pos);
                if (cell.getOccupant() == t) {
                    cell.setOccupant(nullptr);
                }
            }
            _towers.erase(_towers.begin() + i);
            ++removedCount;
        } else {
            ++i;
        }
    }

    return removedCount;
}
//ход врагов
void GameController::turnEnemies() {
    if (_enemies.empty() || !_field) return;

    // Перебираем всех врагов
    for (auto& enemyPtr : _enemies) {
        Enemy* enemy = enemyPtr.get();
        if (!enemy || enemy->get_health() <= 0) continue;

        Position ePos = enemy->getPosition();
        Position pPos = _player.getPosition();

        // Проверяем — находится ли игрок на соседней клетке (по 4 направлениям)
        bool playerAdjacent =
            (std::abs(ePos.getRow() - pPos.getRow()) == 1 && ePos.getColumn() == pPos.getColumn()) ||
            (std::abs(ePos.getColumn() - pPos.getColumn()) == 1 && ePos.getRow() == pPos.getRow());

        if (playerAdjacent) {
            // Игрок рядом — враг атакует
            _player.takeDamage(enemy->get_damage());
            std::cout << "Враг атаковал игрока!\n";
            std::cout << "HP врага " << enemy->get_health() << "\n";
            std::cout << "Позиция врага (" << enemy->getPosition().getRow() << ", " << enemy->getPosition().getColumn() << ")\n";
            continue; // враг остаётся на месте
        }

        // Иначе — выбираем случайное направление
        int dirIndex = std::rand() % 4;
        Direction dir = Direction::UP;

        switch (dirIndex) {
            case 0: dir = Direction::UP; break;
            case 1: dir = Direction::DOWN; break;
            case 2: dir = Direction::LEFT; break;
            case 3: dir = Direction::RIGHT; break;
        }

        // Двигаем врага на поле
        _field->moveUnit(enemy, dir);
    }
}

//ход башни
void GameController::turnTowers() {
    if (_towers.empty() || !_field) return;

    int Row = _field->getRowCount();
    int Column = _field->getColumnCount(); 
    for (auto& towerPtr : _towers) {
        SpawnerTower* tower = towerPtr.get();
        if (!tower || !tower->isAlive()) continue;

        // 1️⃣ обновляем кулдаун
        tower->tick();
        std::cout << "Tower tcik: " << tower->get_currentCooldown() << std::endl;
        // 2️⃣ если башня готова спавнить
        if (tower->canSpawn()) {
            Position tPos = tower->getPosition();

            // 3️⃣ пробуем найти первую свободную соседнюю клетку
            std::vector<Position> nearby = {
                {tPos.getRow() - 1, tPos.getColumn()}, // вверх
                {tPos.getRow() + 1, tPos.getColumn()}, // вниз
                {tPos.getRow(), tPos.getColumn() - 1}, // влево
                {tPos.getRow(), tPos.getColumn() + 1}  // вправо
            };

            for (auto& pos : nearby) {
                // проверяем, что координаты в пределах поля
                if (pos.getRow() < 0 || pos.getColumn() < 0 ||
                    pos.getRow() >= Row || pos.getColumn() >= Column)
                    continue;

                auto& cell = _field->getCell(pos);
                if (cell.getOccupant() == nullptr && cell.getType() == CellType::EMPTY) {
                    // создаём нового врага
                    int enemyHP = 10 + 3 * _levelIndex;
                    int enemyDamage = 20 + 3 * _levelIndex;

                    auto enemy = std::make_unique<Enemy>(enemyHP, enemyDamage);
                    enemy->setPosition(pos);
                    cell.setOccupant(enemy.get());
                    _enemies.push_back(std::move(enemy));
                    break;
                }
            }

            // 4️⃣ сбрасываем кулдаун в любом случае
            tower->resetCooldown();

            // можно добавить лог: если spawned == false, то просто “пропуск спавна”
        }
    }
}
