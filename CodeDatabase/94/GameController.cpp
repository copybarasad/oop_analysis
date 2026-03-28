#include "GameController.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА GAMECONTROLLER (ГЛАВНЫЙ КОНТРОЛЛЕР ИГРЫ)
// ============================================================================

// Конструктор игрового контроллера
GameController::GameController(int fieldWidth, int fieldHeight) 
    : field(std::unique_ptr<GameField>(new GameField(fieldWidth, fieldHeight))),
      currentState(GameState::PLAYING),
      nextSpellThreshold(30),
      awaitingSpellTarget(false),
      selectedSpellIndex(-1) {
    
    std::unique_ptr<Player> playerPtr(new Player(100, 15, 10));
    player = playerPtr.get();
    
    Position startPos = field->findRandomEmptyPosition();
    field->addCharacter(startPos, std::move(playerPtr));
    player->setPosition(startPos);
    
    giveRandomSpell();
    
    for (int i = 0; i < 6; ++i) {
        Position enemyPos = field->findRandomEmptyPosition();
        std::unique_ptr<Enemy> enemy(new Enemy(30, 10));
        field->addCharacter(enemyPos, std::move(enemy));
    }
    
    Position towerPos = field->findRandomEmptyPosition();
    EnemyTower tower(towerPos);
    field->addTower(tower);
}

// Деструктор
GameController::~GameController() {
}

// Конструктор копирования
GameController::GameController(const GameController& other) 
    : field(std::unique_ptr<GameField>(new GameField(*other.field))),
      currentState(other.currentState),
      nextSpellThreshold(other.nextSpellThreshold),
      awaitingSpellTarget(other.awaitingSpellTarget),
      selectedSpellIndex(other.selectedSpellIndex) {
    
    // Восстанавливаем указатель на игрока после копирования поля
    player = dynamic_cast<Player*>(const_cast<Character*>(
        field->viewCharacterAt(field->getPlayerPosition())));
}

// Конструктор перемещения
GameController::GameController(GameController&& other) noexcept
    : field(std::move(other.field)),
      player(other.player),
      currentState(other.currentState),
      nextSpellThreshold(other.nextSpellThreshold),
      awaitingSpellTarget(other.awaitingSpellTarget),
      selectedSpellIndex(other.selectedSpellIndex) {
    
    other.player = nullptr; // Обнуляем указатель в исходном объекте
}

// Оператор присваивания копированием
GameController& GameController::operator=(const GameController& other) {
    if (this != &other) {
        field = std::unique_ptr<GameField>(new GameField(*other.field));
        player = dynamic_cast<Player*>(const_cast<Character*>(
            field->viewCharacterAt(field->getPlayerPosition())));
        currentState = other.currentState;
        nextSpellThreshold = other.nextSpellThreshold;
        awaitingSpellTarget = other.awaitingSpellTarget;
        selectedSpellIndex = other.selectedSpellIndex;
    }
    return *this;
}

// Оператор присваивания перемещением
GameController& GameController::operator=(GameController&& other) noexcept {
    if (this != &other) {
        field = std::move(other.field);
        player = other.player;
        currentState = other.currentState;
        nextSpellThreshold = other.nextSpellThreshold;
        awaitingSpellTarget = other.awaitingSpellTarget;
        selectedSpellIndex = other.selectedSpellIndex;
        
        other.player = nullptr;
    }
    return *this;
}

