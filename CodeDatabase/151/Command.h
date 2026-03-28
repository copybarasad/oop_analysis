#ifndef COMMAND_H
#define COMMAND_H

#include "Position.h"
#include <memory>
#include <string>
#include <map>

class GameField;
class Player;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(GameField* field, Player* player) = 0;
};

class MoveCommand : public Command {
    int dx, dy;
public:
    MoveCommand(int dx, int dy);
    void execute(GameField* field, Player* player) override;
};

class CastSpellCommand : public Command {
    int index;
    Position target;
public:
    CastSpellCommand(int index, const Position& target);
    void execute(GameField* field, Player* player) override;
};

class SaveCommand : public Command {
public:
    SaveCommand() = default;
    void execute(GameField* field, Player* player) override;
};

class LoadCommand : public Command {
public:
    LoadCommand() = default;
    void execute(GameField* field, Player* player) override;
};

class BuySpellCommand : public Command {
public:
    BuySpellCommand() = default;
    void execute(GameField* field, Player* player) override;
};

class HelpCommand : public Command {
private:
    const std::map<char, std::string>& keyMap;

public:
    HelpCommand(const std::map<char, std::string>& map) : keyMap(map) {}
    void execute(GameField* field, Player* player) override;
};

class QuitCommand : public Command {
public:
    QuitCommand() = default;
    void execute(GameField* field, Player* player) override;
};

#endif