#ifdef _WIN32
#define NOMINMAX
#endif

#include "game.h"
#include "03_direct_damage_spell.h"
#include "04_area_damage_spell.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>
#endif

const std::string Game::SAVE_FILE = "game_save.dat";

Game::Game() : state_(GameState::MAIN_MENU), field_(15, 15),
currentLevel_(1), score_(0), running_(true) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Game::~Game() {}

void Game::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Game::waitForEnter() const {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Game::displayField() const {
    int width = field_.getWidth();
    int height = field_.getHeight();

    std::cout << "\n  ";
    for (int x = 0; x < width; ++x) std::cout << x % 10 << " ";
    std::cout << std::endl;

    for (int y = 0; y < height; ++y) {
        std::cout << y % 10 << " ";
        for (int x = 0; x < width; ++x) {
            Position pos{ x, y };
            char cell = '.';

            if (player_->getPosition().x == x && player_->getPosition().y == y) {
                cell = 'P';
            }
            else {
                bool enemyHere = false;
                for (size_t i = 0; i < enemies_.size(); ++i) {
                    if (enemies_[i].isAlive() &&
                        enemies_[i].getPosition().x == x &&
                        enemies_[i].getPosition().y == y) {
                        cell = 'E';
                        enemyHere = true;
                        break;
                    }
                }

                if (!enemyHere && !field_.isPositionPassable(pos)) {
                    cell = '#';
                }
            }

            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nP - Игрок, E - Враг, # - Стена, . - Путь" << std::endl;
}

void Game::displayStats() const {
    std::cout << "\n=== УРОВЕНЬ " << currentLevel_ << " ===" << std::endl;
    std::cout << "Счет: " << score_ << " | Здоровье: " << player_->getHealth()
        << " | Убийств: " << player_->getKillCount() << std::endl;
    std::cout << "Режим боя: " << (player_->isMeleeMode() ? "ближний" : "дальний")
        << " | Урон: " << player_->getDamage()
        << " | Радиус атаки: " << (player_->isMeleeMode() ? 1 : 3) << std::endl;

    std::cout << "Заклинания (" << player_->getSpellHand().getSpellCount()
        << "/" << player_->getSpellHand().getMaxSize() << "): ";
    for (int i = 0; i < player_->getSpellHand().getSpellCount(); ++i) {
        auto spell = player_->getSpellHand().getSpell(i);
        std::cout << spell->getName();
        if (i < player_->getSpellHand().getSpellCount() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

void Game::run() {
    while (running_) {
        switch (state_) {
        case GameState::MAIN_MENU:
            mainMenu();
            break;
        case GameState::PLAYING:
            gameLoop();
            break;
        case GameState::GAME_OVER:
            clearScreen();
            std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
            std::cout << "Итоговый счет: " << score_ << std::endl;

            int choice;
            std::cout << "\n1. Начать заново\n2. Выйти в меню\nВыберите вариант: ";
            std::cin >> choice;

            if (choice == 1) {
                startNewGame();
            }
            else {
                state_ = GameState::MAIN_MENU;
            }
            break;
        case GameState::LEVEL_COMPLETE:
            clearScreen();
            std::cout << "\n=== УРОВЕНЬ " << currentLevel_ << " ПРОЙДЕН! ===" << std::endl;
            score_ += currentLevel_ * 100;

            if (currentLevel_ < MAX_LEVELS) {
                // сохраняем состояние игрока перед очисткой уровня
                int savedHealth = player_->getHealth();
                int savedKillCount = player_->getKillCount();
                bool savedMode = player_->isMeleeMode();

                enemies_.clear();

                // Загружаем следующий уровень
                currentLevel_++;
                loadLevel(currentLevel_);

                // Восстанавливаем всё
                int healthToAdd = 15;
                int currentHealth = player_->getHealth();
                if (currentHealth < 100) {
                    int newHealth = currentHealth + healthToAdd;
                    if (newHealth > 100) newHealth = 100;
                    int healthDifference = currentHealth - newHealth;
                    player_->takeDamage(healthDifference);
                }

                // Восстанавливаем количество убийств
                int currentKills = player_->getKillCount();
                if (currentKills < savedKillCount) {
                    for (int i = currentKills; i < savedKillCount; i++) {
                        player_->addKill();
                    }
                }

                // Восстанавливаем режим боя
                if (player_->isMeleeMode() != savedMode) {
                    player_->switchCombatMode();
                }

                state_ = GameState::PLAYING;
            }
            else {
                std::cout << "\n=== ПОБЕДА! ВЫ ПРОШЛИ ВСЕ УРОВНИ! ===" << std::endl;
                std::cout << "Итоговый счет: " << score_ << std::endl;
                waitForEnter();

                // Полная очистка при завершении игры
                enemies_.clear();
                player_.reset();
                state_ = GameState::MAIN_MENU;
            }
            break;
        }
    }
}

void Game::mainMenu() {
    clearScreen();

    int choice = 0;

    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << std::endl;
    std::cout << "1. Новая игра" << std::endl;
    std::cout << "2. Загрузить игру" << std::endl;
    std::cout << "3. Выйти" << std::endl;
    std::cout << "Выберите вариант: ";

    while (!(std::cin >> choice) || choice < 1 || choice > 3) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод. Попробуйте снова: ";
    }

    switch (choice) {
    case 1:
        startNewGame();
        break;
    case 2:
        try {
            loadGame();
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка загрузки: " << e.what() << std::endl;
            std::cout << "Начинаем новую игру..." << std::endl;
            waitForEnter();
            startNewGame();
        }
        break;
    case 3:
        running_ = false;
        break;
    }
}

void Game::startNewGame() {
    // Очистка старого состояния
    player_.reset();
    enemies_.clear();
    currentLevel_ = 1;
    score_ = 0;

    // Используем существующее поле
    field_ = GameField(12, 12);

    // Загрузка первого уровня
    loadLevel(currentLevel_);
    state_ = GameState::PLAYING;

    clearScreen();
    std::cout << "\n=== НОВАЯ ИГРА НАЧАТА ===" << std::endl;
}

void Game::loadGame() {
    std::ifstream file(SAVE_FILE, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Файл сохранения не найден");
    }

    try {
        deserialize(file);
        std::cout << "\nИгра успешно загружена!" << std::endl;
        state_ = GameState::PLAYING;
    }
    catch (const std::exception& e) {
        file.close();
        throw std::runtime_error(std::string("Ошибка чтения файла: ") + e.what());
    }

    file.close();
}

void Game::saveGame() {
    std::ofstream file(SAVE_FILE, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось создать файл сохранения");
    }

    try {
        serialize(file);
        std::cout << "\nИгра успешно сохранена!" << std::endl;
    }
    catch (const std::exception& e) {
        file.close();
        throw std::runtime_error(std::string("Ошибка записи в файл: ") + e.what());
    }

    file.close();
}

void Game::gameLoop() {
    while (state_ == GameState::PLAYING) {
        clearScreen();
        displayStats();
        displayField();

        // Ход игрока
        processPlayerTurn();

        if (isGameOver()) {
            state_ = GameState::GAME_OVER;
            break;
        }

        // Ход врагов
        processEnemyTurn();

        if (isGameOver()) {
            state_ = GameState::GAME_OVER;
            break;
        }

        if (isLevelComplete()) {
            state_ = GameState::LEVEL_COMPLETE;
            break;
        }
    }
}

void Game::processPlayerTurn() {
    std::cout << "\n=== ВАШ ХОД ===" << std::endl;
    std::cout << "1. Переместиться" << std::endl;
    std::cout << "2. Использовать заклинание" << std::endl;
    std::cout << "3. Сменить режим боя" << std::endl;
    std::cout << "4. Сохранить игру" << std::endl;
    std::cout << "5. Вернуться в меню" << std::endl;
    std::cout << "Выберите действие: ";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 5) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод. Попробуйте снова: ";
    }

    switch (choice) {
    case 1: {
        std::cout << "Куда переместиться? (W - вверх, S - вниз, A - влево, D - вправо): ";
        char dir;
        std::cin >> dir;
        handleMoveCommand(dir);
        break;
    }
    case 2: {
        auto& hand = player_->getSpellHand();
        if (hand.getSpellCount() == 0) {
            std::cout << "У вас нет заклинаний!" << std::endl;
            waitForEnter();
            return;
        }

        std::cout << "Ваши заклинания:" << std::endl;
        for (int i = 0; i < hand.getSpellCount(); ++i) {
            auto spell = hand.getSpell(i);
            std::cout << i + 1 << ". " << spell->getName()
                << " (радиус: " << spell->getRange() << ")" << std::endl;
        }

        std::cout << "Выберите заклинание (1-" << hand.getSpellCount() << "): ";
        int spellIndex;
        std::cin >> spellIndex;
        spellIndex--;

        if (spellIndex < 0 || spellIndex >= hand.getSpellCount()) {
            std::cout << "Неверный выбор!" << std::endl;
            waitForEnter();
            return;
        }

        handleCastSpellCommand(spellIndex);
        break;
    }
    case 3:
        handleSwitchModeCommand();
        break;
    case 4:
        try {
            saveGame();
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка сохранения: " << e.what() << std::endl;
            waitForEnter();
        }
        break;
    case 5:
        state_ = GameState::MAIN_MENU;
        break;
    }
}

Position Game::findClosestEnemyInRange(int range) const {
    Position playerPos = player_->getPosition();
    Position closestEnemyPos = { -1, -1 };
    int minDistance = range + 1; // Инициализируем значением больше диапазона

    for (const auto& enemy : enemies_) {
        if (!enemy.isAlive()) continue;

        Position enemyPos = enemy.getPosition();
        int distance = abs(playerPos.x - enemyPos.x) + abs(playerPos.y - enemyPos.y);

        if (distance <= range && distance < minDistance) {
            minDistance = distance;
            closestEnemyPos = enemyPos;
        }
    }

    return closestEnemyPos;
}

void Game::processEnemyTurn() {
    // Ход врагов
    bool enemyAttacked = false;

    for (size_t i = 0; i < enemies_.size(); ++i) {
        if (!enemies_[i].isAlive()) continue;

        Position enemyPos = enemies_[i].getPosition();
        Position playerPos = player_->getPosition();

        int distance = abs(enemyPos.x - playerPos.x) + abs(enemyPos.y - playerPos.y);

        if (distance <= 1) {
            int damage = enemies_[i].getDamage();
            int oldHealth = player_->getHealth();
            player_->takeDamage(damage);

            std::cout << "\n=== ХОД ВРАГОВ ===" << std::endl;
            std::cout << "Враг " << (i + 1) << " атакует игрока!" << std::endl;
            std::cout << "Игрок получает " << damage << " урона!" << std::endl;
            std::cout << "Здоровье игрока: " << oldHealth << " → " << player_->getHealth() << std::endl;

            if (!player_->isAlive()) {
                std::cout << "Игрок повержен!" << std::endl;
            }

            enemyAttacked = true;
        }
        else {
            Position newPos = enemyPos;

            if (enemyPos.x < playerPos.x) newPos.x++;
            else if (enemyPos.x > playerPos.x) newPos.x--;
            else if (enemyPos.y < playerPos.y) newPos.y++;
            else if (enemyPos.y > playerPos.y) newPos.y--;

            // Проверка на возможность перемещения
            if (field_.isPositionValid(newPos) && field_.isPositionPassable(newPos)) {
                bool cellOccupied = false;
                for (size_t j = 0; j < enemies_.size(); ++j) {
                    if (i != j && enemies_[j].isAlive() &&
                        enemies_[j].getPosition().x == newPos.x &&
                        enemies_[j].getPosition().y == newPos.y) {
                        cellOccupied = true;
                        break;
                    }
                }

                if (!cellOccupied && !(newPos.x == playerPos.x && newPos.y == playerPos.y)) {
                    enemies_[i].move(newPos);
                }
            }
        }
    }

    // Удаляем мертвых врагов
    for (auto it = enemies_.begin(); it != enemies_.end();) {
        if (!it->isAlive()) {
            score_ += 50;
            it = enemies_.erase(it);
        }
        else {
            ++it;
        }
    }

    // Пауза после атаки врагов, чтобы игрок видел сообщение
    if (enemyAttacked) {
        waitForEnter();
    }
}

void Game::loadLevel(int level) {
    // Если игрок уже существует, не создаем нового, а только перемещаем в начальную позицию
    if (!player_) {
        int startHealth = 100;
        int startDamage = 25;
        player_ = std::make_unique<Player>(startHealth, startDamage, Position{ 0, 0 });
    }
    else {
        // Перемещаем существующего игрока в начальную позицию
        player_->move(Position{ 0, 0 });
    }

    // Создаем врагов
    int enemyCount = 2 + level;
    enemies_.clear(); // Очищаем старых врагов

    int width = field_.getWidth();
    int height = field_.getHeight();

    for (int i = 0; i < enemyCount; ++i) {
        Position enemyPos;
        int attempts = 0;
        do {
            enemyPos = Position{
                std::rand() % width,
                std::rand() % height
            };
            attempts++;
            if (attempts > 100) {
                enemyPos = Position{ width - 1, height - 1 };
                break;
            }
        } while (!field_.isPositionPassable(enemyPos) ||
            (enemyPos.x == 0 && enemyPos.y == 0) ||
            (abs(enemyPos.x) < 3 && abs(enemyPos.y) < 3));

        int enemyHealth = 30 + level * 10;
        int enemyDamage = 10 + level * 2;
        enemies_.emplace_back(enemyHealth, enemyDamage, enemyPos);
    }
}

bool Game::isLevelComplete() const {
    for (const auto& enemy : enemies_) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return true;
}

bool Game::isGameOver() const {
    return !player_->isAlive();
}

void Game::handleMoveCommand(char direction) {
    Position newPos = player_->getPosition();
    Position oldPos = player_->getPosition(); // Сохраняем старую позицию

    switch (toupper(direction)) {
    case 'W': newPos.y--; break;
    case 'S': newPos.y++; break;
    case 'A': newPos.x--; break;
    case 'D': newPos.x++; break;
    default:
        std::cout << "Неверное направление!" << std::endl;
        waitForEnter();
        return;
    }

    if (!field_.isPositionValid(newPos)) {
        std::cout << "Нельзя выйти за пределы поля!" << std::endl;
        waitForEnter();
        return;
    }

    // Проверка на врага на новой позиции
    bool enemyOnCell = false;
    Enemy* targetEnemy = nullptr;

    for (auto& enemy : enemies_) {
        if (enemy.isAlive() && enemy.getPosition().x == newPos.x &&
            enemy.getPosition().y == newPos.y) {
            enemyOnCell = true;
            targetEnemy = &enemy;
            break;
        }
    }

    if (enemyOnCell) {
        int attackRange = player_->isMeleeMode() ? 1 : 3;
        int distance = abs(oldPos.x - newPos.x) + abs(oldPos.y - newPos.y);

        if (distance <= attackRange) {
            std::cout << "Игрок атакует врага на позиции ("
                << newPos.x << "," << newPos.y << ")" << std::endl;
            int oldEnemyHealth = targetEnemy->getHealth();
            targetEnemy->takeDamage(player_->getDamage());
            std::cout << "Нанесено " << player_->getDamage()
                << " урона. Здоровье врага: " << oldEnemyHealth << " → " << targetEnemy->getHealth() << std::endl;

            if (!targetEnemy->isAlive()) {
                std::cout << "Враг повержен! +50 очков" << std::endl;
                score_ += 50;
                player_->addKill();
            }
            waitForEnter();
        }
        else {
            std::cout << "Враг слишком далеко для атаки! (радиус атаки: "
                << attackRange << ", расстояние: " << distance << ")" << std::endl;
            waitForEnter();
        }
        return;
    }

    // Проверка проходимости клетки
    if (!field_.isPositionPassable(newPos)) {
        std::cout << "Клетка непроходима!" << std::endl;
        waitForEnter();
        return;
    }

    // Перемещаем игрока
    player_->move(newPos);
    std::cout << "Переместились в (" << newPos.x << "," << newPos.y << ")" << std::endl;
}

void Game::handleCastSpellCommand(int spellIndex) {
    auto spell = player_->getSpellHand().getSpell(spellIndex);

    Position targetPos;

    if (auto directSpell = std::dynamic_pointer_cast<DirectDamageSpell>(spell)) {
        // Для прямого урона ищем ближайшего врага в радиусе
        targetPos = findClosestEnemyInRange(spell->getRange());

        if (targetPos.x == -1 && targetPos.y == -1) {
            std::cout << "Нет врагов в радиусе заклинания!" << std::endl;
            waitForEnter();
            return;
        }

        // Находим врага на этой позиции
        for (auto& enemy : enemies_) {
            if (enemy.isAlive() &&
                enemy.getPosition().x == targetPos.x &&
                enemy.getPosition().y == targetPos.y) {
                directSpell->setTargetEnemy(&enemy);
                break;
            }
        }
    }
    else if (auto areaSpell = std::dynamic_pointer_cast<AreaDamageSpell>(spell)) {
        // Для заклинания площади используем позицию ближайшего врага
        targetPos = findClosestEnemyInRange(spell->getRange());

        if (targetPos.x == -1 && targetPos.y == -1) {
            // Если врагов нет, используем позицию впереди игрока
            Position playerPos = player_->getPosition();
            targetPos = { playerPos.x + 1, playerPos.y }; 
        }
    }

    // Применяем заклинание
    if (player_->castSpell(spellIndex, targetPos)) {
        std::cout << "Заклинание \"" << spell->getName() << "\" успешно применено!" << std::endl;
        std::cout << "Цель: (" << targetPos.x << "," << targetPos.y << ")" << std::endl;

        bool damageDealt = false;

        if (auto directSpell = std::dynamic_pointer_cast<DirectDamageSpell>(spell)) {
            for (auto& enemy : enemies_) {
                if (enemy.isAlive() &&
                    enemy.getPosition().x == targetPos.x &&
                    enemy.getPosition().y == targetPos.y) {

                    damageDealt = true;

                    if (!enemy.isAlive()) {
                        std::cout << "Враг повержен! +50 очков" << std::endl;
                        score_ += 50;
                        player_->addKill();
                    }
                    else {
                        std::cout << "Здоровье врага: " << enemy.getHealth() << std::endl;
                    }
                    break;
                }
            }
        }
        else if (auto areaSpell = std::dynamic_pointer_cast<AreaDamageSpell>(spell)) {
            // Урон по площади
            auto area = areaSpell->getArea(targetPos);
            bool hitEnemy = false;

            std::cout << "Область поражения: ";
            for (const auto& pos : area) {
                std::cout << "(" << pos.x << "," << pos.y << ") ";
            }
            std::cout << std::endl;

            for (auto& enemy : enemies_) {
                if (!enemy.isAlive()) continue;

                for (const auto& pos : area) {
                    if (enemy.getPosition().x == pos.x &&
                        enemy.getPosition().y == pos.y) {

                        damageDealt = true;
                        int oldHealth = enemy.getHealth();
                        enemy.takeDamage(25); // Фиксированный урон 
                        std::cout << "Враг в (" << pos.x << "," << pos.y
                            << ") получает урон! Здоровье: "
                            << oldHealth << " → " << enemy.getHealth() << std::endl;
                        hitEnemy = true;

                        if (!enemy.isAlive()) {
                            std::cout << "Враг повержен! +50 очков" << std::endl;
                            score_ += 50;
                            player_->addKill();
                        }
                    }
                }
            }

            if (!hitEnemy) {
                std::cout << "В области нет врагов" << std::endl;
            }
        }

        // Удаляем использованное заклинание
        player_->getSpellHand().removeSpell(spellIndex);

        // Пауза при нанесении урона
        if (damageDealt) {
            waitForEnter();
        }
    }
    else {
        std::cout << "Не удалось применить заклинание! Цель слишком далеко." << std::endl;
        waitForEnter();
    }
}

void Game::handleSwitchModeCommand() {
    player_->switchCombatMode();
    std::cout << "Режим боя изменен на: "
        << (player_->isMeleeMode() ? "ближний" : "дальний") << std::endl;
    std::cout << "Урон: " << player_->getDamage()
        << " | Радиус атаки: " << (player_->isMeleeMode() ? 1 : 3) << std::endl;
}

void Game::serialize(std::ofstream& file) const {
    // Сохраняем состояние игры
    file.write(reinterpret_cast<const char*>(&currentLevel_), sizeof(currentLevel_));
    file.write(reinterpret_cast<const char*>(&score_), sizeof(score_));

    // Сохраняем размеры поля
    int width = field_.getWidth();
    int height = field_.getHeight();
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));

    // Сохраняем игрока
    if (player_) {
        int health = player_->getHealth();
        Position pos = player_->getPosition();
        bool mode = player_->isMeleeMode();
        int kills = player_->getKillCount();

        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
        file.write(reinterpret_cast<const char*>(&mode), sizeof(mode));
        file.write(reinterpret_cast<const char*>(&kills), sizeof(kills));
    }

    // Сохраняем врагов
    size_t enemyCount = enemies_.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));

    for (const auto& enemy : enemies_) {
        int health = enemy.getHealth();
        Position pos = enemy.getPosition();
        int damage = enemy.getDamage();
        bool alive = enemy.isAlive();

        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&alive), sizeof(alive));
    }
}

