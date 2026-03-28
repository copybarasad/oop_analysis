#ifndef ALLY_H
#define ALLY_H

class Ally {
public:
    Ally(int x, int y, int health, int damage);

    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDamage() const;
    int GetX() const;
    int GetY() const;

    void TakeDamage(int damage);
    void SetPosition(int x, int y);
    bool IsAlive() const;

private:
    int health_;
    int max_health_;
    int damage_;
    int x_;
    int y_;
};

#endif // ALLY_H
