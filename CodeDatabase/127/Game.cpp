// Game.cpp

#include "Game.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>

#include "GameHelper.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include "ControlConfig.h"
#include "GameEventObserver.h"
#include "Command.h"

namespace {

    std::string trim(const std::string& value) {
        const auto first = value.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            return {};
        }
        const auto last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    std::string toLowerCopy(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return value;
    }

    std::string extractArgument(const std::string& command) {
        const auto firstSpace = command.find_first_of(" \t");
        if (firstSpace == std::string::npos) return {};
        const auto start = command.find_first_not_of(" \t", firstSpace);
        if (start == std::string::npos) return {};
        const auto end = command.find_last_not_of(" \t\r\n");
        return command.substr(start, end - start + 1);
    }

    char extractDirectionChar(const std::string& command) {
        for (size_t i = 1; i < command.size(); ++i) {
            unsigned char c = static_cast<unsigned char>(command[i]);
            if (std::isalpha(c)) {
                return static_cast<char>(c);
            }
        }
        return '\0';
    }

    constexpr std::array<std::pair<int, int>, 4> kCardinalMoves{ {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    } };

}  // namespace

std::mt19937 Game::rng_(std::random_device{}());

// Конструктор игры
Game::Game(int width, int height, int numEnemies, int handCapacity, int separatorLines, int maxLevels)
    : field_(width, height),
    player_(),
    hand_(handCapacity),
    separatorLines_(separatorLines),
    turn_(0),
    level_(1),
    maxLevels_(maxLevels),
    requestExitProgram_(false),
    requestMenuExit_(false),
    baseEnemyHP_(60),
    baseEnemyDamage_(8),
    baseEnemyCount_(numEnemies),
    controlConfig_(nullptr),
    eventObserver_(nullptr),
    lastErrorMessage_() {
    initialize(numEnemies);
}

// Инициализация поля и объектов игры
void Game::initialize(int numEnemies) {
    enemies_.clear();
    allies_.clear();
    towers_.clear();
    traps_.clear();

    int totalCell = field_.getWidth() * field_.getHeight();
    int impassableCount = std::max(1, totalCell / 10);

    std::uniform_int_distribution<int> distX(0, field_.getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, field_.getHeight() - 1);

    for (int i = 0; i < impassableCount; ++i) {
        int ix, iy;
        int guard = 0;
        do {
            ix = distX(rng_);
            iy = distY(rng_);
            ++guard;
            if (guard > 1000) break;
        } while ((ix == 0 && iy == 0) || field_.getCellType(ix, iy) != Cell::Type::Empty);
        if (field_.isInside(ix, iy)) field_.setCellType(ix, iy, Cell::Type::Impassable);
    }

    if (!field_.isInside(0, 0) || field_.getCellType(0, 0) == Cell::Type::Impassable) {
        bool placed = false;
        for (int y = 0; y < field_.getHeight() && !placed; ++y) {
            for (int x = 0; x < field_.getWidth() && !placed; ++x) {
                if (field_.getCellType(x, y) == Cell::Type::Empty) {
                    player_.setPosition(x, y);
                    placed = true;
                }
            }
        }
    }
    else {
        player_.setPosition(0, 0);
    }

    int enemyCount = baseEnemyCount_ + (level_ - 1) * 2;
    if (enemyCount < 1) enemyCount = 1;

    std::uniform_int_distribution<int> exDist(0, field_.getWidth() - 1);
    std::uniform_int_distribution<int> eyDist(0, field_.getHeight() - 1);

    for (int i = 0; i < enemyCount; ++i) {
        int ex, ey;
        int guard = 0;
        do {
            ex = exDist(rng_);
            ey = eyDist(rng_);
            ++guard;
            if (guard > 1000) break;
        } while (!field_.isInside(ex, ey) ||
            field_.getCellType(ex, ey) != Cell::Type::Empty ||
            (ex == player_.getX() && ey == player_.getY()));
        int hp = baseEnemyHP_ + (level_ - 1) * 15;
        int dmg = baseEnemyDamage_ + (level_ - 1) * 5;
        enemies_.emplace_back(ex, ey, hp, dmg);
    }

    towers_.clear();
    int tx = std::max(0, field_.getWidth() - 2);
    int ty = std::max(0, field_.getHeight() - 2);
    towers_.emplace_back(tx, ty, 15 + (level_ - 1) * 5, 4, 60 + (level_ - 1) * 10, 2);

    if (hand_.size() == 0) hand_.drawRandomSpell();
}

const Field& Game::getField() const {
    return field_;
}


// Проверка пустоты клетки для движения
bool Game::isCellEmptyForMove(int x, int y) const {
    if (!field_.isInside(x, y)) return false;
    if (field_.getCellType(x, y) == Cell::Type::Impassable) return false;

    for (const auto& tr : traps_) {
        if (tr.getX() == x && tr.getY() == y) return false;
    }

    if (player_.getX() == x && player_.getY() == y) return false;

    for (const auto& ally : allies_) {
        if (ally.isAlive() && ally.getX() == x && ally.getY() == y) return false;
    }
    for (const auto& enemy : enemies_) {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) return false;
    }
    for (const auto& tower : towers_) {
        if (tower.getX() == x && tower.getY() == y) return false;
    }
    return true;
}

// Установка конфигурации управления
void Game::setControlConfig(ControlConfig* config) {
    controlConfig_ = config;
}

// Преобразование клавиши согласно настройкам
std::pair<char, bool> Game::translateKey(char key) const {
    if (controlConfig_ == nullptr) {
        return std::make_pair(key, false); // Если конфигурации нет, возвращаем клавишу как есть
    }
    
    std::string command = controlConfig_->getCommandForKey(key);
    if (command.empty()) {
        return std::make_pair(key, false); // Команда не найдена, возвращаем клавишу как есть
    }
    
    // Преобразуем команду обратно в стандартную клавишу
    if (command == "move_up") return std::make_pair('w', true);
    if (command == "move_down") return std::make_pair('s', true);
    if (command == "move_left") return std::make_pair('a', true);
    if (command == "move_right") return std::make_pair('d', true);
    if (command == "switch_melee") return std::make_pair('m', true);
    if (command == "switch_ranged") return std::make_pair('r', true);
    if (command == "use_spell") return std::make_pair('u', true);
    if (command == "buy_spell") return std::make_pair('b', true);
    if (command == "menu") return std::make_pair('q', true);
    if (command == "attack") return std::make_pair('f', true);
    
    return std::make_pair(key, true); // Команда найдена, но неизвестная
}

