#pragma once
#include "iostream"
#include "Building.h"

class GameField;

class SpawnerBuilding: public Building {
private:
    std::function<void(GameField*)> spawn;
    std::string name;
    int turns_to_spawn;
    int lasted_turns;
public:
    SpawnerBuilding(std::string name, std::function<void(GameField*)> spawn, int turns_to_spawn):
        name(name),
        spawn(spawn),
        turns_to_spawn(turns_to_spawn),
        lasted_turns(0)
    {}
    ~SpawnerBuilding() = default;

    void makeTurn(GameField* f) {
        lasted_turns++;
        std::cout << "lasted_turns: " << lasted_turns << std::endl;
        if(lasted_turns == turns_to_spawn) {
            std::cout << "spawn" << std::endl;
            spawn(f);
            lasted_turns = 0;
        }
    };

    int getTurnsToSpawn() const { return turns_to_spawn; }
    int getLastedTurns() const { return lasted_turns; }

    void setLastedTurns(int turns) { lasted_turns = turns; }

    const std::string getName() const { return name; };
};