#include "game.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game() 
    : field(nullptr), player(nullptr), 
      tower(nullptr), damageTower(nullptr), currentLevel(1), 
      gameRunning(false), actionCounter(0) {
    srand(time(0));
}

Game::~Game() {
    cleanup();
}

void Game::cleanup() {
    delete field;
    delete player;
    delete tower;
    delete damageTower;
    
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    
    field = nullptr;
    player = nullptr;
    tower = nullptr;
    damageTower = nullptr;
}

void Game::initializeGame() {
    cleanup();
    
    std::cout << "СОЗДАНИЕ НОВОЙ ИГРЫ" << std::endl;
    
    //поле
    int width, height;
    std::cout << "Введите размеры поля (ширина высота): ";
    std::cin >> width >> height;
    std::cin.ignore();
    
    std::cout << "Введите процент препятствий: ";
    int percentage;
    std::cin >> percentage;
    std::cin.ignore();
    
    field = new Field(width, height, percentage);
    
    
    //игрок
    std::cout << "Введите имя персонажа: ";
    std::string playerName;
    std::getline(std::cin, playerName);
    
    player = new Player(playerName, *field, constants::PLAYER_INITIAL_X, constants::PLAYER_INITIAL_Y, constants::HAND_SIZE);
    
    initializeLevel();
    
    gameRunning = true;
    
    std::cout << "Игра началась! Удачи, " << player->getname() << "!" << std::endl;
}

void Game::initializeLevel() {
    //враги
    int enemyDamageIncrement = (currentLevel - 1) * constants::ENEMY_DAMAGE_INCREMENT;
    Enemy* enemy = new Enemy("Враг", *field, constants::ENEMY_INITIAL_X, constants::ENEMY_INITIAL_Y);
    enemy->upgradeDamage(enemyDamageIncrement);
    enemies.push_back(enemy);
    
    //башни
    tower = new Tower("Башня", *field, constants::TOWER_INITIAL_X, constants::TOWER_INITIAL_Y);
    damageTower = new DamageTower("Башня урона", *field, constants::DAMAGE_TOWER_INITIAL_X, constants::DAMAGE_TOWER_INITIAL_Y);
    int towerDamageIncrement = (currentLevel - 1) * constants::TOWER_DAMAGE_INCREMENT;
    damageTower->upgradeDamage(towerDamageIncrement);
    
    actionCounter = 0;
}

void Game::startNewGame() {
    try {
        initializeGame();
    } catch (const std::exception& e) {
        std::cout << "Ошибка инициализации игры: " << e.what() << std::endl;
    }
}
/*
void Game::restartGame() {
    std::cout << "Перезапуск игры" << std::endl;
    currentLevel = 1;
    startNewGame();
}*/


void Game::processEnemiesTurn() {
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            if (enemy->canAttack(player->getX(), player->getY())) {
                player->takeDamage(enemy->getAttackPower());
                std::cout << enemy->getname() << " атаковал игрока! Урон: " << enemy->getAttackPower() << std::endl;
            } else {
                enemy->moveTowards(player->getX(), player->getY());
                if (!enemy->isAlive()) {
                    field->getCell(enemy->getX(), enemy->getY()).removeEntity();
                    player->addScore(1);
                    std::cout << "Враг уничтожен! Получено 1 очко." << std::endl;
                }
            }
        }
    }
    
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->isAlive()) {
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::processTowersTurn() {
    //спавн башня
    actionCounter++;
    if (actionCounter >= constants::TOWER_SPAWN_RATE) {
        tower->spawnEnemy(enemies);
        actionCounter = 0;
    }
    
    //башня урона
    damageTower->updateCooldown();
    if (int towerDamage = damageTower->attackPlayer(player->getX(), player->getY())) {
        player->takeDamage(towerDamage);
        std::cout << damageTower->getName() << " атаковала игрока! Урон: " << towerDamage << std::endl;
    }
}

void Game::checkLevelCompletion() {
    bool enemiesRemain = false;
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemiesRemain = true;
            break;
        }
    }
    
    if (!enemiesRemain && !damageTower->isAlive()) {
        std::cout << "Уровень " << currentLevel << " пройден!" << std::endl;
        
        if (currentLevel >= 3) {
            gameRunning = false;
            std::cout << "ПОЗДРАВЛЯЕМ! Вы прошли все уровни!" << std::endl;
            std::cout << "Итоговый счет: " << player->getScore() << std::endl;
        } else {
            advanceToNextLevel();
        }
    }
}

void Game::checkGameOver() {
    if (!player->isAlive()) {
        gameRunning = false;
        handleGameOver();
        return;
    }
    
    bool canMove = false;
    std::vector<std::pair<int, int>> directions = {{0,-1}, {0,1}, {-1,0}, {1,0}};
    for (auto dir : directions) {
        int testX = player->getX() + dir.first;
        int testY = player->getY() + dir.second;
        if (field->isValidPosition(testX, testY) && field->getCell(testX, testY).isPassable()) {
            canMove = true;
            break;
        }
    }
    
    if (!canMove) {
        gameRunning = false;
        std::cout << "Игра окончена! Игрок заблокирован." << std::endl;
        handleGameOver();
    }
}

void Game::advanceToNextLevel() {
    currentLevel++;
    std::cout << "Переход на уровень " << currentLevel << "!" << std::endl;
    
    std::string playerName = player->getname();
    int playerScore = player->getScore();
    Player::CombatStyle playerCombatStyle = player->getCombatType();
    int playerDamage = player->getPower();
    
    std::vector<Spell*> savedSpells;
    Hand& oldHand = player->getSpellHand();
    for (int i = 0; i < oldHand.getSpellCount(); i++) {
        Spell* spell = oldHand.getSpell(i);
        if (DirectDamageSpell* directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
            DirectDamageSpell* newSpell = new DirectDamageSpell();
            newSpell->upgrade(directSpell->getPower() - constants::DIRECT_SPELL_POWER);
            savedSpells.push_back(newSpell);
        } else if (AreaDamageSpell* areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
            AreaDamageSpell* newSpell = new AreaDamageSpell();
            newSpell->upgrade(areaSpell->getPower() - constants::AREA_SPELL_POWER);
            savedSpells.push_back(newSpell);
        } else if (TrapSpell* trapSpell = dynamic_cast<TrapSpell*>(spell)) {
            TrapSpell* newSpell = new TrapSpell();
            newSpell->upgrade(trapSpell->getPower() - constants::TRAP_SPELL_POWER);
            savedSpells.push_back(newSpell);
        }
    }
    
    int newWidth = std::min(field->getWidth() + constants::FIELD_SIZE_INCREMENT, constants::FIELD_MAX_SIZE);
    int newHeight = std::min(field->getLength() + constants::FIELD_SIZE_INCREMENT, constants::FIELD_MAX_SIZE);
    
    if (newWidth != field->getWidth() || newHeight != field->getLength()) {
        std::cout << "Введите процент препятствий для расширенного поля: ";
        int percentage;
        std::cin >> percentage;
        std::cin.ignore();
        Field* newField = new Field(newWidth, newHeight, percentage);
        delete field;
        field = newField;
        std::cout << "Размер поля увеличен до " << newWidth << "x" << newHeight << std::endl;
    }
    
    Player* oldPlayer = player;
    player = new Player(playerName, *field, constants::PLAYER_INITIAL_X, constants::PLAYER_INITIAL_Y, constants::HAND_SIZE);
    player->setScore(playerScore);
    player->setCombatType(playerCombatStyle);
    player->upgradeDamage(playerDamage - constants::PLAYER_NEAR_COMBAT_POWER);
    
    Hand& newHand = player->getSpellHand();
    for (Spell* spell : savedSpells) {
        newHand.addSpell(spell);
    }
    delete oldPlayer;
    player->setLives(constants::PLAYER_INITIAL_LIVES);
    std::cout << "Здоровье игрока восстановлено до " << player->getLives() << std::endl;
    
    removeHalfOfSpells();
    
    offerPlayerUpgrade();
    
    delete tower;
    tower = nullptr;
    delete damageTower;
    damageTower = nullptr;
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    
    try {
        initializeLevel();
    } catch (const std::exception& e) {
        std::cout << "Ошибка при создании нового уровня: " << e.what() << std::endl;
        gameRunning = false;
        handleGameOver();
    }
}

void Game::handleGameOver() {
    std::cout << "\nИГРА ОКОНЧЕНА" << std::endl;
    std::cout << "Ваш герой пал в бою на уровне " << currentLevel << std::endl;
    std::cout << "Итоговый счет: " << player->getScore() << std::endl;
    
    std::cout << "Хотите начать заново? (yes/no): ";
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "yes") {
        restartGame();
    } else {
        std::cout << "Спасибо за игру!" << std::endl;
    }
}



Field* Game::getField() const {
    return field;
}
Player* Game::getPlayer() const {
    return player;
}
const std::vector<Enemy*>& Game::getEnemies() const {
    return enemies;
}
Tower* Game::getTower() const {
    return tower;
}
DamageTower*  Game::getDamageTower() const {
    return damageTower;
}

int Game::getCurrentLevel() const {
    return currentLevel;
}

bool Game::isRunning() const {
    return gameRunning;
}

void Game::setCurrentLevel(int level) {
    currentLevel = level;
}
void Game::setGameRunning(bool running) {
    gameRunning = running;
}