// Обработка движения и атаки игрока
void Game::handlePlayerMove(std::string full_command, char extra_char) {
    if (full_command.empty()) {
        // Сообщение об ошибке должно выводиться через рендерер, а не здесь
        return;
    }

    // Преобразуем первую клавишу согласно настройкам
    auto [cmd, found] = translateKey(full_command[0]);
    // Если конфигурация установлена и команда не найдена в файле - игнорируем
    if (controlConfig_ != nullptr && !found) {
        // Сообщение об ошибке должно выводиться через рендерер, а не здесь
        return;
    }
    if (extra_char != '\0') {
        auto [extra, extraFound] = translateKey(extra_char);
        if (controlConfig_ != nullptr && !extraFound) {
            // Игнорируем направление, если оно не найдено в настройках
            extra_char = '\0';
        } else {
            extra_char = extra;
        }
    }
    switch (cmd) {
    case 'u':
    case 'U': {
        std::istringstream iss(full_command);
        std::string command;
        int index, tx, ty;
        if (iss >> command >> index >> tx >> ty) {
            std::string errorMsg;
            if (hand_.useSpell(index, *this, player_, tx, ty, &errorMsg)) {
                // Логирование использования заклинания
                if (eventObserver_ != nullptr) {
                    eventObserver_->onSpellUsed(index, tx, ty);
                }
            }
            else {
                lastErrorMessage_ = errorMsg;
            }
        }
        else {
            // Сообщение об ошибке должно выводиться через рендерер, а не здесь
        }
        break;
    }
    case 'b':
    case 'B': {
        // Покупка случайного заклинания
        std::uniform_int_distribution<int> spellDist(0, 4);
        int spellType = spellDist(rng_);
        std::unique_ptr<Spell> newSpell;

        switch (spellType) {
        case 0:
            newSpell = std::make_unique<DirectDamageSpell>();
            break;
        case 1:
            newSpell = std::make_unique<AreaDamageSpell>();
            break;
        case 2:
            newSpell = std::make_unique<TrapSpell>();
            break;
        case 3:
            newSpell = std::make_unique<SummonSpell>();
            break;
        default: {
            SpellEnhancement enh;
            enh.extraDamage = 10;
            newSpell = std::make_unique<BuffSpell>(enh);
            break;
        }
        }

        if (newSpell) {
            int cost = newSpell->cost();
            // Информация о покупке заклинания должна выводиться через рендерер, а не здесь
            
            if (hand_.buySpell(std::move(newSpell), player_)) {
                // Логирование покупки заклинания
                if (eventObserver_ != nullptr) {
                    // Получаем имя последнего заклинания в руке
                    const auto& spells = hand_.getSpells();
                    if (!spells.empty() && spells.back()) {
                        std::string spellName = spells.back()->name() ? std::string(spells.back()->name()) : "Unknown";
                        eventObserver_->onSpellBought(spellName);
                    }
                }
            }
            else {
                // Сообщение об ошибке покупки должно выводиться через рендерер, а не здесь
            }
        }
        break;
    }
    case 'w':
    case 'W':
        processMovementOrMeleeAttack(0, -1);
        break;
    case 's':
    case 'S':
        processMovementOrMeleeAttack(0, 1);
        break;
    case 'a':
    case 'A':
        processMovementOrMeleeAttack(-1, 0);
        break;
    case 'd':
    case 'D':
        processMovementOrMeleeAttack(1, 0);
        break;
    case 'm':
    case 'M':
        player_.setAttackMode(Player::AttackMode::Melee);
        // Сообщение о переключении режима должно выводиться через рендерер, а не здесь
        break;
    case 'r':
    case 'R':
        player_.setAttackMode(Player::AttackMode::Ranged);
        // Сообщение о переключении режима должно выводиться через рендерер, а не здесь
        break;
    case 'f':
    case 'F': {
        if (player_.getAttackMode() != Player::AttackMode::Ranged) {
            // Сообщение об ошибке должно выводиться через рендерер, а не здесь
            return;
        }
        char direction = extra_char;
        if (direction == '\0') {
            // Сообщение об ошибке должно выводиться через рендерер, а не здесь
            return;
        }
        int dx = 0, dy = 0;
        if (direction == 'w' || direction == 'W') dy = -1;
        else if (direction == 's' || direction == 'S') dy = 1;
        else if (direction == 'a' || direction == 'A') dx = -1;
        else if (direction == 'd' || direction == 'D') dx = 1;
        else {
            // Сообщение об ошибке должно выводиться через рендерер, а не здесь
            return;
        }
        int px = player_.getX();
        int py = player_.getY();
        int range = player_.getRangedRange();
        for (int step = 1; step <= range; ++step) {
            int tx = px + dx * step;
            int ty = py + dy * step;
            if (!field_.isInside(tx, ty)) break;
            int eidx = findEnemyIndexAt(tx, ty);
            if (eidx != -1) {
                int dmg = player_.getAttackDamage();
                
                // Логирование атаки игрока
                if (eventObserver_ != nullptr) {
                    eventObserver_->onPlayerAttack(dmg, tx, ty);
                }
                
                damageEnemy(eidx, dmg);
                // Сообщение о попадании должно выводиться через рендерер, а не здесь
                return;
            }
            int tidx = findTowerIndexAt(tx, ty);
            if (tidx != -1) {
                int dmg = player_.getAttackDamage();
                
                // Логирование атаки игрока
                if (eventObserver_ != nullptr) {
                    eventObserver_->onPlayerAttack(dmg, tx, ty);
                }
                
                damageTower(tidx, dmg);
                // Сообщение о попадании должно выводиться через рендерер, а не здесь
                return;
            }
        }
        // Сообщение о промахе должно выводиться через рендерер, а не здесь
        break;
    }
    case 'q':
    case 'Q':
        // Сообщение о выходе должно выводиться через рендерер, а не здесь
        break;
    default:
        // Сообщение об ошибке должно выводиться через рендерер, а не здесь
        break;
    }
}

