#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "imovable.h"
#include "handsplayer.h"
#include "entity.h"

class Player : public Entity, public IMovable
{
public:
    enum TypeBattle
    {
        MELEE,
        RANGED
    };

    Player(int health, int mana, int xPos, int yPos, int rangedDamage, int meleeDamage);
    virtual ~Player() override = default;

    void changeTypeBattle();
    HandsPlayer& getHands() { return *m_hands; }

    void move(const Cell& cell, int dx, int dy) override;
    int attack(Entity& other) override;
    void takeDamage(int damage) override;

private:
    int m_rangedDamage;
    TypeBattle m_typeBattle;
    std::unique_ptr<HandsPlayer> m_hands;
};

#endif // PLAYER_H
