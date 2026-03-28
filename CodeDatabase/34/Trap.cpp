#include "Trap.h"
#include "Exceptions.h"

namespace Game {

Trap::Trap(const Utils::Point& position, int damage)
    : position(position), damage(damage), triggered(false) {}

Trap::Trap(std::istream& is) {
    deserialize(is);
}

const Utils::Point& Trap::getPosition() const {
    return position;
}

int Trap::getDamage() const {
    return damage;
}

bool Trap::isTriggered() const {
    return triggered;
}

void Trap::trigger() {
    triggered = true;
}

void Trap::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&position), sizeof(position));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&triggered), sizeof(triggered));
}

void Trap::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&position), sizeof(position));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&triggered), sizeof(triggered));
    if (is.fail()) throw FileReadException("Failed to read Trap data.");
}

}