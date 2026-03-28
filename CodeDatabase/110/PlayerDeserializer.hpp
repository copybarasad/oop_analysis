#ifndef PLAYER_DESER_H
#define PLAYER_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "../ItemDeserializer/ItemDeserializer.hpp"

class PlayerDeserializer : public BaseDeserializer {
    public:
    Player* deserialize(const std::string& data, size_t id, unsigned int maxHealth, unsigned int currentHealth, unsigned int baseDamage);
};

#endif