#include "ConsoleInputManager.h"
#include <unistd.h>
#include "JsonLoadHandler.h"
#include "Player.h"
#include "Shop.h"
#include "Commands/LoadGameCommand.h"
#include "Commands/NewGameCommand.h"
#include "Commands/QuitGameCommand.h"
#include "Commands/QuitMenuCommand.h"
#include "Commands/ChangeAttackModeCommand.h"
#include "Commands/CancelCommand.h"
#include "Commands/DirectionCommand.h"
#include "Commands/ShootCommand.h"
#include "Commands/ShopCommand.h"
#include "Commands/SpellSelectCommand.h"
#include "Commands/LoadFromGameCommand.h"
#include "Commands/SaveGameCommand.h"

ConsoleInputManager::ConsoleInputManager() : originalTerminalSettings() {
    tcgetattr(STDIN_FILENO, &originalTerminalSettings);
    termios newTerminalSettings = originalTerminalSettings;
    newTerminalSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminalSettings);
}

ConsoleInputManager::~ConsoleInputManager() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTerminalSettings);
}

std::string ConsoleInputManager::setupMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) {
    try {
        loadMenuControls(running, game, changeMode);
    } catch (std::exception& e) {
        setDefaultMenuControls(running, game, changeMode);
        return std::string(e.what()) + "Загружены настройки управления меню по умолчанию";
    }
    return "";
}

void ConsoleInputManager::loadMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) {
    controls.clear();
    bindsInfo.clear();
    JsonLoadHandler file("menu_controls.json");
    json saved_controls = file.load();

    std::vector<std::pair<std::string, std::function<std::shared_ptr<Command>(char)>>> binds = {
        { "Новая игра",  [&](char c) {
            bindsInfo += std::string(1, c) + " - Начать новую игру\n";
            return std::make_shared<NewGameCommand>(game, changeMode);
        } },
        { "Загрузить игру", [&](char c) {
            bindsInfo += std::string(1, c) + " - Загрузить игру\n";
            return std::make_shared<LoadGameCommand>(game, changeMode);
        } },
        { "Выйти из программы", [&](char c) {
            bindsInfo += std::string(1, c) + " - Завершить программу\n";
            return std::make_shared<QuitMenuCommand>(running);
        } }
    };

    for (const auto& bind : binds) {
        if (!saved_controls.contains(bind.first))
            throw std::invalid_argument("Ключ " + bind.first + " отсутствует в сохранённом управлении меню\n");
        if (!saved_controls[bind.first].is_string())
            throw std::invalid_argument("Значение ключа " + bind.first + " не является символом в сохранённом управлении меню\n");
        char c = saved_controls[bind.first].get<std::string>()[0];
        if (c > 127)
            throw std::invalid_argument("Значение ключа " + bind.first + " содержит некорректный символ\n");
        c = static_cast<char>(std::tolower(c));
        if (controls.contains(c))
            throw std::invalid_argument("На клавишу " + std::string(1, c) + " назначено более одной команды");
        controls[c] = bind.second(c);
    }
}

void ConsoleInputManager::setDefaultMenuControls(bool& running, std::shared_ptr<Game>& game, bool& changeMode) {
    controls.clear();
    bindsInfo.clear();
    bindsInfo += "1 - Начать новую игру\n";
    controls['1'] = std::make_shared<NewGameCommand>(game, changeMode);
    bindsInfo += "2 - Загрузить программу\n";
    controls['2'] = std::make_shared<LoadGameCommand>(game, changeMode);
    bindsInfo += "3 - Завершить программу\n";
    controls['3'] = std::make_shared<QuitMenuCommand>(running);
}

std::string ConsoleInputManager::setupGameControls(std::shared_ptr<Game>& game, bool& changeMode) {
    try {
        loadGameControls(game, changeMode);
    } catch (std::exception& e) {
        setDefaultGameControls(game, changeMode);
        return std::string(e.what()) + "\nЗагружены настройки управления игрой по умолчанию";
    }
    return "";
}

