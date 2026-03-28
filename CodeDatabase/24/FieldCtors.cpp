#include "Field.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"
#include <random>

// Конструктор копирования
Field::Field(const Field& other)
    : width(other.width),
      height(other.height),
      player(other.player),
      playerPos(other.playerPos),
      enemyManager(std::make_unique<EnemyManager>()),
      grid(other.grid) {
    
    // Копируем врагов из другого поля
    const auto& otherEnemies = other.enemyManager->getEnemies();
    for (const auto& enemyPair : otherEnemies) {
        enemyManager->addEnemy(
            std::make_unique<Enemy>(*enemyPair.first),
            enemyPair.second
        );
    }
}

// Конструктор перемещения
Field::Field(Field&& other) noexcept
    : width(other.width),
      height(other.height),
      player(std::move(other.player)),
      playerPos(std::move(other.playerPos)),
      enemyManager(std::move(other.enemyManager)),
      grid(std::move(other.grid)) {
    
    // Сбрасываем состояние исходного объекта
    other.width = 0;
    other.height = 0;
}

// Оператор присваивания копированием
Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        player = other.player;
        playerPos = other.playerPos;
        grid = other.grid;
        
        // Копируем врагов
        enemyManager = std::make_unique<EnemyManager>();
        const auto& otherEnemies = other.enemyManager->getEnemies();
        for (const auto& enemyPair : otherEnemies) {
            enemyManager->addEnemy(
                std::make_unique<Enemy>(*enemyPair.first),
                enemyPair.second
            );
        }
    }
    return *this;
}

// Оператор присваивания перемещением
Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        player = std::move(other.player);
        playerPos = std::move(other.playerPos);
        grid = std::move(other.grid);
        enemyManager = std::move(other.enemyManager);
        
        // Сбрасываем состояние исходного объекта
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

// Конструктор игрового поля
Field::Field(int w, int h, const Pos& startPos) 
    : width(w), height(h), playerPos(startPos), enemyManager(std::make_unique<EnemyManager>()) {
    
    // Проверка валидности размеров поля
    if (!Config::Constraints::validateFieldSize(w) || !Config::Constraints::validateFieldSize(h)) {
        throw std::invalid_argument("Invalid field size");
    }
    
    // Проверка стартовой позиции
    if (!startPos.isValid(w, h)) {
        throw std::invalid_argument("Invalid start position");
    }
    
    initializeGrid();
}

// Конструктор для создания поля по конфигурации уровня
Field::Field(const LevelConfig& config)
    : width(config.fieldWidth), height(config.fieldHeight), 
      playerPos(Config::Settings::PLAYER_START_X, Config::Settings::PLAYER_START_Y),
      enemyManager(std::make_unique<EnemyManager>()) {
    
    initializeGrid();
    
    // Случайное размещение врагов
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(1, width - 2);  // Не у самых краев
    std::uniform_int_distribution<> distY(1, height - 2);
    
    for (int i = 0; i < config.enemyCount; ++i) {
        int attempts = 0;
        while (attempts < 50) {  // Ограничиваем попытки
            Pos enemyPos(distX(gen), distY(gen));
            
            // Проверяем, что позиция валидна и не занята
            if (isValidPosition(enemyPos) && 
                isCellPassable(enemyPos) && 
                !isEnemyAt(enemyPos) && 
                enemyPos != playerPos &&
                enemyPos.distanceTo(playerPos) > 3) {  // Не слишком близко к игроку
                
                addEnemy(std::make_unique<Enemy>(config.enemyHealth, config.enemyDamage), enemyPos);
                break;
            }
            attempts++;
        }
    }
}