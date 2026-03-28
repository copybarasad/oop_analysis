#ifndef TRAPWRAPPER_H
#define TRAPWRAPPER_H

#include "CharacterSaveWrapper.h"
#include "../Characters/Trap.h"

class TrapSaveWrapper : public ISaveLoadWrapper {
private:
    Trap& trap;
    CharacterSaveWrapper characterSaveWrapper;
    
public:
    explicit TrapSaveWrapper(Trap& trapRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif