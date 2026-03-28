#pragma once

#include "pos.h"

namespace game {
    /// @brief Base entity class 
    class Character {
        protected:
            int health_{0};
            int damage_{0};
            int deadTurnCounter_{0};

            Pos position_{};

        public:
            // === Constructors ===

            Character(int hp, int dmg, Pos pos) 
                : health_(hp), damage_(dmg), position_(pos) {}
            
            virtual ~Character() = default;

            Character(const Character&) = default;
            Character(Character&&) = default;
            Character& operator=(const Character&) = default;
            Character& operator=(Character&&) = default;

            // === Getters ===

            int getHP()                 const { return health_; }
            int getDamage()             const { return damage_; }
            Pos getPosition()           const { return position_; }

            int getDeadTurnCounter()    const { return deadTurnCounter_; }
            bool isAlive()              const { return health_ >  0; }
            bool isCorpse()             const { return health_ <= 0; }
            
            // === Setters ===

            void incrementDeadCounter() noexcept { deadTurnCounter_++; }
            void setPosition(Pos pos) { position_ = pos; }
            void setHP(int hp) { health_ = hp; }
            void setDamage(int dmg) { damage_ = dmg; }
            void setDeadTurnCounter(int counter) { deadTurnCounter_ = counter; }

            // === Damage ===

            virtual void takeDamage(int dmg);
    };
}