#include "trap.h"
#include "save_manager.h"

Trap::Trap(int x, int y, int damage)
    : position_(x, y), damage_(damage), active_(true) {}

Position Trap::getPosition() const { return position_; }
int Trap::getDamage() const { return damage_; }
bool Trap::isActive() const { return active_; }
void Trap::deactivate() { active_ = false; }

void Trap::save(std::ostream& os) const {
    os << position_.x << " " << position_.y << " " << damage_ << " " << active_ << "\n";
}

void Trap::load(std::istream& is) {
    int activeInt;
    is >> position_.x >> position_.y >> damage_ >> activeInt;
    if (is.fail()) throw SaveDataException("Failed to load trap data");
    active_ = (activeInt != 0);
    std::string leftover;
    std::getline(is, leftover);
}