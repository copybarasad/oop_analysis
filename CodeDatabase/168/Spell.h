#pragma once

#include "Position.h"
#include <string>
#include <memory>

class GameController;

struct SpellResult {
    bool success;
    bool consumesTurn;
    std::string message;

    SpellResult(bool success = false, bool consumesTurn = false, const std::string& message = "")
        : success(success), consumesTurn(consumesTurn), message(message) {}
};

struct EnhancementContext {
    int bonusRange; 
    int bonusArea;
    int bonusDamage;
    int bonusSummons;
    int stackCount;

    EnhancementContext() : bonusRange(0), bonusArea(0), bonusDamage(0), bonusSummons(0), stackCount(0) {}

    void addStack() {
        stackCount++;
        bonusRange += 1;
        bonusArea += 1;
        bonusDamage += 10;
        bonusSummons += 1;
    }

    void reset() {
        bonusRange = 0;
        bonusArea = 0;
        bonusDamage = 0;
        bonusSummons = 0;
        stackCount = 0;
    }

    bool hasEnhancement() const { return stackCount > 0; }
};

class Spell {
public:
    virtual ~Spell() = default;

    virtual SpellResult cast(GameController& game, const Position& target, EnhancementContext& enhancement) = 0;

    virtual std::string getName() const = 0;

    virtual std::string getDescription() const = 0;

    virtual char getSymbol() const = 0;

    virtual int getRange() const = 0;

    virtual bool isEnhancement() const { return false; }

    virtual std::unique_ptr<Spell> clone() const = 0;
};
