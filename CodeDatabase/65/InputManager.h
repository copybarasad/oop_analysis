#pragma once

#include <unistd.h>

enum class Command {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    CHANGE_ATTACK_MODE,
    QUIT,
    CONFIRM,
    CANCEL,
    CAST_SPELL,
    SAVE_GAME,
    LOAD_GAME,
};

enum class Direction {
    NONE,
    UP,
    DOWN,
    RIGHT,
    LEFT
};

enum class Numbers {
    ZERO=0,
    ONE=1,
    TWO=2,
    THREE=3,
    FOUR=4,
};

class InputManager {
public:
    char getChar() {
        char ch = ' ';
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }

    Command getCommand() {
        char cmd = ' ';
        read(STDIN_FILENO, &cmd, 1);

        switch (cmd) {
            case 'w': return Command::MOVE_UP;
            case 'a': return Command::MOVE_LEFT;
            case 's': return Command::MOVE_DOWN;
            case 'd': return Command::MOVE_RIGHT;
            case 'q': return Command::QUIT;
            case 'e': return Command::CHANGE_ATTACK_MODE;
            case ' ': return Command::ATTACK;
            case '\n': return Command::CONFIRM; // Enter
            case 27: return Command::CANCEL; // ESC
            case 'c': return Command::CAST_SPELL;
            case 'f': return Command::SAVE_GAME;
            case 'l': return Command::LOAD_GAME;
            default: return Command::NONE;
        }
    }

    Direction getDirection() {
        char direction = ' ';
        read(STDIN_FILENO, &direction, 1);

        switch (direction) {
            case 'w': return Direction::UP;
            case 'a': return Direction::LEFT;
            case 's': return Direction::DOWN;
            case 'd': return Direction::RIGHT;
            default: return Direction::NONE;
        }
    }
};
