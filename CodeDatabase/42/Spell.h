#pragma once
#include <string>

class Spell {
public:
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRange() const = 0;
    virtual ~Spell() = default;
};
