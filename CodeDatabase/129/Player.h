#pragma once
#include "../../saving/Saving.h"
#include "../utils/Values.h"
#include "Entity.h"

class Player : public Entity{
private:
    bool close_combat;
public:
    Player();
    ~Player();

    void set_combat(bool c_c);
    bool is_close_combat();

    Saving::PlayerData serialize() const;
    void deserialize(const Saving::PlayerData& data);
};