// Процесс движения или атаки ближним боем
void Game::processMovementOrMeleeAttack(int dx, int dy) {
    int newX = player_.getX() + dx;
    int newY = player_.getY() + dy;

    if (!field_.isInside(newX, newY)) {
        lastErrorMessage_ = "Невозможно переместиться за пределы поля.";
        return;
    }

    int eidx = findEnemyIndexAt(newX, newY);
    if (eidx != -1) {
        if (player_.getAttackMode() == Player::AttackMode::Melee) {
            int dmg = player_.getAttackDamage();
            
            // Логирование атаки игрока
            if (eventObserver_ != nullptr) {
                eventObserver_->onPlayerAttack(dmg, newX, newY);
            }
            
            damageEnemy(eidx, dmg);
            // Сообщения о событиях должны выводиться через рендерер, а не здесь
            if (enemies_[eidx].isAlive()) {
                int eDmg = enemies_[eidx].getDamage();
                player_.takeDamage(eDmg);
                
                // Логирование урона игроку
                if (eventObserver_ != nullptr) {
                    eventObserver_->onPlayerDamaged(eDmg);
                }
                
                // Сообщения о событиях должны выводиться через рендерер, а не здесь
            }
            // Сообщения о событиях должны выводиться через рендерер, а не здесь
        }
        else {
            lastErrorMessage_ = "Нельзя атаковать врага в режиме дальнего боя.";
        }
        return;
    }

    int tidx = findTowerIndexAt(newX, newY);
    if (tidx != -1) {
        if (player_.getAttackMode() == Player::AttackMode::Melee) {
            int dmg = player_.getAttackDamage();
            
            // Логирование атаки игрока
            if (eventObserver_ != nullptr) {
                eventObserver_->onPlayerAttack(dmg, newX, newY);
            }
            
            damageTower(tidx, dmg);
            // Сообщения о событиях должны выводиться через рендерер, а не здесь
        }
        else {
            lastErrorMessage_ = "Нельзя атаковать башню в режиме дальнего боя.";
        }
        return;
    }

    if (!isCellEmptyForMove(newX, newY)) {
        lastErrorMessage_ = "Невозможно переместиться в эту клетку.";
        return;
    }

    player_.setPosition(newX, newY);
    
    // Логирование перемещения игрока
    if (eventObserver_ != nullptr) {
        eventObserver_->onPlayerMove(newX, newY);
    }
    
    // Сообщения о событиях должны выводиться через рендерер, а не здесь
}

void Game::setPlayerAttackMode(Player::AttackMode mode) {
    player_.setAttackMode(mode);
    // Сообщение о переключении режима должно выводиться через рендерер, а не здесь
}

