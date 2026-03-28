#ifndef ENEMY_H_
#define ENEMY_H_

class Field; 
class Player;  

class Enemy {
public:
    Enemy(int health = 50, int damage = 10, int x = 0, int y = 0);
    virtual ~Enemy() = default;
    int GetHealth() const;
    int GetDamage() const;
    void TakeDamage(int damage);
    bool IsAlive() const;
    int GetX() const;
    int GetY() const;
    void SetPosition(int x, int y);
    virtual void Update(Field& field, Player& player) {}
    void SetHealth(int h) { health_ = h; }
    void SetDamage(int d) { damage_ = d; }

private:
    int health_;
    int damage_;
    int x_;
    int y_;
};

#endif // ENEMY_H_
