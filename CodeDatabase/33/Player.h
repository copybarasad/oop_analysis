#pragma once
#include <string>
#include "Character.h"

class Player : public Character {
public:
    enum class Mode { Melee, Ranged };

    Player();
    Player(std::string name, int hp, int melee, int ranged, Coord pos);

    const std::string& Name() const;
    void SetName(const std::string& n);

    Mode CurrentMode() const;
    void ToggleMode();

    int MeleeDamage() const;
    int RangedDamage() const;
    void SetMelee(int v);
    void SetRanged(int v);
    int CurrentDamage() const;

    int Score() const;
    void SetScore(int s);
    void AddScore(int s);

    int  BaseHP() const;
    void SetBaseHP(int v);
    void HealFull();

private:
    std::string name_;
    Mode mode_;
    int melee_;
    int ranged_;
    int score_;
    int base_hp_;
};
