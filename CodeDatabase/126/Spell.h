#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>
#include <vector>
#include "../field/Position.h"
#include "../field/GameField.h"

class GameObject;

class Spell {
public:
    virtual ~Spell() = default;
    
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getRange() const = 0;
    
    virtual bool canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const = 0;
    virtual void cast(const Position& casterPos, const Position& targetPos, GameField& field, 
                      std::vector<std::shared_ptr<GameObject>>& objects) = 0;
    
    virtual std::string serialize() const = 0;
};

#endif