// Обработка движения игрока
bool GameController::movePlayer(int deltaX, int deltaY) {
    if (currentState != GameState::PLAYING) {
        return false; // Игра не в активном состоянии
    }
    
    // Если ожидается выбор цели для заклинания, обрабатываем как выбор цели
    if (awaitingSpellTarget) {
        return castSpellAtTarget(deltaX, deltaY);
    }
    
    Position currentPos = field->getPlayerPosition();
    Position newPos(currentPos.x() + deltaX, currentPos.y() + deltaY);
    
    // Проверяем что новая позиция проходима
    if (!field->isPositionPassable(newPos)) {
        return false;
    }
    
    // Логика дальней атаки в режиме RANGED
    if (player->getCombatMode() == CombatMode::RANGED) {
        for (int distance = 1; distance <= 2; ++distance) {
            Position attackPos(currentPos.x() + deltaX * distance, currentPos.y() + deltaY * distance);
            // Проверяем валидность позиции и проходимость
            if (!field->isValidPosition(attackPos) || !field->isPositionPassable(attackPos)) {
                break;
            }
            // Если на позиции есть враг - атакуем его
            if (field->hasCharacterAt(attackPos)) {
                const Character* characterAtPos = field->viewCharacterAt(attackPos);
                if (characterAtPos && characterAtPos != player) {
                    attackEnemyAt(attackPos);
                    return true;
                }
            }
        }
    }
    
    // Логика ближней атаки и движения
    if (field->hasCharacterAt(newPos)) {
        const Character* characterAtNewPos = field->viewCharacterAt(newPos);
        if (characterAtNewPos && characterAtNewPos != player) {
            attackEnemyAt(newPos);
            return true;
        }
    }
    
    // Перемещаем игрока если нет препятствий
    if (field->movePlayer(newPos)) {
        player->setPosition(newPos);
        return true;
    }
    
    return false;
}

// Атака врага в указанной позиции
void GameController::attackEnemyAt(const Position& pos) {
    const Character* character = field->viewCharacterAt(pos);
    if (character && character != player) {
        Character* enemy = const_cast<Character*>(character);
        enemy->takeDamage(player->getCurrentDamage());
        
        // Враги не контратакуют при атаке игрока на расстоянии
        // Только если игрок находится в смежной клетке
        
        // Если враг убит - удаляем его и начисляем очки
        if (!enemy->isAlive()) {
            field->removeCharacter(pos);
            player->addScore(10);
            
            // Проверяем не пора ли дать новое заклинание
            if (player->getScore() >= nextSpellThreshold) {
                giveRandomSpell();
                nextSpellThreshold += 15;
            }
        }
    }
}

// Переключение режима боя игрока
bool GameController::switchCombatMode() {
    if (currentState == GameState::PLAYING && !awaitingSpellTarget) {
        player->switchCombatMode();
        return true;
    }
    return false;
}

// Использование заклинания по индексу
bool GameController::useSpell(int spellIndex) {
    if (currentState != GameState::PLAYING || awaitingSpellTarget) {
        return false;
    }
    
    Spell* spell = player->getHand().getSpell(spellIndex);
    if (!spell) {
        return false; // Заклинание не найдено
    }
    
    if (spell->requiresTarget()) {
        // Для заклинаний с выбором цели - переходим в режим выбора
        awaitingSpellTarget = true;
        selectedSpellIndex = spellIndex;
        return true; // Ждем выбора цели
    } else {
        // Для заклинаний без цели применяем сразу
        bool result = spell->cast(Position(0, 0), *field, *player);
        
        // Удаляем заклинание после использования (расходуемое)
        if (result) {
            player->getHand().removeSpell(spellIndex);
            update();
            handleEnemyTurns();
        }
        return result;
    }
}

// Применение заклинания с выбором цели
bool GameController::castSpellAtTarget(int deltaX, int deltaY) {
    if (!awaitingSpellTarget) {
        return false;
    }
    
    Spell* spell = player->getHand().getSpell(selectedSpellIndex);
    if (!spell) {
        awaitingSpellTarget = false;
        selectedSpellIndex = -1;
        return false;
    }
    
    // Получаем целевую позицию на основе направления
    Position target = getSpellTargetFromDirection(deltaX, deltaY);
    
    // Проверяем валидность целевой позиции
    if (!field->isValidPosition(target)) {
        awaitingSpellTarget = false;
        selectedSpellIndex = -1;
        return false;
    }
    
    // Применяем заклинание
    bool result = spell->cast(target, *field, *player);
    
    // Удаляем заклинание после использования
    if (result) {
        player->getHand().removeSpell(selectedSpellIndex);
    }
    
    // Сбрасываем состояние выбора цели
    awaitingSpellTarget = false;
    selectedSpellIndex = -1;
    
    if (result) {
        update();
        handleEnemyTurns();
    }
    
    return result;
}

