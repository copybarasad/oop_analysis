#pragma once
#include <string>  
#include <utility> 
#include "IspellCard.h"

class DirectSpell : public ISpellCard
{
private:
    int level_;
    std::string name_;
    std::string desctription_;
    int damage_;
    int radius_;
public:
    DirectSpell(std::string name, std::string description, int damage, int radius);

    // Active methods
    std::pair<int, int> use() override;
    std::string name() const override;
    void levelUp() override;
    int getRadius() override;
    void use(ISpellCard &spell) override;
};