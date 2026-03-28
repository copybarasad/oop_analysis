#ifndef CHARACTERWRAPPER_H
#define CHARACTERWRAPPER_H

#include "ISaveLoadWrapper.h"
#include "../Characters/Character.h"

class CharacterSaveWrapper : public ISaveLoadWrapper {
protected:
    Character& character;
    
public:
    explicit CharacterSaveWrapper(Character& characterRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif