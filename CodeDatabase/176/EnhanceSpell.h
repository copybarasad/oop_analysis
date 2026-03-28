#ifndef ENHANCE_SPELL_H
#define ENHANCE_SPELL_H

#include "Spell.h"

class EnhanceSpell : public Spell {
private:
    int manaCost;
    std::string name;
    std::string description;

public:
    EnhanceSpell(int cost);
    
    bool cast(GameManager& gameManager, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    bool requiresTarget() const override;
};

#endif
