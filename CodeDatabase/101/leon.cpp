#include "leon.h"



Leon::Leon(int cx, int cy)
{
    x = cx;
    y = cy;
    health_ability = 2;
    leon_position = new Point(x, y, unlock);
}

Leon::~Leon()
{
    delete leon_position;
}


int Leon::get_leon_x() const
{
    return x;
}

int Leon::get_leon_y() const
{
    return y;
}

int Leon::get_health_ability() const
{
    return health_ability;
}

void Leon::set_point_x(int x)
{
    this->x = x;
}

void Leon::set_point_y(int y)
{
    this->y = y;
}

void Leon::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&health_ability), sizeof(health_ability));

    leon_position->serialize(os);

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Leon в поток");
    }
}

void Leon::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&health_ability), sizeof(health_ability));

    leon_position->deserialize(is);

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Leon из потока");
    }
}
