#pragma once
#include <memory>

class Player;
class Field;

// Interface: all methods are pure virtual, no data fields
class Spell {
public:
    virtual ~Spell() = default;
    
    // Pure virtual methods - must be implemented by derived classes
    virtual bool cast(Player* caster, int targetX, int targetY, Field& field) = 0;
    virtual const char* getName() const = 0;
    virtual const char* getDescription() const = 0;
    virtual std::unique_ptr<Spell> clone() const = 0;
    virtual int getRadius() const = 0;
    virtual int getDamage() const = 0;
    virtual bool isInRange(Player* caster, int targetX, int targetY) const = 0;
};

