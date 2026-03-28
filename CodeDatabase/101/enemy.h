#ifndef ENEMY_H
#define ENEMY_H
#include "point.h"
#include <vector>
// #include "serializable.h"
#include <ostream>
#include <istream>

class Enemy {
public:
    Enemy(int x, int y);
    ~Enemy();

    int get_health() const;
    int get_coord_x() const;
    int get_coord_y() const;
    bool get_alive() const;
    int get_count_for_slow();
    int get_max_health();
    int get_damage();

    void subtract_health(int value);
    void set_alive(bool v);
    void set_damage(int v);
    void set_position(int dx, int dy);
    void set_count_for_slow(int val);
    void recovery_health();
    void change_health(int n);
    std::vector<int> random_journey();
    std::vector<int> enemy_atack();
    void spawn_enemy(int x, int y);
    void serialize(std::ostream& os) const ;
    void deserialize(std::istream& is) ;

private:
    int health;
    int damage;
    Point* enemy_position;
    int f;
    bool aliveEnemy;
    int count_for_slow;
};

#endif // ENEMY_H
