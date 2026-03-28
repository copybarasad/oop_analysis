#pragma once
#include "Entity.h"
#include <string>

class Game;

class Ally : public Entity
{
public:
    Ally(std::string id, Position pos, int hp, int attack);

    void OnTurn(Game& game) override;
    int GetAttack() const;

private:
    int attack_;
};