bool Game::buyRandomSpell() {
    // Проверяем заполненность руки перед покупкой
    if (hand_.size() >= hand_.getCapacity()) {
        lastErrorMessage_ = "Рука переполнена. Нельзя купить заклинание.";
        return false;
    }
    
    // Покупка случайного заклинания
    std::uniform_int_distribution<int> spellDist(0, 4);
    int spellType = spellDist(rng_);
    std::unique_ptr<Spell> newSpell;
    int cost = 0;

    switch (spellType) {
    case 0:
        newSpell = std::make_unique<DirectDamageSpell>();
        cost = newSpell->cost();
        break;
    case 1:
        newSpell = std::make_unique<AreaDamageSpell>();
        cost = newSpell->cost();
        break;
    case 2:
        newSpell = std::make_unique<TrapSpell>();
        cost = newSpell->cost();
        break;
    case 3:
        newSpell = std::make_unique<SummonSpell>();
        cost = newSpell->cost();
        break;
    default: {
        SpellEnhancement enh;
        enh.extraDamage = 10;
        newSpell = std::make_unique<BuffSpell>(enh);
        cost = newSpell->cost();
        break;
    }
    }

    if (newSpell) {
        // Проверяем очки перед покупкой
        if (player_.getScore() < cost) {
            lastErrorMessage_ = "Недостаточно очков для покупки заклинания.";
            return false;
        }
        
        // Покупаем заклинание (hand_.buySpell проверит очки еще раз и потратит их)
        // Проверка очков уже выполнена выше, поэтому hand_.buySpell должен успешно потратить очки
        if (hand_.buySpell(std::move(newSpell), player_)) {
            // Логирование покупки заклинания
            if (eventObserver_ != nullptr) {
                // Получаем имя последнего заклинания в руке
                const auto& spells = hand_.getSpells();
                if (!spells.empty() && spells.back()) {
                    std::string spellName = spells.back()->name() ? std::string(spells.back()->name()) : "Unknown";
                    eventObserver_->onSpellBought(spellName);
                }
            }
            return true;
        }
        else {
            // Если покупка не удалась после проверок, значит что-то пошло не так
            // Это не должно происходить, так как мы проверили очки и место в руке выше
            // Но на всякий случай выводим общую ошибку
            lastErrorMessage_ = "Не удалось купить заклинание.";
            return false;
        }
    }
    return false;
}

bool Game::useSpell(int index, int tx, int ty) {
    std::string errorMsg;
    if (hand_.useSpell(index, *this, player_, tx, ty, &errorMsg)) {
        // Логирование использования заклинания
        if (eventObserver_ != nullptr) {
            eventObserver_->onSpellUsed(index, tx, ty);
        }
        return true;
    }
    else {
        lastErrorMessage_ = errorMsg;
        return false;
    }
}

void Game::performRangedAttack(char direction) {
    if (player_.getAttackMode() != Player::AttackMode::Ranged) {
        lastErrorMessage_ = "Выстрел возможен только в режиме дальнего боя.";
        return;
    }
    if (direction == '\0') {
        lastErrorMessage_ = "Команда 'f' требует направление.";
        return;
    }
    int dx = 0, dy = 0;
    if (direction == 'w' || direction == 'W') dy = -1;
    else if (direction == 's' || direction == 'S') dy = 1;
    else if (direction == 'a' || direction == 'A') dx = -1;
    else if (direction == 'd' || direction == 'D') dx = 1;
    else {
        lastErrorMessage_ = "Неверное направление для выстрела.";
        return;
    }
    int px = player_.getX();
    int py = player_.getY();
    int range = player_.getRangedRange();
    for (int step = 1; step <= range; ++step) {
        int tx = px + dx * step;
        int ty = py + dy * step;
        if (!field_.isInside(tx, ty)) break;
        int eidx = findEnemyIndexAt(tx, ty);
        if (eidx != -1) {
            int dmg = player_.getAttackDamage();
            
            // Логирование атаки игрока
            if (eventObserver_ != nullptr) {
                eventObserver_->onPlayerAttack(dmg, tx, ty);
            }
            
            damageEnemy(eidx, dmg);
            // Сообщение о попадании должно выводиться через рендерер, а не здесь
            return;
        }
        int tidx = findTowerIndexAt(tx, ty);
        if (tidx != -1) {
            int dmg = player_.getAttackDamage();
            
            // Логирование атаки игрока
            if (eventObserver_ != nullptr) {
                eventObserver_->onPlayerAttack(dmg, tx, ty);
            }
            
            damageTower(tidx, dmg);
            // Сообщение о попадании должно выводиться через рендерер, а не здесь
            return;
        }
    }
    // Сообщение о промахе должно выводиться через рендерер, а не здесь
}