void Game::setField(Field* newField) { 
    if (field) delete field;
    field = newField; 
}

void Game::setPlayer(Player* newPlayer) { 
    if (player) delete player;
    player = newPlayer; 
}

void Game::setTower(Tower* newTower) { 
    if (tower) delete tower;
    tower = newTower; 
}

void Game::setDamageTower(DamageTower* newTower) { 
    if (damageTower) delete damageTower;
    damageTower = newTower; 
}

void Game::clearEnemies() { 
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void Game::addEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void Game::updatePlayerEffects() {
    if (player) {
        player->updateSlowEffect();
    }
}

void Game::removeHalfOfSpells() {
    Hand& hand = player->getSpellHand();
    int spellCount = hand.getSpellCount();
    if (spellCount <= 1) return;
    
    int spellsToRemove = spellCount / 2;
    std::cout << "Удаляем " << spellsToRemove << " заклинаний из руки..." << std::endl;
    
    for (int i = 0; i < spellsToRemove; i++) {
        int randomIndex = rand() % hand.getSpellCount();
        hand.removeSpell(randomIndex);
    }
}

void Game::offerPlayerUpgrade() {
    std::cout << "\nУЛУЧШЕНИЕ ХАРАКТЕРИСТИК" << std::endl;
    std::cout << "Выберите улучшение для уровня " << currentLevel << ":" << std::endl;
    std::cout << "1. Увеличить урон игрока на " << constants::PLAYER_DAMAGE_INCREMENT << std::endl;
    std::cout << "2. Увеличить урон всех заклинаний на " << constants::SPELL_DAMAGE_INCREMENT << std::endl;
    std::cout << "Ваш выбор: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "1") {
        player->upgradeDamage(constants::PLAYER_DAMAGE_INCREMENT);
        std::cout << "Урон игрока увеличен! Теперь: " << player->getPower() << std::endl;
    } else if (choice == "2") {
        Hand& hand = player->getSpellHand();
        for (int i = 0; i < hand.getSpellCount(); i++) {
            Spell* spell = hand.getSpell(i);
            if (SpellBasis* spellBasis = dynamic_cast<SpellBasis*>(spell)) {
                spellBasis->upgrade();
            }
        }
        std::cout << "Урон всех заклинаний увеличен!" << std::endl;
    } else {
        std::cout << "Неверный выбор, улучшение не применено." << std::endl;
    }
}

bool Game::movePlayer(int dx, int dy) {
    if (!player) return false;
    
    bool success = false;
    if (dx == 0 && dy == -1) {
        success = player->moveUp();
    } else if (dx == 0 && dy == 1) {
        success = player->moveDown();
    } else if (dx == -1 && dy == 0) {
        success = player->moveLeft();
    } else if (dx == 1 && dy == 0) {
        success = player->moveRight();
    }
    
    if (success) {
        std::cout << "Новая позиция: (" << player->getX() << ", " << player->getY() << ")" << std::endl;
    }
    return success;
}

bool Game::attack() {
    if (!player) return false;
    
    bool attacked = false;
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive() && player->canAttack(enemy->getX(), enemy->getY(), enemy->getname())) {
            enemy->takeDamage(player->getPower());
            if (!enemy->isAlive()) {
                field->getCell(enemy->getX(), enemy->getY()).removeEntity();
                player->addScore(1);
                std::cout << "Враг уничтожен! Получено 1 очко." << std::endl;
            }
            attacked = true;
            break;
        }
    }
    
    if (!attacked && damageTower->isAlive() && 
        player->canAttack(damageTower->getX(), damageTower->getY(), damageTower->getName())) {
        damageTower->takeDamage(player->getPower());
        if (!damageTower->isAlive()) {
            player->addScore(3);
        }
        attacked = true;
    }
    
    if (!attacked) {
        std::cout << "Нет целей для атаки в радиусе!" << std::endl;
    }
    
    return attacked;
}

bool Game::changeCombatType() {
    if (!player) return false;
    
    player->switchCombatStyle(player->getCombatType() == Player::CombatStyle::MELEE 
        ? Player::CombatStyle::RANGED 
        : Player::CombatStyle::MELEE);
    return true;
}

