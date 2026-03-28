#ifndef ENEMYWRAPPER_H
#define ENEMYWRAPPER_H

#include "CharacterSaveWrapper.h"
#include "../Characters/Enemy.h"

class EnemySaveWrapper : public ISaveLoadWrapper {
private:
    Enemy& enemy;
    CharacterSaveWrapper characterSaveWrapper;
    
public:
    explicit EnemySaveWrapper(Enemy& enemyRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif