#include "console_input_reader.h"

#include <iostream>
#include <limits>

ConsoleInputReader::ConsoleInputReader(GameController& ctrl)
    : controller(ctrl) {
}

MoveDirection ConsoleInputReader::getMoveInput(bool& quitRequested) {
    std::cout << "Move (w/a/s/d), c=cast spell, b=shop, e=save, l=load, q=quit: ";
    char ch;
    std::cin >> ch;
    clearBuffer();

    switch (ch) {
    case 'w': case 'W': return MoveDirection::UP;
    case 's': case 'S': return MoveDirection::DOWN;
    case 'a': case 'A': return MoveDirection::LEFT;
    case 'd': case 'D': return MoveDirection::RIGHT;
    case 'c': case 'C': return MoveDirection::CAST_SPELL;
    case 'b': case 'B': return MoveDirection::OPEN_SHOP;
    case 'e': case 'E':
        if (controller.saveGame("autosave.bin")) {
            std::cout << "Game autosaved!\n";
        }
        else {
            std::cout << "Save failed!\n";
        }
        return getMoveInput(quitRequested); 
    case 'l': case 'L':
        if (controller.loadGame("autosave.bin")) {
            std::cout << "Game loaded!\n";
        }
        else {
            std::cout << "No save file or load failed.\n";
        }
        return getMoveInput(quitRequested);
    case 'q': case 'Q':
        quitRequested = true;
        return MoveDirection::UP; 
    default:
        std::cout << "Invalid command! Use w/a/s/d, c, b, e, l, q.\n";
        return getMoveInput(quitRequested);
    }
}

int ConsoleInputReader::chooseSpellIndex(size_t maxSpells) const {
    if (maxSpells == 0) {
        std::cout << "Your hand is empty! Buy spells in the shop first.\n";
        return -1;
    }

    std::cout << "Choose spell to cast (1-" << maxSpells << ") or 0 to cancel: ";
    char ch;
    std::cin >> ch;
    clearBuffer();

    if (ch == '0') {
        std::cout << "Casting cancelled.\n";
        return -1;
    }

    if (ch >= '1' && ch <= '9') {
        int index = ch - '1';
        if (static_cast<size_t>(index) < maxSpells) {
            return index;
        }
    }

    std::cout << "Invalid choice!\n";
    return -1;
}

Position ConsoleInputReader::chooseTarget() const {
    int x, y;
    std::cout << "Enter target coordinates (x y): ";
    std::cin >> x >> y;
    clearBuffer();
    return Position(x, y);
}

int ConsoleInputReader::chooseShopItem(size_t maxItems) const {
    if (maxItems == 0) {
        std::cout << "Shop is empty!\n";
        return -1;
    }

    std::cout << "Choose spell to buy (1-" << maxItems << ") or 0 to cancel: ";
    char ch;
    std::cin >> ch;
    clearBuffer();

    if (ch == '0') {
        std::cout << "Purchase cancelled.\n";
        return -1;
    }

    if (ch >= '1' && ch <= '9') {
        int index = ch - '1';
        if (static_cast<size_t>(index) < maxItems) {
            return index;
        }
    }

    std::cout << "Invalid choice!\n";
    return -1;
}

bool ConsoleInputReader::confirmQuitWithSave() const {
    std::cout << "Quit the game? Save first? (y = quit without save, s = save and quit, n = cancel): ";
    char confirm;
    std::cin >> confirm;
    clearBuffer();

    if (confirm == 's' || confirm == 'S') {
        if (controller.saveGame("autosave.bin")) {
            std::cout << "Game saved. Goodbye!\n";
        }
        else {
            std::cout << "Save failed, but quitting anyway.\n";
        }
        return true;
    }

    if (confirm == 'y' || confirm == 'Y') {
        std::cout << "Goodbye!\n";
        return true;
    }

    std::cout << "Quit cancelled.\n";
    return false;
}

void ConsoleInputReader::clearBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}