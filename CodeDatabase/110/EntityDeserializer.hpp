#ifndef ENTITY_DESER_H
#define ENTITY_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "../ItemDeserializer/ItemDeserializer.hpp"
#include "../PlayerDeserializer/PlayerDeserializer.hpp"
#include "modules/Entities/Inanimate/Building/Building.hpp"
#include "modules/Entities/Inanimate/Tower/Tower.hpp"
#include "modules/Entities/Ally/Ally.hpp"
#include "modules/Entities/Inanimate/Trap/Trap.hpp"

class EntityDeserializer : public BaseDeserializer {
    private:
    ItemDeserializer itemDeserializer;

    public:
    Entity* deserialize(const std::string& data);

    private:
    Building* deserializeBuilding(const std::string& data, size_t id, unsigned int maxHealth, unsigned int currentHealth, unsigned int damage);
    Tower* deserializeTower(const std::string& data, size_t id, unsigned int maxHealth, unsigned int currentHealth, unsigned int damage);
};

#endif