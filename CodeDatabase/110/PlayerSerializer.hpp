#ifndef PLAYER_SER_H
#define PLAYER_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../ItemSerializer/ItemSerializer.hpp"
#include "modules/Entities/Player/Player.hpp"

class PlayerSerializer : public Serializable {
    private:
    const Player& player;
    
    public:
    PlayerSerializer (const Player& player);

    std::string serialize () const override;
};

#endif