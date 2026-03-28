#include "poison.h"


Poison::Poison() {
    damage = 6;
    price = 30;
    radius = 6;
}

bool Poison::use_card(Visitor& visitor, int x, int y)
{
    return visitor.visitEnemy(x, y, damage, radius, 0);

}

int Poison::get_damage() const
{
    return this-> damage;
}

int Poison::get_price() const
{
    return this->price;
}

void Poison::upgrade(int level)
{
    this->radius += level/2 + 1;
}

void Poison::serialize(std::ostream& os) const  {
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&price), sizeof(price));
    os.write(reinterpret_cast<const char*>(&radius), sizeof(radius));
    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Poison в поток");
    }
}

void Poison::deserialize(std::istream& is)  {
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&price), sizeof(price));
    is.read(reinterpret_cast<char*>(&radius), sizeof(radius));

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Poison из потока");
    }

}