bool Game::showStatus() {
    std::cout << "\nСТАТИСТИКА ИГРОКА" << std::endl;
    std::cout << "Имя: " << player->getname() << std::endl;
    std::cout << "Здоровье: " << player->getLives() << std::endl;
    std::cout << "Очки: " << player->getScore() << std::endl;
    std::cout << "Стиль боя: " << (player->getCombatType() == Player::CombatStyle::MELEE ? "Ближний" : "Дальний") << std::endl;
    std::cout << "Сила атаки: " << player->getPower() << std::endl;
    std::cout << "Дальность: " << player->getRadius() << std::endl;
    std::cout << "Позиция: (" << player->getX() << "," << player->getY() << ")" << std::endl;

    for (const Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            std::cout << "\nСТАТИСТИКА ВРАГА" << std::endl;
            std::cout << "Имя: " << enemy->getname() << std::endl;
            std::cout << "Здоровье: " << enemy->getLives() << std::endl;
            std::cout << "Позиция: (" << enemy->getX() << "," << enemy->getY() << ")" << std::endl;
        }
    }

    if (damageTower && damageTower->isAlive()) {
        std::cout << "\nБАШНЯ УРОНА" << std::endl;
        std::cout << "Имя: " << damageTower->getName() << std::endl;
        std::cout << "Здоровье: " << damageTower->getLives() << std::endl;
        std::cout << "Позиция: (" << damageTower->getX() << "," << damageTower->getY() << ")" << std::endl;
        std::cout << "Перезарядка: " << damageTower->getCooldownTimer() << std::endl;
    }
    return false;
}

bool Game::showSpells() {
    if (player) {
        player->showSpells();
    }
    return false;
}

bool Game::castSpell(int spellIndex, int targetX, int targetY) {
    if (!player) return false;
    
    if (player->getSpellHand().isEmpty()) {
        std::cout << "В руке нет заклинаний!" << std::endl;
        return false;
    }
    
    bool spellUsed = false;
    bool attacked = false;
    std::string spellName = player->getSpellHand().getAllSpells()[spellIndex]->getName();
    
    if (spellName == "Магическая ловушка") {
        player->castSpell(spellIndex, targetX, targetY, 0, 0);
        spellUsed = true;
    }
    else {
        for (Enemy* enemy : enemies) {
            if (int spellPower = player->castSpell(spellIndex, targetX, targetY, enemy->getX(), enemy->getY())) {
                enemy->takeDamage(spellPower);
                if (!enemy->isAlive()) {
                    field->getCell(enemy->getX(), enemy->getY()).removeEntity();
                    player->addScore(1);
                    std::cout << "Враг уничтожен заклинанием! Получено 1 очко." << std::endl;
                }
                attacked = true;
                break;
            }
        }
        
        if (damageTower->isAlive()) {
            if (int spellPower = player->castSpell(spellIndex, targetX, targetY, damageTower->getX(), damageTower->getY())) {
                damageTower->takeDamage(spellPower);
                if (!damageTower->isAlive()) {
                    player->addScore(3);
                }
                attacked = true;
            }
        }
        
        if (attacked) {
            spellUsed = true;
        }
    }
    if (spellUsed) {
        player->getSpellHand().removeSpell(spellIndex);
        std::cout << "Заклинание использовано!" << std::endl;
        return true;
    } else {
        std::cout << "Не удалось использовать заклинание." << std::endl;
        return false;
    }
}

bool Game::buySpell() {
    if (!player) return false;
    return player->buyNewSpell();
}

bool Game::saveGame() {
    try {
        saveSystem.saveGame(*this);
        std::cout << "Игра успешно сохранена!" << std::endl;
        return true;
    } catch (const std::runtime_error& e) {
        std::cout << "Ошибка сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool Game::loadGame() {
    try {
        return saveSystem.loadGame(*this);
    } catch (const std::runtime_error& e) {
        std::cout << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    }
}

bool Game::quitGame() {
    setGameRunning(false);
    std::cout << "Выход из игры..." << std::endl;
    return false;
}

bool Game::showMenu() {
    std::cout << "\nКОМАНДЫ" << std::endl;
    std::cout << "W - Вперед" << std::endl;
    std::cout << "S - Назад" << std::endl;
    std::cout << "A - Влево" << std::endl;
    std::cout << "D - Вправо" << std::endl;
    std::cout << "X - Атака" << std::endl;
    std::cout << "C - Смена оружия" << std::endl;
    std::cout << "SHOW_STATUS - Статистика" << std::endl;
    std::cout << "SHOW_SPELLS - Показать заклинания" << std::endl;
    std::cout << "CAST - Использовать заклинание" << std::endl;
    std::cout << "BUY - Купить заклинание" << std::endl;
    std::cout << "SAVE - Сохранить игру" << std::endl;
    std::cout << "LOAD - Загрузить игру" << std::endl;
    std::cout << "RESTART - Начать заново" << std::endl;
    std::cout << "SHOW_MENU - Помощь" << std::endl;
    std::cout << "QUIT - Выход" << std::endl;
    return false;
}

bool Game::restartGame() {
    std::cout << "Перезапуск игры" << std::endl;
    currentLevel = 1;
    startNewGame();
    return false;
}