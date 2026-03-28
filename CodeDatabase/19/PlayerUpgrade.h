#pragma once

#include <string>

class GameLogic;

class PlayerUpgrade {
public:
    virtual ~PlayerUpgrade();

    virtual std::string text(int level) = 0;
    virtual void apply(const GameLogic& logic, int level) = 0;
    virtual int price(int level) = 0;
    virtual std::string id() = 0;
};

