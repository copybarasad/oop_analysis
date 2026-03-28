#include "InputHandler.h"
#include <iostream>
#include <limits>
#include <cctype>

InputHandler::InputHandler() : key_bindings_() {
    // KeyBindings инициализируется с дефолтными значениями
}

void InputHandler::LoadKeyBindings(const std::string& filename) {
    key_bindings_.LoadFromFile(filename);
}

void InputHandler::SetKeyBindings(const KeyBindings& bindings) {
    key_bindings_ = bindings;
}

bool InputHandler::ReadInt(int& value) {
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

bool InputHandler::ReadChar(char& c) {
    if (!(std::cin >> c)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

std::unique_ptr<Command> InputHandler::CharToCommand(char c) {
    CommandType cmd_type = key_bindings_.GetCommandType(c);
    
    if (key_bindings_.IsMoveCommand(cmd_type)) {
        auto [dx, dy] = key_bindings_.GetMoveOffset(cmd_type);
        return std::make_unique<MoveCommand>(cmd_type, dx, dy);
    }
    
    return std::make_unique<Command>(cmd_type);
}

std::unique_ptr<Command> InputHandler::ReadGameCommand() {
    char c;
    if (!ReadChar(c)) {
        return std::make_unique<Command>(CommandType::NONE);
    }
    return CharToCommand(c);
}

std::unique_ptr<MenuCommand> InputHandler::ReadMenuChoice() {
    int choice;
    if (!ReadInt(choice)) {
        return std::make_unique<MenuCommand>(-1);
    }
    return std::make_unique<MenuCommand>(choice);
}

std::unique_ptr<Command> InputHandler::ReadYesNo() {
    char c;
    if (!ReadChar(c)) {
        return std::make_unique<Command>(CommandType::NONE);
    }
    
    c = std::tolower(c);
    if (c == 'y') {
        return std::make_unique<Command>(CommandType::YES);
    } else if (c == 'n') {
        return std::make_unique<Command>(CommandType::NO);
    }
    return std::make_unique<Command>(CommandType::NONE);
}

std::unique_ptr<TextInputCommand> InputHandler::ReadFileName() {
    std::string filename;
    std::getline(std::cin >> std::ws, filename);

    if (filename.empty()) {
        return std::make_unique<TextInputCommand>(CommandType::NONE, "");
    }

    for (char c : filename) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-' && c != ' ') {
            return std::make_unique<TextInputCommand>(CommandType::NONE, "");
        }
    }

    return std::make_unique<TextInputCommand>(CommandType::MENU_SELECT, filename);
}

std::optional<int> InputHandler::ReadSpellIndex(int max_index) {
    int index;
    if (!ReadInt(index)) {
        return std::nullopt;
    }
    if (index < 0 || index >= max_index) {
        return std::nullopt;
    }
    return index;
}

std::optional<int> InputHandler::ReadSaveSlot(int save_count) {
    int slot;
    if (!ReadInt(slot)) {
        return std::nullopt;
    }
    if (slot < 0 || slot >= save_count) {
        return std::nullopt;
    }
    return slot;
}
