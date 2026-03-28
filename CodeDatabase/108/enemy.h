#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include "entity.h"

class GameState;
class GameField;

class Enemy : public Entity {
    int attack_range;

public:
    Enemy(std::string name, const Position& position, int health, int damage, int range = 1);

    void move_towards_player(const Position& player_position, GameState& game_state);
    void find_alternative_path(const Position& player_position, GameState& game_state);

    bool can_attack_player(const Position& player_position) const;
    int get_attack_range() const;
};

#endif //OOP_ENEMY_H