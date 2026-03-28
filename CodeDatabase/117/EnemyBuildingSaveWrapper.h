#ifndef ENEMYBUILDINGWRAPPER_H
#define ENEMYBUILDINGWRAPPER_H

#include "CharacterSaveWrapper.h"
#include "../Characters/EnemyBuilding.h"

class EnemyBuildingSaveWrapper : public ISaveLoadWrapper {
private:
    EnemyBuilding& build;
    CharacterSaveWrapper characterSaveWrapper;
    
public:
    explicit EnemyBuildingSaveWrapper(EnemyBuilding& buildRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif