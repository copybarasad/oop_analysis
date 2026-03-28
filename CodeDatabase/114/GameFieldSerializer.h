#ifndef GAMEFIELDSERIALIZER_H
#define GAMEFIELDSERIALIZER_H

#include "ISerializer.h"
#include "CellSerializer.h"
#include "EnemySerializer.h"
#include "EnemyBuildingSerializer.h"
#include "PetSerializer.h"
#include "PlayerSerializer.h"
#include "../GameField.h"

class GameFieldSerializer : public ISerializer<GameField> {
private:
    CellSerializer cellSerializer;
    EnemySerializer enemySerializer;
    EnemyBuildingSerializer buildingSerializer;
    PetSerializer petSerializer;
    PlayerSerializer playerSerializer;
    
public:
    nlohmann::json serialize(const GameField& field) const override;
    void deserialize(GameField& field, const nlohmann::json& json) const override;
};

#endif

