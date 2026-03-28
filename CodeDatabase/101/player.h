#ifndef PLAYER_H
#define PLAYER_H
#include "constants.h"
#include "point.h"
#include "handPlayer.h"
#include <istream>
#include <ostream>

enum Directions{
    up,
    down,
    right,
    left
};

enum TypeBattle {
    close_combat,
    ranged_combat
};

class Player
{
public:
    Player(int x, int y, int size);
    ~Player();

    Point* get_position() const;
    int get_x() const;
    int get_y() const;
    int get_health() const;
    TypeBattle get_typeBattle() const;
    Directions get_direction() const;
    int get_damage() const;
    int get_zoneX() const;
    int get_zoneY() const;
    int get_money() const;
    int get_count_for_slow() const;
    handPlayer* get_hand();

    Point& set_position();
    void set_damage(int value);
    void set_zoneX(int v);
    void set_zoneY(int v);
    void set_count_for_slow(int val);
    void change_typeBattle();
    void change_direction_attack(Directions d);
    void change_health(int n);
    void add_money(int val);
    void spend_money(int val);
    void recovery_health();
    void spawn_player(int x, int y);
    void player_journey(int dx, int dy);
    std::vector<int> close_attack();
    std::vector<int> ranged_atack();
    void serialize(std::ostream& os) const ;
    void deserialize(std::istream& is) ;

private:
    int health;
    int damage;
    int score;
    TypeBattle type;
    Point* player_position;
    Directions dir;
    int count_for_slow;
    int zoneX;
    int zoneY;
    int money;
    handPlayer* myHand;


};

#endif // PLAYER_H
