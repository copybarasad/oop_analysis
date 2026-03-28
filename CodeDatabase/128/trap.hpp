#pragma once

class Trap {
public:
    Trap(int x, int y, int damage, bool active = true)
        : x_(x), y_(y), damage_(damage), active_(active) {}

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getDamage() const noexcept { return damage_; }
    bool IsActive() const noexcept { return active_; }
    void Deactivate() noexcept { active_ = false; }

private:    
    int x_;
    int y_;
    int damage_;
    bool active_;
};
