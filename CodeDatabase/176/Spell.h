#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>

class GameManager;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool cast(GameManager& gameManager, int targetX, int targetY) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual bool requiresTarget() const = 0;
};

#endif