void ConsoleInputManager::loadGameControls(std::shared_ptr<Game>& game, bool& changeMode) {
    Shop& shop = game->getShop();

    controls.clear();
    bindsInfo.clear();
    JsonLoadHandler file("game_controls.json");
    json saved_controls = file.load();

    std::vector<std::pair<std::string, std::function<std::shared_ptr<Command>(char)>>> binds = {
        { "Перемещение вверх", [&](char c) {
            bindsInfo += std::string(1, c) + " - Перемещение вверх\n";
            return std::make_shared<DirectionCommand>(UP,  game);
        } },
        { "Перемещение влево", [&](char c) {
            bindsInfo += std::string(1, c) + " - Перемещение влево\n";
            return std::make_shared<DirectionCommand>(LEFT,  game);
        } },
        { "Перемещение вниз", [&](char c) {
            bindsInfo += std::string(1, c) + " - Перемещение вниз\n";
            return std::make_shared<DirectionCommand>(DOWN,  game);
        } },
        { "Перемещение вправо", [&](char c) {
            bindsInfo += std::string(1, c) + " - Перемещение вправо\n";
            return std::make_shared<DirectionCommand>(RIGHT,  game);
        } },
        { "Заклинание 1", [&](char c) {
            shop.bindItem(0, c);
            bindsInfo += std::string(1, c) + " - Заклинание 1\n";
            return std::make_shared<SpellSelectCommand>(0,  game);
        } },
        { "Заклинание 2", [&](char c) {
            shop.bindItem(1, c);
            bindsInfo += std::string(1, c) + " - Заклинание 2\n";
            return std::make_shared<SpellSelectCommand>(1,  game);
        } },
        { "Заклинание 3", [&](char c) {
            shop.bindItem(2, c);
            bindsInfo += std::string(1, c) + " - Заклинание 3\n";
            return std::make_shared<SpellSelectCommand>(2,  game);
        } },
        { "Заклинание 4", [&](char c) {
            shop.bindItem(3, c);
            bindsInfo += std::string(1, c) + " - Заклинание 4\n";
            return std::make_shared<SpellSelectCommand>(3,  game);
        } },
        { "Заклинание 5", [&](char c) {
            shop.bindItem(4, c);
            bindsInfo += std::string(1, c) + " - Заклинание 5\n";
            return std::make_shared<SpellSelectCommand>(4,  game);
        } },
        { "Заклинание 6", [&](char c) {
            shop.bindItem(5, c);
            bindsInfo += std::string(1, c) + " - Заклинание 6\n";
            return std::make_shared<SpellSelectCommand>(5,  game);
        } },
        { "Заклинание 7", [&](char c) {
            shop.bindItem(6, c);
            bindsInfo += std::string(1, c) + " - Заклинание 7\n";
            return std::make_shared<SpellSelectCommand>(6,  game);
        } },
        { "Заклинание 8", [&](char c) {
            shop.bindItem(7, c);
            bindsInfo += std::string(1, c) + " - Заклинание 8\n";
            return std::make_shared<SpellSelectCommand>(7,  game);
        } },
        { "Заклинание 9", [&](char c) {
            shop.bindItem(8, c);
            bindsInfo += std::string(1, c) + " - Заклинание 9\n";
            return std::make_shared<SpellSelectCommand>(8,  game);
        } },
        { "Заклинание 10", [&](char c) {
            shop.bindItem(9, c);
            bindsInfo += std::string(1, c) + " - Заклинание 10\n";
            return std::make_shared<SpellSelectCommand>(9, game);
        } },
        { "Сменить режим атаки", [&](char c) {
            bindsInfo += std::string(1, c) + " - Сменить режим атаки\n";
            return std::make_shared<ChangeAttackModeCommand>(game);
        } },
        { "Выстрелить/применить заклинание", [&](char c) {
            bindsInfo += std::string(1, c) + " - Выстрелить/применить заклинание\n";
            return std::make_shared<ShootCommand>(game);
        } },
        { "Магазин", [&](char c) {
            bindsInfo += std::string(1, c) + " - Магазин\n";
            return std::make_shared<ShopCommand>(game);
        } },
        { "Отмена заклинания", [&](char c) {
            bindsInfo += std::string(1, c) + " - Отмена заклинания\n";
            return std::make_shared<CancelCommand>(game);
        } },
        { "Выход из игры с сохранением", [&](char c) {
            bindsInfo += std::string(1, c) + " - Выход из игры с сохранением\n";
            return std::make_shared<QuitGameCommand>(game, changeMode);
        } },
    { "Сохранение игры", [&](char c) {
        bindsInfo += std::string(1, c) + " - Сохранение игры\n";
        return std::make_shared<SaveGameCommand>(game, changeMode);
        } },
        { "Загрузка игры", [&](char c) {
            bindsInfo += std::string(1, c) + " - Загрузка игры\n";
            return std::make_shared<LoadFromGameCommand>(game, changeMode);
        } }
    };

    for (const auto& bind : binds) {
        if (!saved_controls.contains(bind.first))
            throw std::invalid_argument("Ключ " + bind.first + " отсутствует в сохранённом управлении игры");
        if (!saved_controls[bind.first].is_string())
            throw std::invalid_argument("Значение ключа " + bind.first + " не является символом в сохранённом управлении игры");
        char c = saved_controls[bind.first].get<std::string>()[0];
        if (c > 127)
            throw std::invalid_argument("Значение ключа " + bind.first + " содержит некорректный символ");
        c = static_cast<char>(std::tolower(c));
        if (controls.contains(c))
            throw std::invalid_argument("На клавишу " + std::string(1, c) + " назначено более одной команды");
        controls[c] = bind.second(c);
    }
}