bool Game::run() {
    requestExitProgram_ = false;
    requestMenuExit_ = false;
    turn_ = 0;

    while (true) {
        if (!player_.isAlive()) {
            std::cout << "Вы погибли. 1) Повторить уровень  2) Меню  3) Выход из программы\n> ";
            std::string answer;
            if (!std::getline(std::cin, answer)) {
                requestExitProgram_ = true;
                break;
            }
            const auto choice = toLowerCopy(trim(answer));
            if (choice == "1" || choice == "повтор" || choice == "retry") {
                field_ = Field(field_.getWidth(), field_.getHeight());
                player_.restoreToFullHP();
                initialize(baseEnemyCount_ + (level_ - 1) * 2);
                continue;
            }
            if (choice == "2" || choice == "меню" || choice == "menu") {
                return false;
            }
            if (choice == "3" || choice == "exit" || choice == "выход") {
                requestExitProgram_ = true;
                break;
            }
            std::cout << "Неизвестная команда.\n";
            continue;
        }

        if (isLevelCleared()) {
            std::cout << "Уровень " << level_ << " пройден!\n";
            presentUpgradeMenu();
            advanceToNextLevel();
            if (requestExitProgram_) {
                break;
            }
            continue;
        }

        // Примечание: Отрисовка теперь выполняется через GameRenderer в GameLoop
        // Метод run() оставлен для обратной совместимости, но не используется в новой архитектуре
        // Выводим подсказку с правильными командами из файла настроек
        if (controlConfig_ != nullptr) {
            auto bindings = controlConfig_->getAllBindings();
            char help = bindings.count("help") ? bindings.at("help") : 'h';
            char save = bindings.count("save") ? bindings.at("save") : 'z';
            char load = bindings.count("load") ? bindings.at("load") : 'l';
            char menu = bindings.count("menu") ? bindings.at("menu") : 'q';
            char exit = bindings.count("exit") ? bindings.at("exit") : 'x';
            std::cout << "Введите команду (" << help << "/" << save << "/" << load << "/" << menu << "/" << exit << "): ";
        } else {
            std::cout << "Введите команду (help/save/load/menu/exit): ";
        }
        std::string fullCommand;
        if (!std::getline(std::cin, fullCommand)) {
            requestExitProgram_ = true;
            break;
        }
        const std::string trimmed = trim(fullCommand);
        if (trimmed.empty()) {
            continue;
        }
        const std::string lowered = toLowerCopy(trimmed);
        
        // Если конфигурация установлена и введен один символ - проверяем команду из файла
        if (controlConfig_ != nullptr && trimmed.length() == 1) {
            char key = trimmed[0];
            std::string command = controlConfig_->getCommandForKey(key);
            if (!command.empty()) {
                // Обрабатываем команды save, load, menu, exit, help из файла настроек
                if (command == "save") {
                    std::cout << "Укажите имя файла для сохранения: ";
                    std::string filename;
                    if (std::getline(std::cin, filename)) {
                        filename = trim(filename);
                        if (filename.empty()) {
                            filename = "game.sav";
                        }
                        try {
                            saveToFile(filename);
                            std::cout << "Игра сохранена в '" << filename << "'.\n";
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Не удалось сохранить игру: " << e.what() << "\n";
                        }
                    }
                    continue;
                }
                if (command == "load") {
                    std::cout << "Укажите имя файла для загрузки: ";
                    std::string filename;
                    if (std::getline(std::cin, filename)) {
                        filename = trim(filename);
                        if (filename.empty()) {
                            std::cout << "Имя файла не указано.\n";
                            continue;
                        }
                        try {
                            loadFromFile(filename);
                            std::cout << "Игра загружена из '" << filename << "'.\n";
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Не удалось загрузить игру: " << e.what() << "\n";
                        }
                    }
                    continue;
                }
                if (command == "menu") {
                    return false;
                }
                if (command == "exit") {
                    requestExitProgram_ = true;
                    break;
                }
                if (command == "help") {
                    // Используем HelpCommand для вывода справки с правильными командами
                    HelpCommand helpCmd;
                    helpCmd.execute(*this);
                    continue;
                }
                // Если команда найдена, но это не save/load/menu/exit/help - обрабатываем через handlePlayerMove
                const char extraChar = extractDirectionChar(fullCommand);
                handlePlayerMove(fullCommand, extraChar);
                alliesTurn();
                enemiesTurn();
                ++turn_;
                continue;
            }
            // Если команда не найдена в файле настроек - игнорируем базовые команды
            std::cout << "Команда не найдена в настройках управления.\n";
            continue;
        }
        
        // Обработка строковых команд (только если конфигурация НЕ установлена или команда длиннее одного символа)
        if (lowered == "help") {
            std::cout << "Команды: w/a/s/d - движение; f+направление - дальняя атака; "
                "m/r - сменить режим; u <index> <x> <y> - использовать заклинание; "
                "save <file>; load <file>; menu - вернуться в меню; exit - выйти из программы.\n";
            continue;
        }
        if (lowered == "exit") {
            requestExitProgram_ = true;
            break;
        }
        if (lowered == "menu" || lowered == "q" || lowered == "quit") {
            return false;
        }
        if (lowered.rfind("save", 0) == 0) {
            std::string filename = extractArgument(trimmed);
            if (filename.empty()) {
                filename = "game.sav";
            }
            try {
                saveToFile(filename);
                std::cout << "Игра сохранена в '" << filename << "'.\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Не удалось сохранить игру: " << e.what() << "\n";
            }
            continue;
        }
        if (lowered.rfind("load", 0) == 0) {
            const std::string filename = extractArgument(trimmed);
            if (filename.empty()) {
                std::cout << "Укажите имя файла для загрузки.\n";
            }
            else {
                try {
                    loadFromFile(filename);
                    std::cout << "Игра загружена из '" << filename << "'.\n";
                }
                catch (const std::exception& e) {
                    std::cerr << "Не удалось загрузить игру: " << e.what() << "\n";
                }
            }
            continue;
        }

        const char extraChar = extractDirectionChar(fullCommand);
        handlePlayerMove(fullCommand, extraChar);
        alliesTurn();
        enemiesTurn();
        ++turn_;
    }

    return requestExitProgram_;
}

