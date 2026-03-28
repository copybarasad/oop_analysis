#ifndef ENTITY_SER_H
#define ENTITY_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../../Entities/Entity/Entity.hpp"
#include "../PlayerSerializer/PlayerSerializer.hpp"
#include "../../../../../Entities/Ally/Ally.hpp"
#include "../../../../../Entities/Enemies/Enemy.hpp"
#include "../../../../../Entities/Inanimate/Building/Building.hpp"
#include "../../../../../Entities/Inanimate/Tower/Tower.hpp"
#include "../../../../../Entities/Inanimate/Trap/Trap.hpp"

class EntitySerializer : public Serializable {
    private:
    const Entity& entity;
    
    public:
    EntitySerializer (const Entity& entity);

    std::string serialize () const override;
};

#endif