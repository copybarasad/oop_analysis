#ifndef HANDWRAPPER_H
#define HANDWRAPPER_H

#include "ISaveLoadWrapper.h"
#include "../Spells/Hand.h"

class HandSaveWrapper : public ISaveLoadWrapper {
private:
    Hand& hand;
    
public:
    explicit HandSaveWrapper(Hand& handRef);
    void save(nlohmann::json& output) override;
    void load(const nlohmann::json& input) override;
    size_t getHash() const override;
};

#endif