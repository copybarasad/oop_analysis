#pragma once
#include <string>  
#include <utility> 
#include "IspellCard.h"

class AreaSpell : public ISpellCard{

    private:
        int level_;
        int radius_;
        int damage_;
        std::string name_;
        std::string description_;

    public:
        AreaSpell(std::string name, std::string description, int damage, int radius);

        //Active methods
        std::pair<int, int> use() override;
        void use(ISpellCard &spell) override;

        void levelUp() override;
        std::string name() const override;

        int getRadius() override;
};  