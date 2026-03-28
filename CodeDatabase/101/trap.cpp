#include "trap.h"


Trap::Trap() {
    damage = 6;
    price = 35;
}

bool Trap::use_card(Visitor& visitor, int x, int y)
{
    return visitor.visitMap(x, y);
}

int Trap::get_price() const
{
    return this->price;
}

void Trap::upgrade(int level)
{
    damage += level;
}

int Trap::get_damage() const
{
    return damage;
}


void Trap::serialize(std::ostream& os) const  {
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&price), sizeof(price));

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Trap в поток");
    }
}

void Trap::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&price), sizeof(price));
    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Trap из потока");
    }
}
