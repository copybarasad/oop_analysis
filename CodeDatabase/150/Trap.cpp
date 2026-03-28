#include "Trap.hpp"

Trap::Trap() : positionX_(0), positionY_(0), damage_(25), activated_(false) {}

Trap::Trap(int x, int y, int damage)
    : positionX_(x), positionY_(y), damage_(damage), activated_(false) {}

void Trap::activate() { 
    activated_ = true; 
}

void Trap::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&positionX_), sizeof(positionX_));
    os.write(reinterpret_cast<const char*>(&positionY_), sizeof(positionY_));
    os.write(reinterpret_cast<const char*>(&damage_), sizeof(damage_));
    os.write(reinterpret_cast<const char*>(&activated_), sizeof(activated_));
}

void Trap::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&positionX_), sizeof(positionX_));
    is.read(reinterpret_cast<char*>(&positionY_), sizeof(positionY_));
    is.read(reinterpret_cast<char*>(&damage_), sizeof(damage_));
    is.read(reinterpret_cast<char*>(&activated_), sizeof(activated_));
}