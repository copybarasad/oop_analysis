#pragma once
#include "Character.h"
#include "Player.h"
#include "Coord.h"

class Turret : public Character {
public:
    Turret(Coord pos, int hp, int damage, int range, int cooldown_turns);

    int Range() const;
    int Cooldown() const;
    int CooldownTimer() const;
    void SetCooldownTimer(int v);

    bool TryFireAtPlayer(const Coord& player_pos, Player& player);
    void CooldownTick();

private:
    int range_;
    int cooldown_max_;
    int cooldown_left_;

    int ManhattanDistance(const Coord& a, const Coord& b) const;
};
