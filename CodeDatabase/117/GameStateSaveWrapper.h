#ifndef GAMESTATEWRAPPER_H
#define GAMESTATEWRAPPER_H

#include "FieldSaveWrapper.h"
#include "HandSaveWrapper.h"
#include "PlayerSaveWrapper.h"
#include "EnemySaveWrapper.h"
#include "AllySaveWrapper.h"
#include "EnemyTowerSaveWrapper.h"
#include "EnemyBuildingSaveWrapper.h"
#include "TrapSaveWrapper.h"
#include "../Controllers/GameState.h"

class GameStateSaveWrapper : public ISaveLoadWrapper {
private:
    GameState& gameState;
    FieldSaveWrapper fieldSaveWrapper;
    HandSaveWrapper handSaveWrapper;
    PlayerSaveWrapper playerSaveWrapper;
    
public:
    explicit GameStateSaveWrapper(GameState& gameStateRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif