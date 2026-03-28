#include "lightning.h"

Lightning::Lightning() {
    damage = 11;
    price = 30;
    width_damage = 2;
}

bool Lightning::use_card(Visitor& visitor, int x, int y)
{
    return visitor.visitEnemy(x, y, damage, 100, width_damage);
}

int Lightning::get_price() const
{
    return this->price;
}

void Lightning::upgrade(int level)
{
    this->width_damage = level + 1;
}


void Lightning::serialize(std::ostream& os) const  {
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&price), sizeof(price));
    os.write(reinterpret_cast<const char*>(&width_damage), sizeof(width_damage));

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Lightning в поток");
    }
}

void Lightning::deserialize(std::istream& is)  {
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&price), sizeof(price));
    is.read(reinterpret_cast<char*>(&width_damage), sizeof(width_damage));

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Lightning из потока");
    }

}
