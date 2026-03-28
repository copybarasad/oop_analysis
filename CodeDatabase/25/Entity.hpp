#pragma once

#include "Position.hpp"

// Базовый персонаж: обладает запасом здоровья, атакой и позицией на карте 
class Entity {
protected:
    int hp_{1};
    int atk_{1};
    Pos pos_{};

public:
    Entity() = default;
    Entity(int hp, int atk, Pos p) : hp_(hp), atk_(atk), pos_(p) {}
    virtual ~Entity() = default;

    bool alive() const { return hp_ > 0; }
    int  hp()    const { return hp_; }
    int  atk()   const { return atk_; }
    Pos  pos()   const { return pos_; }

    void set_pos(Pos p) { pos_ = p; }

    // Уменьшите HP на d (не ниже нуля).
    void take_damage(int d) {
        if (d > 0) {
            hp_ -= d;
            if (hp_ < 0) hp_ = 0;
        }
    }
};
