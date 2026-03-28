#pragma once

class Game;

// Базовый интерфейс для всех команд
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Game& game) = 0;
};
