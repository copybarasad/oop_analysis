#ifndef GAME_ENEMY_H_
#define GAME_ENEMY_H_

class Enemy {
public:
    explicit Enemy(int health = 50, int damage = 15);

    int health() const { return health_; }
    int damage() const { return damage_; }

    void take_damage(int damage);

    void set_position(int row, int col);
    int row() const { return row_; }
    int col() const { return col_; }

    bool is_alive() const { return health_ > 0; }

private:
    int health_;
    int damage_;
    int row_ = -1;
    int col_ = -1;
};

#endif
