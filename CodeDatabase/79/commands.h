#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include "movement.h"

class Game;

class Basic_command{
public:
    virtual ~Basic_command() = default;
    virtual int execute(Game& game) = 0;
};

class Move_command : public Basic_command{
private:
    Movement::Direction direction;
public:
    Move_command(Movement::Direction dir);
    int execute(Game& game) override;
};

class Attack_command : public Basic_command{
public:
    int execute(Game& game) override;
};

class Use_spell_command : public Basic_command{
public:
    int execute(Game& game) override;
};

class Buy_spell_command : public Basic_command{
public:
    int execute(Game& game) override;
};

class Change_weapon_command : public Basic_command{
public:
    int execute(Game& game) override;
};

class Save_game_command : public Basic_command{
public:
    int execute(Game& game) override;
};

class Exit_game_command : public Basic_command{
public:
    int execute(Game& game) override;
};

class Main_menu_commands : public Basic_command{
private:
    char choice;
public:
    Main_menu_commands(char _choice);
    int execute(Game& game) override;
};

class Upgrade_commands : public Basic_command{
private:
    char choice;
public:
    Upgrade_commands(char _choice);
    int execute(Game& game) override;
};

#endif