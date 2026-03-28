#ifndef GAME_SLOWTRAP_H
#define GAME_SLOWTRAP_H
#include "../basics/Object.h"

class SlowTrap : public Object{
private:
    bool activated_ = false;
public:
    SlowTrap() : Object("SlowTrap") {
    }

    void update() override;
    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif