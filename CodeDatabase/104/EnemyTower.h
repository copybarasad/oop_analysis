#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class Player;

class EnemyTower {
private:
    int position_x_;
    int position_y_;
    int attack_range_;
    int damage_;
    bool can_attack_;

public:
    EnemyTower(int x, int y, int attack_range, int damage);

    void update();
    void tryAttackPlayer(Player& player);

    int getPositionX() const { return position_x_; }
    int getPositionY() const { return position_y_; }
    int getAttackRange() const { return attack_range_; }
    int getDamage() const { return damage_; }
    bool canAttack() const { return can_attack_; }
    void setPosition(int x, int y) {
        position_x_ = x;
        position_y_ = y;
    }
    void setAttackRange(int range) { attack_range_ = range; }
    void setDamage(int damage) { damage_ = damage; }
    void setCanAttack(bool can_attack) { can_attack_ = can_attack; }
};

#endif