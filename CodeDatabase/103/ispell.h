#pragma once
#include <string>
#include <utility>

class GeneralManager;

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual void upgrade() = 0;
    virtual void cast(GeneralManager& gen_man, std::pair<int, int> target) = 0;
    virtual int getRadius() const = 0;
    virtual int getDamage() const = 0;
};
