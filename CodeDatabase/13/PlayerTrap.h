#ifndef GAME_PLAYERTRAP_H
#define GAME_PLAYERTRAP_H
#include "../basics/Object.h"

class PlayerTrap : public Object {
private:
    int damage_;

public:
    explicit PlayerTrap(const int damage) : Object("PlayerTrap"), damage_(damage) {
    }

    void update() override;

    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif
