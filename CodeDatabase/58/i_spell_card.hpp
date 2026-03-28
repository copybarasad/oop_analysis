#pragma once

#include <string>
#include <iostream>

class Cell;
class World;
class Entity;

class ISpellCard{
public:
    virtual ~ISpellCard() = default;

    virtual bool cast(World* world) = 0;
    virtual std::string get_name() = 0;
};