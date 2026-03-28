#pragma once
#include "Trap.h"


class Fishing_net : public Trap{
private:
    GameConfig config;
    int damage;
    int cost;
    int range;
    Position pos;
    SpellFlag flag;

public:
    Fishing_net(GameConfig &cfg);
    Position &getPosition() override;
    void setPosition(Position &pos) override;
    int getDamage() override;
    void setDamage(int amount);
    int getRange() override;
    void setRange(int amount);
    SpellFlag getFlag() override;
    std::string getFlagString() const override;
    int getCost() override;
    void setCost(int amount);
    bool setTrap(Player &pl, Position &new_pos, Sea &sea) override;
    
    void saveTo(SaveFileRAII &out) const override;
    void loadFrom(SaveFileRAII& in) override;
};