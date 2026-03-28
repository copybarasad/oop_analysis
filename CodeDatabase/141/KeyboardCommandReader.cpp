#include "KeyboardCommandReader.h"
#include <cctype>
#include <iostream>

KeyboardCommandReader::KeyboardCommandReader(const ControlMapping &mapping, InputSystem &input)
    : controls(mapping), inputSystem(input) {
}

Command KeyboardCommandReader::readCommand() {
    char key = inputSystem.getInput();
    Command cmd;
    cmd.type = controls.commandForKey(key);
    return cmd;
}

Direction KeyboardCommandReader::readDirection() {
    while (true) {
        char dir = inputSystem.getInput();
        CommandType ct = controls.commandForKey(dir);
        switch (ct) {
            case CommandType::MOVE_UP: return {0, -1};
            case CommandType::MOVE_LEFT: return {-1, 0};
            case CommandType::MOVE_DOWN: return {0, 1};
            case CommandType::MOVE_RIGHT: return {1, 0};
            default:
                break;
        }
    }
}

int KeyboardCommandReader::readIndex() {
    int value = -1;
    if (std::cin >> value) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return -1;
}

std::string KeyboardCommandReader::directionPrompt() const {
    char up = controls.keyFor(CommandType::MOVE_UP);
    char down = controls.keyFor(CommandType::MOVE_DOWN);
    char left = controls.keyFor(CommandType::MOVE_LEFT);
    char right = controls.keyFor(CommandType::MOVE_RIGHT);
    if (up == '\0') up = 'W';
    if (down == '\0') down = 'S';
    if (left == '\0') left = 'A';
    if (right == '\0') right = 'D';
    std::string prompt = "Направление (";
    prompt.push_back(up);
    prompt += "/";
    prompt.push_back(down);
    prompt += "/";
    prompt.push_back(left);
    prompt += "/";
    prompt.push_back(right);
    prompt += "): ";
    return prompt;
}
