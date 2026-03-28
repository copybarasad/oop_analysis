#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    Enemy(int health, int damage);

    int GetHealth() const;
    int GetDamage() const;
    void SetPosition(int x, int y);
    void GetPosition(int& x, int& y) const;
    void TakeDamage(int damage);
    bool IsAlive() const;
    void setHealth(int health) { health_ = health; }
    void setDamage(int damage) { damage_ = damage; }
    int getPositionX() const { return position_x_; }
    int getPositionY() const { return position_y_; }

private:
    int health_;
    int damage_;
    int position_x_;
    int position_y_;
};

#endif