// Отмена выбора цели для заклинания
void GameController::cancelSpellTarget() {
    awaitingSpellTarget = false;
    selectedSpellIndex = -1;
}

// Получение целевой позиции для заклинания на основе направления
Position GameController::getSpellTargetFromDirection(int deltaX, int deltaY) const {
    Position playerPos = field->getPlayerPosition();
    Spell* spell = player->getHand().getSpell(selectedSpellIndex);
    
    if (spell && (deltaX != 0 || deltaY != 0)) {
        int range = spell->getRange();
        
        // Применяем дальность к направлению (умножаем на дальность заклинания)
        int targetX = playerPos.x() + deltaX * range;
        int targetY = playerPos.y() + deltaY * range;
        
        // Ограничиваем границами поля
        targetX = std::max(0, std::min(field->getWidth() - 1, targetX));
        targetY = std::max(0, std::min(field->getHeight() - 1, targetY));
        
        return Position(targetX, targetY);
    }
    
    return playerPos;
}

// Выдача случайного заклинания игроку
void GameController::giveRandomSpell() {
    if (player->getHand().isFull()) {
        return; // Рука заполнена
    }
    
    // Генерация случайного числа для выбора заклинания
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 3); // 4 заклинания
    
    std::unique_ptr<Spell> spell;
    switch (dist(gen)) {
        case 0:
            spell = std::unique_ptr<Spell>(new HealSpell());
            break;
        case 1:
            spell = std::unique_ptr<Spell>(new DirectDamageSpell(30, 3));
            break;
        case 2:
            spell = std::unique_ptr<Spell>(new AreaDamageSpell());
            break;
        case 3:
            spell = std::unique_ptr<Spell>(new TrapSpell());
            break;
    }
    
    if (spell) {
        player->addSpell(std::move(spell));
    }
}

// Обновление состояния игры
void GameController::update() {
    // Проверяем жив ли игрок
    if (!player->isAlive()) {
        currentState = GameState::PLAYER_DIED;
        return;
    }
    
    // Обновляем кулдауны башен
    field->updateTowers();
    
    // Проверяем условие победы - все враги побеждены
    bool enemiesRemain = false;
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            Position pos(x, y);
            if (field->hasCharacterAt(pos)) {
                const Character* character = field->viewCharacterAt(pos);
                if (character && dynamic_cast<const Enemy*>(character) && character->isAlive()) {
                    enemiesRemain = true;
                    break;
                }
            }
        }
        if (enemiesRemain) break;
    }
    
    if (!enemiesRemain) {
        currentState = GameState::VICTORY;
    }
}

// Обработка ходов врагов
void GameController::handleEnemyTurns() {
    handleEnemyAttacks();
    handleEnemyMovement();
    handleTowerAttacks();
}

// Обработка атак вражеских башен
void GameController::handleTowerAttacks() {
    Position playerPos = field->getPlayerPosition();
    
    for (const auto& tower : field->getTowers()) {
        if (tower.canAttack()) {
            // Проверяем находится ли игрок в радиусе атаки (Манхэттенское расстояние)
            int dx = std::abs(playerPos.x() - tower.getPosition().x());
            int dy = std::abs(playerPos.y() - tower.getPosition().y());
            int distance = dx + dy; // Манхэттенское расстояние
            
            if (distance <= tower.getAttackRange()) {
                player->takeDamage(tower.getDamage());
                std::cout << "Tower hit you for " << tower.getDamage() << " damage!\n";
                const_cast<EnemyTower&>(tower).attack();
                
                if (!player->isAlive()) {
                    currentState = GameState::PLAYER_DIED;
                    return;
                }
            }
        }
    }
}

