#ifndef PLAYERSERIALIZER_H
#define PLAYERSERIALIZER_H

#include "ISerializer.h"
#include "EntitySerializer.h"
#include "HandSerializer.h"
#include "../entities/Player.h"

class PlayerSerializer : public ISerializer<Player> {
private:
    EntitySerializer entitySerializer;
    HandSerializer handSerializer;
    
public:
    nlohmann::json serialize(const Player& player) const override;
    void deserialize(Player& player, const nlohmann::json& json) const override;
};

#endif

