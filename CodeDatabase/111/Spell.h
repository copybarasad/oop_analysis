#pragma once
#include <string>

class Location;
class GameBoard;

class Spell {
public:
    virtual ~Spell() = default;
    
    virtual bool canCast(const Location& casterPos, const Location& targetPos, const GameBoard& board) const = 0;
    virtual void cast(const Location& casterPos, const Location& targetPos, GameBoard& board) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
};