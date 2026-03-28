#pragma once
#include "Character.hpp"
#include "Grid.hpp"
#include "Sleeve.hpp"
#include "Actions.hpp"

class GameWorld;
class GameContext;

class Player : public Character {
private:
    Sleeve sleeve_;
public:
    Player(int hp, int damage, Position start_pos, Logger& logger);

    bool move(Position pos, Grid& grid);
    void attackEnemy(Character& enemy);
    bool act(action a, Grid& grid, Character& enemy, GameWorld& world, GameContext& ctx);
    bool act(action a, Grid& grid, Character& enemy, GameWorld& world, GameContext& ctx, int param1);
    bool act(action a, Grid& grid, Character& enemy, GameWorld& world, GameContext& ctx, int param1, int param2);

    void save(std::ostream& os) const override;
    void load(std::istream& is);

    void setSleeve(Sleeve&& s) { sleeve_ = std::move(s); }
    Sleeve&& moveSleeve() { return std::move(sleeve_); }
};