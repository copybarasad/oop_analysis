#ifndef ALLY_H_
#define ALLY_H_

#include "field.h"
#include "player.h"

class Ally {
public:
    Ally(int x, int y, int health = 50, int damage = 10);
    ~Ally() = default;
    void Update(Field* field, Player* player);

    int GetX() const;
    int GetY() const;
    int GetHealth() const;
    int GetDamage() const;
    void TakeDamage(int damage);
    bool IsAlive() const;
    void SetPosition(int x, int y);
    void SetHealth(int h) { health_ = h; }
    void SetDamage(int d) { damage_ = d; }


private:
    int health_;
    int damage_;
    int x_;
    int y_;
    CellType under_type_;
};

#endif // ALLY_H_

