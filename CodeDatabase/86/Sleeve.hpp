#pragma once

#include "Position.hpp"
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <cmath>
#include <iostream>


class GameWorld;
class GameContext;
class Player;

inline int CalcDist(const Position& a, const Position& b) {
    int dx = std::abs(a.x - b.x);
    int dy = std::abs(a.y - b.y);
    return dx > dy ? dx : dy;
}

class IspellCard {
public:
    virtual ~IspellCard() = default;
    virtual const char* name() const = 0;
    virtual bool cast(GameWorld& world, GameContext& ctx, const Position& targetPos) = 0;

    virtual void save(std::ostream& os) const = 0;
    virtual void load(std::istream& is) = 0;
    virtual int getTypeID() const = 0;
};

class Sleeve {
    int maxSize;
    std::vector<std::unique_ptr<IspellCard>> cards;

public:
    Sleeve(int n);
    
    Sleeve(Sleeve&&) = default;
    Sleeve& operator=(Sleeve&&) = default;

    void addRandomSpell();
    void buyNewSpell(int stars, Player& player);
    void showCards() const;
    int getSize() const;
    bool useSpell(int index, GameWorld& world, GameContext& ctx, const Position& targetPos);

    void save(std::ostream& os) const;
    void load(std::istream& is);
};

class IdirectDamage : public IspellCard {
    int damage_;
    int range_;
public:
    IdirectDamage(int damage, int range) : damage_(damage), range_(range) {}
    const char* name() const override { return "DirectDamage"; }
    bool cast(GameWorld& world, GameContext& ctx, const Position& targetPos) override;
    void save(std::ostream& os) const override;
    void load(std::istream& is) override;
    int getTypeID() const override { return 1; }
};

class IsquareDamage : public IspellCard {
    int damage_;
    int range_;
public:
    IsquareDamage(int damage, int range) : damage_(damage), range_(range) {}
    const char* name() const override { return "SquareDamage"; }
    bool cast(GameWorld& world, GameContext& ctx, const Position& targetPos) override;
    void save(std::ostream& os) const override;
    void load(std::istream& is) override;
    int getTypeID() const override { return 2; }
};