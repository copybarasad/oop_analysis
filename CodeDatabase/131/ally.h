#ifndef OOP_ALLY_H_
#define OOP_ALLY_H_

#include "position.h"

namespace rpg {
    class Ally final {
    public:
        Ally(int health, int damage, const Position &position);

        [[nodiscard]] Position GetPosition() const;

        [[nodiscard]] bool IsAlive() const;

        [[nodiscard]] int GetDamage() const;

        [[nodiscard]] int GetHealth() const;

        void TakeDamage(int damage);

        void SetPosition(const Position &pos);

    private:
        int health_;
        int damage_;
        Position position_;
    };
}

#endif
