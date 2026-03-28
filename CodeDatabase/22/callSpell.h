#pragma once
#include <string>  
#include <utility> 
#include "IspellCard.h"

class CallSpell : public ISpellCard{
    private:
        int level_;
        int radius_;

    public: 
        CallSpell();

        std::pair <int, int> use() override;
        void use(ISpellCard &spell) override;

        std::string name() const override;
        void levelUp() override;
        int getRadius() override;
};