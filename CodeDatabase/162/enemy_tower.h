#ifndef ENEMY_TOWER_H_
#define ENEMY_TOWER_H_

class Field;
class Player;

class EnemyTower {
public:
    EnemyTower(int x, int y, int range = 3, int damage = 15);
    int GetX() const { return x_; }
    int GetY() const { return y_; }
    void Update(Field& field, Player& player);
    int GetHealth() const { return health_; }
    void TakeDamage(int dmg) { health_ -= dmg; if (health_ < 0) health_ = 0; }
    bool IsDestroyed() const { return health_ <= 0; }
    int GetDamage() const { return damage_; }
    int GetRange() const { return range_; }
    int GetCooldown() const { return cooldown_; }
    void SetHealth(int h) { health_ = h; }
    void SetCooldown(int c) { cooldown_ = c; }

private:
    int x_, y_, range_, damage_;
    int cooldown_ = 0;
    int health_ = 30; 
};

#endif // ENEMY_TOWER_H_
