#ifndef ALLYWRAPPER_H
#define ALLYWRAPPER_H

#include "CharacterSaveWrapper.h"
#include "../Characters/Ally.h"

class AllySaveWrapper : public ISaveLoadWrapper {
private:
    Ally& ally;
    CharacterSaveWrapper characterSaveWrapper;
    
public:
    explicit AllySaveWrapper(Ally& allyRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif