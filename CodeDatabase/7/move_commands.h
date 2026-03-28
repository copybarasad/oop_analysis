#pragma once
#include "command.h"
#include "Game.h"
#include <string>

class MoveUpCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        // Вызываем метод игры, а не игрока!
        resultMessage = game.movePlayer(0, -1);
        return true;
    }
    std::string getType() const override { return "MOVE_UP"; }
};

class MoveDownCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        resultMessage = game.movePlayer(0, 1);
        return true;
    }
    std::string getType() const override { return "MOVE_DOWN"; }
};

class MoveLeftCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        resultMessage = game.movePlayer(-1, 0);
        return true;
    }
    std::string getType() const override { return "MOVE_LEFT"; }
};

class MoveRightCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        resultMessage = game.movePlayer(1, 0);
        return true;
    }
    std::string getType() const override { return "MOVE_RIGHT"; }
};