void Game::alliesTurn() {
    for (auto& ally : allies_) {
        if (!ally.isAlive()) {
            continue;
        }

        bool attacked = false;
        for (const auto& [dx, dy] : kCardinalMoves) {
            const int tx = ally.getX() + dx;
            const int ty = ally.getY() + dy;
            const int eIdx = findEnemyIndexAt(tx, ty);
            if (eIdx != -1) {
                damageEnemy(eIdx, ally.getDamage());
                attacked = true;
                break;
            }
        }
        if (attacked) {
            continue;
        }

        std::array<int, 4> order{ 0, 1, 2, 3 };
        std::shuffle(order.begin(), order.end(), rng_);
        for (int dirIndex : order) {
            const int nx = ally.getX() + kCardinalMoves[dirIndex].first;
            const int ny = ally.getY() + kCardinalMoves[dirIndex].second;
            if (isCellEmptyForMove(nx, ny)) {
                ally.setPosition(nx, ny);
                break;
            }
        }
    }
}

void Game::enemiesTurn() {
    auto handleTrap = [this](Enemy& enemy) {
        auto trapIt = std::find_if(traps_.begin(), traps_.end(),
            [&enemy](const Trap& tr) {
                return tr.getX() == enemy.getX() && tr.getY() == enemy.getY();
            });
        if (trapIt != traps_.end()) {
            enemy.takeDamage(trapIt->getDamage());
            traps_.erase(trapIt);
            if (!enemy.isAlive()) {
                player_.addScore(5);
                // Сообщение о событии логируется через GameEventObserver
                // Вывод должен быть в рендерере, а не здесь
            }
        }
        };

    // Направления для случайного движения: вверх, вниз, влево, вправо
    const std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(0, -1),  // вверх
        std::make_pair(0, 1),   // вниз
        std::make_pair(-1, 0),  // влево
        std::make_pair(1, 0)    // вправо
    };

    for (auto& enemy : enemies_) {
        if (!enemy.isAlive()) {
            continue;
        }

        // Всегда случайное движение (как в лабе 3)
        auto tryMove = [&](int nx, int ny) {
            if (!field_.isInside(nx, ny)) return false;
            if (!isCellEmptyForMove(nx, ny)) return false;
            enemy.setPosition(nx, ny);
            handleTrap(enemy);
            return true;
            };

        // Перемешиваем направления для случайности
        std::array<std::pair<int, int>, 4> shuffledDirs = directions;
        std::shuffle(shuffledDirs.begin(), shuffledDirs.end(), rng_);

        // Пробуем переместиться в случайном направлении
        bool moved = false;
        for (const auto& [mx, my] : shuffledDirs) {
            if (tryMove(enemy.getX() + mx, enemy.getY() + my)) {
                moved = true;
                break;
            }
        }
    }

    for (auto& tower : towers_) {
        if (!tower.isAlive()) continue;
        if (turn_ - tower.getLastAttackTurn() < tower.getAttackCooldown()) continue;

        const int dist = std::abs(tower.getX() - player_.getX()) + std::abs(tower.getY() - player_.getY());
        if (dist <= tower.getRange()) {
            const int dmg = std::max(1, tower.getDamage() / 2);
            player_.takeDamage(dmg);
            tower.setLastAttackTurn(turn_);
            // Логирование урона игроку от башни
            if (eventObserver_ != nullptr) {
                eventObserver_->onPlayerDamaged(dmg);
            }
            // Сообщение о событии логируется через GameEventObserver
            // Вывод должен быть в рендерере, а не здесь
        }
    }
}

