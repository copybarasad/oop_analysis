#pragma once
#include "GameConfig.h"
#include "Spell.h"
#include "Trap.h"
#include "Enums.h"
#include <vector>
#include <memory>
class Player;
class Sea;
class Game;


class Divers_bag{
private:
    GameConfig config;
    std::vector<std::unique_ptr<Spell>> spells;
    std::vector<std::unique_ptr<Trap>> traps;
    int max_size;
    int curr_bag_size;
    int spells_size;
    int traps_size;

public:
    explicit Divers_bag(GameConfig &cfg);
    Divers_bag(const Divers_bag&) = delete;
    Divers_bag& operator=(const Divers_bag&) = delete;
    void addRandomSpell();
    bool bagIsEmpty();
    int getMaxSize();
    void setMaxSize(int amount);
    int getSize();
    bool check_spell_index(int index);
    bool check_trap_index(int index);
    bool whatGet(int index);
    Spell* getSpell(int index);
    Trap* getTrap(int index);
    void useSpell(int index);
    int useTrap(Position &pos);
    void gift();
    void display_bag();
    void reset();

    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};