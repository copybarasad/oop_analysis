#pragma once
#include <string> 
#include <utility>

class ISpellCard
{
public:
    virtual ~ISpellCard() = default;
    virtual std::pair<int, int> use() = 0;
    virtual void use(ISpellCard &spell) = 0;
    virtual std::string name() const = 0;
    virtual void levelUp() = 0;
    virtual int getRadius() = 0;
};


