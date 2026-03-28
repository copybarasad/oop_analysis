#include "Ally.h"
#include "Exceptions.h"

namespace Game {

Ally::Ally(Utils::Point initialPosition, int initialHealth, int initialDamage) :
    position(initialPosition),
    health(initialHealth),
    damage(initialDamage)
{}

Ally::Ally(std::istream& is) {
    deserialize(is);
}

void Ally::move(const Utils::Point& newPosition) {
    position = newPosition;
}

void Ally::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) {
        health = 0;
    }
}

int Ally::getHealth() const { return health; }
int Ally::getDamage() const { return damage; }
const Utils::Point& Ally::getPosition() const { return position; }
bool Ally::isAlive() const { return health > 0; }

void Ally::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&position), sizeof(position));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
}

void Ally::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&position), sizeof(position));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    if (is.fail()) throw FileReadException("Failed to read Ally data.");
}

}