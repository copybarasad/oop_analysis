#include "improve.h"


Improve::Improve() {
    price = 60;
    level = 1;
}

bool Improve::use_card(Visitor& visitor, int x, int y)
{
    return visitor.visitMapForImprove(x, y);
}

int Improve::get_price() const
{
    return price;
}

void Improve::upgrade(int new_level)
{
    this->level = new_level + 1;
}

int Improve::get_level()
{
    return level;
}

void Improve::serialize(std::ostream& os) const {

    os.write(reinterpret_cast<const char*>(&price), sizeof(price));
    os.write(reinterpret_cast<const char*>(&level), sizeof(level));
    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Improve в поток");
    }
}

void Improve::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&price), sizeof(price));
    is.read(reinterpret_cast<char*>(&level), sizeof(level));

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Improve из потока");
    }
}