void ConsoleInputManager::setDefaultGameControls(std::shared_ptr<Game>& game, bool& changeMode) {
    Shop& shop = game->getShop();

    controls.clear();
    bindsInfo.clear();
    bindsInfo += "1-0 - Выбрать заклинание\n";
    for (char i = 0; i < 10; i++) {
        char symbol = (1 + i) % 10 + '0';
        shop.bindItem(i, symbol);
        controls[symbol] = std::make_shared<SpellSelectCommand>(i, game);;
    }
    bindsInfo += "wasd - Перемещение вверх/влево/вниз/вправо\n";
    controls['w'] = std::make_shared<DirectionCommand>(UP, game);
    controls['a'] = std::make_shared<DirectionCommand>(LEFT, game);
    controls['s'] = std::make_shared<DirectionCommand>(DOWN, game);
    controls['d'] = std::make_shared<DirectionCommand>(RIGHT, game);

    bindsInfo += "f - Сменить режим атаки\n";
    controls['f'] = std::make_shared<ChangeAttackModeCommand>(game);
    bindsInfo += "space - Выстрелить/применить заклинание\n";
    controls[' '] = std::make_shared<ShootCommand>(game);
    bindsInfo += "c - Отмена заклинания\n";
    controls['c'] = std::make_shared<CancelCommand>(game);
    bindsInfo += "b - Магазин\n";
    controls['b'] = std::make_shared<ShopCommand>(game);

    bindsInfo += "q - Выход из игры с сохранением\n";
    controls['q'] = std::make_shared<QuitGameCommand>(game, changeMode);
    bindsInfo += "o - Сохранение игры\n";
    controls['o'] = std::make_shared<SaveGameCommand>(game, changeMode);
    bindsInfo += "l - Загрузка игры\n";
    controls['l'] = std::make_shared<LoadFromGameCommand>(game, changeMode);
}

std::string& ConsoleInputManager::getControls() { return bindsInfo; }

Command* ConsoleInputManager::getCommand() {
    char c = 0;
    tcflush(STDIN_FILENO, TCIFLUSH);
    read(STDIN_FILENO, &c, 1);
    c = static_cast<char>(std::tolower(c));
    if (controls.contains(c))
        return controls[c].get();
    return nullptr;
}

