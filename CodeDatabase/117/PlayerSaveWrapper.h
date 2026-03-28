#ifndef PLAYERWRAPPER_H
#define PLAYERWRAPPER_H

#include "CharacterSaveWrapper.h"
#include "../Characters/Player.h"

class PlayerSaveWrapper : public ISaveLoadWrapper {
private:
    Player& player;
    CharacterSaveWrapper characterSaveWrapper;
    
public:
    explicit PlayerSaveWrapper(Player& playerRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif