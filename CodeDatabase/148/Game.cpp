#include "Game.h"
#include "GameSaveManager.h"
#include "LevelBuilder.h"
#include "TurnProcessor.h"
#include "SpellHand.h"
#include "Exceptions.h"
#include "GameTypes.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>


/**
 * @file Game.cpp
 * @brief Реализация основного класса игры
 */


/**
 * @brief Конструктор игры
 */
Game::Game() 
    : gameRunning(false) {
    srand(time(nullptr));
    initializeGame();
}


/**
 * @brief Деструктор
 */
Game::~Game() = default;


/**
 * @brief Инициализировать игру
 */
void Game::initializeGame() {
    field = std::make_unique<GameField>(DEFAULT_FIELD_SIZE, DEFAULT_FIELD_SIZE);
    field->generateBlockedCells(20);
    
    int centerX = field->getWidth() / 2;
    int centerY = field->getHeight() / 2;
    
    player = std::make_unique<Player>(centerX, centerY, PLAYER_START_HEALTH, PLAYER_START_DAMAGE);
    spellHand = std::make_unique<SpellHand>(5);
    
    spawnEnemy(2, 2);
    spawnEnemy(12, 3);
    spawnEnemy(5, 10);
    spawnEnemy(10, 12);
    
    spawnTower(1, 1, TOWER_START_HEALTH, TOWER_ATTACK_RANGE);
    
    spellHand->addRandomSpell();
    
    gameRunning = true;
}


/**
 * @brief Добавить союзника
 */
void Game::addAlly(std::unique_ptr<Ally> ally) {
    allies.push_back(std::move(ally));
}


/**
 * @brief Добавить башню
 */
void Game::addTower(std::unique_ptr<EnemyTower> tower) {
    towers.push_back(std::move(tower));
}


/**
 * @brief Добавить ловушку
 */
void Game::addTrap(std::unique_ptr<Trap> trap) {
    traps.push_back(std::move(trap));
}


/**
 * @brief Использовать заклинание (версия без координат)
 */
bool Game::usePlayerSpell(int spellIndex) {
    if (!spellHand) {
        return false;
    }


    bool success = spellHand->useSpell(spellIndex, this);
    
    if (success) {
        processAfterPlayerAction();
    }


    return success;
}


/**
 * @brief Использовать заклинание с координатами
 */
/**
 * @brief Использовать заклинание с координатами
 */
bool Game::usePlayerSpell(int spellIndex, int targetX, int targetY) {
    if (!spellHand) {
        return false;
    }

    Spell* spell = spellHand->getSpellAt(spellIndex);
    if (!spell) {
        return false;
    }

    // Применяем заклинание с координатами
    spell->cast(this, targetX, targetY);  // ← this вместо *this
    
    // Удаляем заклинание после использования
    spellHand->removeSpell(spellIndex);
    
    processAfterPlayerAction();
    
    return true;
}



/**
 * @brief Добавить случайное заклинание
 */
void Game::addRandomSpellToHand() {
    if (spellHand) {
        spellHand->addRandomSpell();
    }
}


/**
 * @brief Обработать действия после хода игрока
 */
void Game::processAfterPlayerAction() {
    checkTrapActivations();
    removeDeadEnemies();
    processAllyTurns();
    processEnemyTurns();
    processTowerTurns();
    removeActivatedTraps();
    removeDeadAllies();
    
    if (!player->isAlive()) {
        gameOver();
    }
}


/**
 * @brief Обработать ходы союзников
 */
void Game::processAllyTurns() {
    TurnProcessor processor(this);
    processor.processAllyTurns();
}


/**
 * @brief Обработать ходы башен
 */
void Game::processTowerTurns() {
    TurnProcessor processor(this);
    processor.processTowerTurns();
}


/**
 * @brief Проверить активацию ловушек
 */
void Game::checkTrapActivations() {
    for (auto& trap : traps) {
        if (!trap->isAlive() || trap->isActivated()) continue;
        
        auto [trapX, trapY] = trap->getPosition();
        
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;
            auto [ex, ey] = enemy->getPosition();
            if (ex == trapX && ey == trapY) {
                int trapDamage = trap->activate();
                enemy->takeDamage(trapDamage);
                break;
            }
        }
    }
}


/**
 * @brief Удалить мёртвых врагов
 */
void Game::removeDeadEnemies() {
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) {
            player->addScore(10);
            player->incrementKillCount();
            
            if (player->getEnemiesKilled() % 2 == 0 && player->getEnemiesKilled() > 0) {
                spellHand->addRandomSpell();
            }
        }
    }


    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), 
            [](const auto& e) { return !e->isAlive(); }),
        enemies.end()
    );
    
    towers.erase(
        std::remove_if(towers.begin(), towers.end(), 
            [](const auto& t) { return !t->isAlive(); }),
        towers.end()
    );
}


/**
 * @brief Удалить мёртвых союзников
 */
void Game::removeDeadAllies() {
    allies.erase(
        std::remove_if(allies.begin(), allies.end(), 
            [](const auto& a) { return !a->isAlive(); }),
        allies.end()
    );
}


/**
 * @brief Удалить активированные ловушки
 */
void Game::removeActivatedTraps() {
    traps.erase(
        std::remove_if(traps.begin(), traps.end(), 
            [](const auto& t) { return !t->isAlive(); }),
        traps.end()
    );
}


/**
 * @brief Обработать ход игрока
 */
void Game::processPlayerTurn(Direction direction) {
    TurnProcessor processor(this);
    processor.processPlayerTurn(direction);
}


/**
 * @brief Обработать ходы врагов
 */
void Game::processEnemyTurns() {
    TurnProcessor processor(this);
    processor.processEnemyTurns();
}


/**
 * @brief Проверить коллизии
 */
void Game::checkCollisions() {
    // Пустая функция - не используется
}


/**
 * @brief Обработать бой
 */
void Game::handleCombat(Enemy* enemy) {
    // Реализация боя делегируется TurnProcessor
    if (enemy && player->getAttackMode() == AttackMode::MELEE) {
        player->attack(enemy);
    }
}


/**
 * @brief Переключить режим атаки игрока
 */
void Game::switchPlayerAttackMode() {
    player->switchAttackMode();
}


/**
 * @brief Проверить валидность хода
 */
bool Game::isValidMove(int x, int y) const {
    if (x < 0 || x >= field->getWidth() || y < 0 || y >= field->getHeight()) {
        return false;
    }


    return field->isCellPassable(x, y);
}


/**
 * @brief Конец игры
 */
void Game::gameOver() {
    gameRunning = false;
}


/**
 * @brief Создать врага
 */
void Game::spawnEnemy(int x, int y) {
    enemies.push_back(std::make_unique<Enemy>(x, y));
}


/**
 * @brief Создать врага с бонусами
 */
void Game::spawnEnemy(int x, int y, int healthBonus, int damageBonus) {
    auto enemy = std::make_unique<Enemy>(x, y);
    int newHealth = enemy->getHealth() + healthBonus;
    int newDamage = enemy->getDamage() + damageBonus;
    enemy->setHealth(newHealth);
    enemy->setDamage(newDamage);
    enemies.push_back(std::move(enemy));
}


/**
 * @brief Создать башню
 */
void Game::spawnTower(int x, int y, int health, int range) {
    if (x >= 0 && x < field->getWidth() && y >= 0 && y < field->getHeight()) {
        towers.push_back(std::make_unique<EnemyTower>(x, y, health, 20, range, 2));
    }
}


/**
 * @brief Загрузить состояние игрока
 */
void Game::loadPlayerState(int health, int maxHealth, int damage, int x, int y, 
                           int score, int kills, AttackMode mode) {
    if (!player) {
        player = std::make_unique<Player>(x, y, health, damage);
    }


    player->setHealth(health);
    player->setMaxHealth(maxHealth);
    player->setDamage(damage);
    player->setPosition(x, y);
    player->setScore(score);
    player->setEnemiesKilled(kills);
    player->setAttackMode(mode);
}


/**
 * @brief Загрузить состояние руки заклинаний
 */
void Game::loadSpellHandState(int spellCount, int maxSize, int boostStacks) {
    if (!spellHand) {
        spellHand = std::make_unique<SpellHand>(maxSize);
    }


    while (spellHand->getSpellCount() > 0) {
        spellHand->removeSpell(0);
    }


    for (int i = 0; i < spellCount; ++i) {
        spellHand->addRandomSpell();
    }


    spellHand->resetBoostStacks();
    for (int i = 0; i < boostStacks; ++i) {
        spellHand->addBoostStack();
    }
}


/**
 * @brief Сохранить игру
 */
void Game::saveGame(const std::string& saveName) {
    try {
        GameSaveManager::saveGame(this, saveName);
    } catch (const GameException& e) {
        // Ошибка сохранения - обрабатывается выше
    }
}


/**
 * @brief Загрузить игру
 */
void Game::loadGame(const std::string& saveName) {
    try {
        GameSaveManager::loadGame(this, saveName);
        gameRunning = true;
    } catch (const GameException& e) {
        // Ошибка загрузки - обрабатывается выше
    }
}


/**
 * @brief Следующий уровень (ИСПРАВЛЕНО - добавлена прокачка)
 */
void Game::nextLevel() {
    // 1. Восстанавливаем здоровье
    player->setHealth(player->getMaxHealth());
    
    // 2. Удаляем половину заклинаний
    int currentSpells = spellHand->getSpellCount();
    int toRemove = currentSpells / 2;
    
    for (int i = 0; i < toRemove; ++i) {
        if (spellHand->getSpellCount() > 0) {
            int randomIndex = rand() % spellHand->getSpellCount();
            spellHand->removeSpell(randomIndex);
        }
    }


    // 3. Переходим на следующий уровень
    levelManager.nextLevel();
    
    // 4. Очищаем все сущности
    enemies.clear();
    allies.clear();
    towers.clear();
    traps.clear();
    
    // 5. Строим новый уровень
    LevelConfig config = levelManager.getCurrentLevelConfig();
    initializeLevel(config);
}


/**
 * @brief Инициализировать уровень
 */
void Game::initializeLevel(const LevelConfig& config) {
    LevelBuilder builder(this);
    builder.buildLevel(config);
}


/**
 * @brief Применить улучшение (ИСПРАВЛЕНО)
 */
void Game::applyUpgrade(const Upgrade* upgrade) {
    if (!upgrade) return;
    
    switch (upgrade->type) {
        case UpgradeType::INCREASE_MAX_HEALTH:
            player->setMaxHealth(player->getMaxHealth() + upgrade->value);
            player->setHealth(player->getMaxHealth());
            break;
            
        case UpgradeType::INCREASE_DAMAGE:
            player->setDamage(player->getDamage() + upgrade->value);
            break;
            
        case UpgradeType::INCREASE_SPELL_HAND_SIZE:
            spellHand->increaseMaxSize(upgrade->value);
            break;
            
        case UpgradeType::HEAL_PLAYER:
            player->setHealth(player->getMaxHealth());
            break;
    }
}


/**
 * @brief Начать новую игру (сброс после Game Over)
 */
void Game::startNewGame() {
    // Очищаем всех врагов, союзников, башни, ловушки
    enemies.clear();
    allies.clear();
    towers.clear();
    traps.clear();
    
    // Сбрасываем уровень
    levelManager.setCurrentLevel(1);
    
    // Пересоздаём игру
    initializeGame();
}


