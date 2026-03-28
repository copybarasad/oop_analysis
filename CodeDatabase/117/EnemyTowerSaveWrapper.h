#ifndef ENEMYTOWERWRAPPER_H
#define ENEMYTOWERWRAPPER_H

#include "CharacterSaveWrapper.h"
#include "../Characters/EnemyTower.h"

class EnemyTowerSaveWrapper : public ISaveLoadWrapper {
private:
    EnemyTower& tower;
    CharacterSaveWrapper characterSaveWrapper;
    
public:
    explicit EnemyTowerSaveWrapper(EnemyTower& towerRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif