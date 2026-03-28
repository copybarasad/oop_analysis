#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>
#include <vector>

class GameField;
class Enemy;
class Player;
class SpellTarget;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool cast(const SpellTarget& target, GameField& field,
        std::vector<Enemy>& enemies, Player& player) = 0;
    virtual std::string getDescription() const = 0;
    virtual std::unique_ptr<Spell> clone() const = 0;
    virtual std::string getName() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
};

#endif