void Game::deserialize(std::ifstream& file) {
    // Загружаем состояние игры
    file.read(reinterpret_cast<char*>(&currentLevel_), sizeof(currentLevel_));
    file.read(reinterpret_cast<char*>(&score_), sizeof(score_));

    // Загружаем размеры поля
    int width, height;
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Создаем поле нужного размера
    field_ = GameField(width, height);

    // Загружаем игрока
    int health;
    Position pos;
    bool mode;
    int kills;

    file.read(reinterpret_cast<char*>(&health), sizeof(health));
    file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
    file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
    file.read(reinterpret_cast<char*>(&kills), sizeof(kills));

    player_ = std::make_unique<Player>(health, 25, pos);
    if (!mode) player_->switchCombatMode();
    for (int i = 0; i < kills; ++i) player_->addKill();

    // Загружаем врагов
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));

    enemies_.clear();
    for (size_t i = 0; i < enemyCount; ++i) {
        int enemyHealth, enemyDamage;
        Position enemyPos;
        bool alive;

        file.read(reinterpret_cast<char*>(&enemyHealth), sizeof(enemyHealth));
        file.read(reinterpret_cast<char*>(&enemyPos), sizeof(enemyPos));
        file.read(reinterpret_cast<char*>(&enemyDamage), sizeof(enemyDamage));
        file.read(reinterpret_cast<char*>(&alive), sizeof(alive));

        Enemy enemy(enemyHealth, enemyDamage, enemyPos);
        if (!alive) {
            enemy.takeDamage(enemyHealth + 1);
        }
        enemies_.push_back(enemy);
    }
}