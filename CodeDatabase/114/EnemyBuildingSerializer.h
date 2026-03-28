#ifndef ENEMYBUILDINGSERIALIZER_H
#define ENEMYBUILDINGSERIALIZER_H

#include "ISerializer.h"
#include "EntitySerializer.h"
#include "../entities/EnemyBuilding.h"

class EnemyBuildingSerializer : public ISerializer<EnemyBuilding> {
private:
    EntitySerializer entitySerializer;
    
public:
    nlohmann::json serialize(const EnemyBuilding& building) const override;
    void deserialize(EnemyBuilding& building, const nlohmann::json& json) const override;
};

#endif