// Обработка атак врагов в ближнем бою
void GameController::handleEnemyAttacks() {
    Position playerPos = field->getPlayerPosition();
    std::vector<Position> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    // Собираем всех врагов на поле
    std::vector<std::pair<Position, Enemy*>> enemies;
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            Position pos(x, y);
            const Character* character = field->viewCharacterAt(pos);
            Enemy* enemy = dynamic_cast<Enemy*>(const_cast<Character*>(character));
            
            if (enemy && character != player && enemy->isAlive()) {
                enemies.push_back({pos, enemy});
            }
        }
    }
    
    // Обрабатываем атаки каждого врага
    for (auto& enemyData : enemies) {
        Position pos = enemyData.first;
        Enemy* enemy = enemyData.second;
        
        // Проверяем находится ли враг рядом с игроком
        bool isNearPlayer = false;
        for (const auto& dir : directions) {
            Position checkPos = pos + dir;
            if (checkPos == playerPos) {
                isNearPlayer = true;
                break;
            }
        }
        
        // Если враг рядом - он атакует игрока
        if (isNearPlayer) {
            player->takeDamage(enemy->getDamage());
            std::cout << "Enemy hit you for " << enemy->getDamage() << " damage!\n";
            
            if (!player->isAlive()) {
                currentState = GameState::PLAYER_DIED;
                return;
            }
        }
    }
}

// Обработка движения врагов
void GameController::handleEnemyMovement() {
    // Собираем позиции всех врагов
    std::vector<Position> enemyPositions;
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            Position pos(x, y);
            const Character* character = field->viewCharacterAt(pos);
            Enemy* enemy = dynamic_cast<Enemy*>(const_cast<Character*>(character));
            if (enemy && character != player && enemy->isAlive()) {
                enemyPositions.push_back(pos);
            }
        }
    }
    
    // Обрабатываем движение каждого врага
    for (const auto& enemyPos : enemyPositions) {
        const Character* character = field->viewCharacterAt(enemyPos);
        Enemy* enemy = dynamic_cast<Enemy*>(const_cast<Character*>(character));
        
        if (enemy && character != player && enemy->isAlive()) {
            Position playerPos = field->getPlayerPosition();
            std::vector<Position> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            bool isNearPlayer = false;
            
            // Проверяем не находится ли враг уже рядом с игроком
            for (const auto& dir : directions) {
                Position checkPos = enemyPos + dir;
                if (checkPos == playerPos) {
                    isNearPlayer = true;
                    break;
                }
            }
            
            // Если враг не рядом с игроком - двигаем его
            if (!isNearPlayer) {
                tryMoveEnemy(enemyPos);
            }
        }
    }
}

// Попытка перемещения врага
bool GameController::tryMoveEnemy(const Position& enemyPos) {
    const Character* character = field->viewCharacterAt(enemyPos);
    Enemy* enemy = dynamic_cast<Enemy*>(const_cast<Character*>(character));
    
    if (!enemy || character == player || !enemy->isAlive()) {
        return false;
    }
    
    Position playerPos = field->getPlayerPosition();
    Position newPos = calculateEnemyMove(enemyPos, playerPos);
    
    // Проверяем что движение только на одну клетку
    int dx = std::abs(newPos.x() - enemyPos.x());
    int dy = std::abs(newPos.y() - enemyPos.y());
    
    if (dx > 1 || dy > 1) {
        return false;
    }
    
    return field->moveCharacter(enemyPos, newPos);
}

