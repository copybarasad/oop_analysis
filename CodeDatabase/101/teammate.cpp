#include "teammate.h"


Teammate::Teammate() {
    price = 80;
    x = 10000;
    y = 10000;
    count = 1;
}

bool Teammate::use_card(Visitor& visitor, int x, int y)
{
    return visitor.visitGame(x, y, count);
}

int Teammate::get_price() const
{
    return this->price;
}

void Teammate::upgrade(int level)
{
    count += level/2 + 1;
}

void Teammate::serialize(std::ostream& os) const  {
    os.write(reinterpret_cast<const char*>(&price), sizeof(price));
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&count), sizeof(count));

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Teammate в поток");
    }
}

void Teammate::deserialize(std::istream& is)  {
    is.read(reinterpret_cast<char*>(&price), sizeof(price));
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&count), sizeof(count));

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Teammate из потока");
    }
}
