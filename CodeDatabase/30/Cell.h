#pragma once
#include <iostream>
#include "Enums.h"

class Cell {
public:
    Cell();
    explicit Cell(bool passable, bool slowing = false);

    Cell(const Cell&) = default;
    Cell& operator=(const Cell&) = default;

    bool isPassable() const;
    void setPassable(bool p);
    bool isSlowing() const { return slowing_; }
    void setSlowing(bool v) { slowing_ = v; }

    void save(std::ostream& os) const {
        os << passable_ << " " << slowing_ << " " << trapDamage_ << "\n";
    }
    void load(std::istream& is) {
        is >> passable_ >> slowing_ >> trapDamage_;
    }

    bool hasTrap() const { return trapDamage_ > 0; }
    void placeTrap(int dmg);          // положили ловушку (перезапишет старую)
    int triggerTrap();                // вернёт урон и уберёт ловушку

private:
    bool passable_{true};
    bool slowing_{};
    int  trapDamage_{0};
};
