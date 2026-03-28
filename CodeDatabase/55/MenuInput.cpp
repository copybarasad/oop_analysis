#include "MenuInput.h"
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

bool MenuInput::safeInputChar(char& out) {
    if (!(cin >> out)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool MenuInput::safeInputInt(int& out) {
    if (!(cin >> out)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[41mОшибка ввода значений!\033[0m" << endl;
        return false;
    }
    return true;
}

std::optional<Command> MenuInput::handleMainMenu() {
    if (firstMainMenu_) {
        firstMainMenu_ = false;
    }

    char choice;
    do {
        cout << "\033[34mВыберите действие:\033[0m ";
    } while (!safeInputChar(choice));

    string funcs = "nlqc";
    if (funcs.find(choice) == string::npos) {
        cout << "\033[41mНеизвестная команда\033[0m" << endl;
        firstMainMenu_ = true;
        return std::nullopt;
    }

    switch (choice) {
        case 'n':
            return Command{CommandID::StartNewGame, 0, 0, 0, 0, 1};
        case 'l':
            return Command{CommandID::LoadGame};
        case 'c':
            state_ = State::LevelsMenu;
            firstLevelsMenu_ = true;
            return getNextCommand();
        case 'q':
            return Command{CommandID::Quit};
        default:
            return std::nullopt;
    }
}

std::optional<Command> MenuInput::handleLevelsMenu() {
    if (firstLevelsMenu_) {
        firstLevelsMenu_ = false;

        cout << "\033[44m-------- УРОВНИ ДЛЯ ПРОХОЖДЕНИЯ --------\033[0m" << endl << endl;
        for (int i = 1; i <= 3; i++) {
            cout << "\033[33m================================\033[0m" << endl;
            cout << i << "\033[34m: " << "уровень <<level_" << i << ">>\033[0m" << endl;
            cout << "\033[33m================================\033[0m" << endl;
        }
    }

    int choice;
    do {
        cout << "Выберите уровень: " << endl;
    } while (!safeInputInt(choice));

    if (choice >= 1 && choice <= 3) {
        state_ = State::MainMenu;
        firstMainMenu_ = true;
        Command cmd{CommandID::SelectLevel};
        cmd.levelIndex = choice;
        return cmd;
    } else {
        cout << "\033[41mОшибка ввода значений!\033[0m" << endl;
        firstLevelsMenu_ = true;
        return std::nullopt;
    }
}

std::optional<Command> MenuInput::getNextCommand() {
    switch (state_) {
        case State::MainMenu:       return handleMainMenu();
        case State::LevelsMenu:     return handleLevelsMenu();
        default:
            state_ = State::MainMenu;
            firstMainMenu_ = true;
            return handleMainMenu();
    }
}

void MenuInput::reset() {
        state_ = State::MainMenu;
        firstMainMenu_ = true;
        firstLevelsMenu_ = true;
    }