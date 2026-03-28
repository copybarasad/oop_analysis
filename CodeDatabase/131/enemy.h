#ifndef OOP_ENEMY_H_
#define OOP_ENEMY_H_

#include "position.h"

namespace rpg {
    class Enemy final {
    public:
        Enemy(int health, int damage, const Position &position);

        [[nodiscard]] int GetHealth() const;

        [[nodiscard]] Position GetPosition() const;

        [[nodiscard]] bool IsAlive() const;

        void TakeDamage(int damage);

        void SetPosition(const Position &pos);

        [[nodiscard]] int GetDamage() const;

    private:
        int health_;
        int damage_;
        Position position_;

        // Инвариант: health_ >= 0
        // Инвариант: damage_ >= 0
    };
}

#endif
