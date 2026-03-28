#ifndef ENTITYSERIALIZER_H
#define ENTITYSERIALIZER_H

#include "ISerializer.h"
#include "../entities/Entity.h"

class EntitySerializer : public ISerializer<Entity> {
public:
    nlohmann::json serialize(const Entity& entity) const override;
    void deserialize(Entity& entity, const nlohmann::json& json) const override;
};

#endif

