#pragma once
#include <string> 
#include "spells/IspellCard.h"

class ImproveSpell : public ISpellCard{
    public:
        ImproveSpell();
        void use(ISpellCard& spell) override;
        std::string name() const override;
        void levelUp() override;
        std::pair<int, int> use() override;
        int getRadius() override;
};