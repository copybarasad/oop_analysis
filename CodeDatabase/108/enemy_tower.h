#ifndef OOP_ENEMY_TOWER_H
#define OOP_ENEMY_TOWER_H

#include "entity.h"
#include "position.h"

class GameState;
class Player;

class EnemyTower : public Entity {
    int attack_range;
    int cooldown;
    int max_cooldown;

public:

    EnemyTower(std::string name, const Position& position, int health, int range, int damage, int cooldown_turns);


    void attack_player(Player& player, GameState& game_state);           // cast spell
    void decrease_cooldown();
    int get_attack_range() const;

    bool can_attack_player(const Player& player) const;
    int get_attack_cooldown() const;
    int get_max_cooldown() const;

};



#endif //OOP_ENEMY_TOWER_H