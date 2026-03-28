#pragma once
#include "IspellCard.h"
#include "field/position.h"

struct Position;

class TrapSpell : public ISpellCard{
    private:
        Position position_;
        int damage_;

    public:
        TrapSpell(int damage, Position position);

        std::pair<int,int> use() override;
        std::string name() const override;
        void levelUp() override;
        Position position();
        void use(ISpellCard &spell) override;
        int getRadius() override;
};