bool Game::isLevelCleared() const {
    return std::none_of(enemies_.begin(), enemies_.end(),
        [](const Enemy& e) { return e.isAlive(); });
}

void Game::advanceToNextLevel() {
    if (level_ >= maxLevels_) {
        // Сообщение о прохождении всех уровней должно выводиться через рендерер, а не здесь
        requestExitProgram_ = true;
        return;
    }

    ++level_;
    int newWidth = std::clamp(field_.getWidth() + 1 + (level_ % 2), 10, 25);
    int newHeight = std::clamp(field_.getHeight() + 1 + ((level_ + 1) % 2), 10, 25);
    field_ = Field(newWidth, newHeight);

    player_.restoreToFullHP();
    player_.setAttackMode(Player::AttackMode::Melee);
    hand_.removeRandomHalf();
    initialize(baseEnemyCount_ + (level_ - 1) * 2);
    turn_ = 0;
}

void Game::presentUpgradeMenu() {
    UpgradeMenu::present(player_);
}

void Game::handlePlayerDeath() {
    // Сообщение о смерти игрока и запрос выбора должны выводиться через рендерер
    // Интерактивный ввод здесь необходим для обработки выбора игрока
    // В идеале это должно быть через рендерер, но для упрощения оставляем здесь
    // Вывод сообщения должен быть в рендерере
    std::string answer;
    if (!std::getline(std::cin, answer)) {
        requestExitProgram_ = true;
        return;
    }
    const auto choice = toLowerCopy(trim(answer));
    if (choice == "1" || choice == "повтор" || choice == "retry") {
        field_ = Field(field_.getWidth(), field_.getHeight());
        player_.restoreToFullHP();
        initialize(baseEnemyCount_ + (level_ - 1) * 2);
    }
    else if (choice == "2" || choice == "меню" || choice == "menu") {
        requestMenuExit_ = true;
    }
    else if (choice == "3" || choice == "exit" || choice == "выход") {
        requestExitProgram_ = true;
    }
    else {
        // Сообщение об ошибке должно выводиться через рендерер, а не здесь
    }
}

void Game::handleLevelCleared() {
    // Логирование прохождения уровня
    if (eventObserver_) {
        eventObserver_->onLevelCleared(level_);
    }
    // Сообщение о прохождении уровня должно выводиться через рендерер
    // Вызов меню прокачки (UpgradeMenu::present использует cout, но это отдельный компонент)
    presentUpgradeMenu();
    advanceToNextLevel();
}
