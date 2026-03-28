#pragma once
#include "Position.h"
#include <string>


class Game;

class Entity {
public:
    Entity(std::string id, Position pos, int hp);
    virtual ~Entity() = default;

    Position GetPosition() const;
    void SetPosition(Position p);

    std::string GetId() const;
    int GetHp() const;
    bool IsAlive() const;

    virtual void TakeDamage(int amount);
    virtual void OnTurn(Game& game);

protected:
    std::string id_;
    Position pos_;
    int hp_;
};