// Расчет движения врага (ИИ)
Position GameController::calculateEnemyMove(const Position& enemyPos, const Position& playerPos) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 3);
    
    // Сначала проверяем соседние клетки на наличие ловушек
    std::vector<Position> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    // Приоритет 1: двигаться к ловушкам (враги наступают на ловушки)
    for (const auto& dir : directions) {
        Position trapPos = enemyPos + dir;
        if (field->isValidPosition(trapPos) && field->hasTrapAt(trapPos) && 
            !field->hasCharacterAt(trapPos) && !field->hasTowerAt(trapPos)) {
            return trapPos; // Наступаем на ловушку
        }
    }
    
    // Приоритет 2: двигаться к игроку
    int deltaX = 0, deltaY = 0;
    
    if (enemyPos.x() < playerPos.x()) deltaX = 1;
    else if (enemyPos.x() > playerPos.x()) deltaX = -1;
    
    if (enemyPos.y() < playerPos.y()) deltaY = 1;
    else if (enemyPos.y() > playerPos.y()) deltaY = -1;
    
    if (deltaX != 0 && deltaY != 0) {
        if (dist(gen) % 2 == 0) {
            deltaY = 0;
        } else {
            deltaX = 0;
        }
    }
    
    Position newPos(enemyPos.x() + deltaX, enemyPos.y() + deltaY);
    
    // Проверяем можно ли двигаться на эту позицию
    if (field->isValidPosition(newPos) && field->isPositionPassable(newPos) && 
        !field->hasCharacterAt(newPos) && !field->hasTowerAt(newPos)) {
        return newPos;
    }
    
    // Приоритет 3: случайные направления
    std::shuffle(directions.begin(), directions.end(), gen);
    
    for (const auto& dir : directions) {
        Position randomPos = enemyPos + dir;
        if (field->isValidPosition(randomPos) && field->isPositionPassable(randomPos) && 
            !field->hasCharacterAt(randomPos) && !field->hasTowerAt(randomPos)) {
            return randomPos;
        }
    }
    
    // Если нет доступных ходов, остаемся на месте
    return enemyPos;
}




// Добавляем в конец файла:

void GameController::addRandomSpell() {
    giveRandomSpell();
}

void GameController::clearEnemies() {
    // Удаляем всех врагов с поля
    auto positions = field->getAllCharacterPositions();
    for (const auto& pos : positions) {
        const Character* character = field->viewCharacterAt(pos);
        if (character && dynamic_cast<const Enemy*>(character)) {
            field->removeCharacter(pos);
        }
    }
}

void GameController::addEnemy(const Position& pos, int health, int damage) {
    std::unique_ptr<Enemy> enemy(new Enemy(health, damage));
    enemy->setHealthPublic(health);
    field->addCharacter(pos, std::move(enemy));
}

std::vector<std::pair<Position, std::pair<int, int>>> GameController::getEnemiesInfo() const {
    std::vector<std::pair<Position, std::pair<int, int>>> enemiesInfo;
    
    auto positions = field->getAllCharacterPositions();
    for (const auto& pos : positions) {
        const Character* character = field->viewCharacterAt(pos);
        const Enemy* enemy = dynamic_cast<const Enemy*>(character);
        if (enemy && enemy->isAlive()) {
            enemiesInfo.push_back({pos, {enemy->getHealth(), enemy->getDamage()}});
        }
    }
    
    return enemiesInfo;
}

void GameController::clearTowers() {
    // Удаляем все башни
    const auto& towers = field->getTowers();
    for (const auto& tower : towers) {
        const_cast<GameField&>(*field).removeTower(tower.getPosition());
    }
}

void GameController::addTower(const Position& pos, int cooldown) {
    EnemyTower tower(pos);
    // Устанавливаем кулдаун
    for (int i = 0; i < cooldown && i < 5; i++) {
        const_cast<EnemyTower&>(tower).updateCooldown();
    }
    field->addTower(tower);
}

std::vector<std::pair<Position, int>> GameController::getTowersInfo() const {
    std::vector<std::pair<Position, int>> towersInfo;
    
    const auto& towers = field->getTowers();
    for (const auto& tower : towers) {
        towersInfo.push_back({tower.getPosition(), tower.getCurrentCooldown()});
    }
    
    return towersInfo;
}

std::vector<Position> GameController::getTrapsInfo() const {
    std::vector<Position> trapsInfo;
    
    const auto& traps = field->getTraps();
    for (const auto& trap : traps) {
        if (trap.second.isActive()) {
            trapsInfo.push_back(trap.first);
        }
    }
    
    return trapsInfo;
}