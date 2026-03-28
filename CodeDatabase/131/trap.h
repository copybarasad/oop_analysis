#ifndef OOP_TRAP_H_
#define OOP_TRAP_H_

#include "position.h"

namespace rpg {
    class Trap final {
    public:
        Trap(const Position& position, int damage);
        
        [[nodiscard]] Position GetPosition() const;
        [[nodiscard]] int GetDamage() const;
        
    private:
        Position position_;
        int damage_;
    };
}

#endif


