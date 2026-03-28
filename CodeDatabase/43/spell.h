#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>

class Game; // Forward declaration

class Spell {
public:
    virtual ~Spell() = default;
    
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual bool canCast(int playerX, int playerY, int targetX, int targetY) const = 0;
    virtual void cast(Game& game, int targetX, int targetY) = 0;
    virtual int getManaCost() const = 0;
};

#endif