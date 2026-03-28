#include "point.h"

Point::Point(int x, int y, State opt) {
    this->x = x;
    this->y = y;
    this->option = opt;
}

Point::Point() {
}


int Point::get_point_x() const{
    return this->x;
}

int Point::get_point_y() const{
    return this->y;
}

 State Point::get_point_opt() const{
    return this->option;
}

void Point::set_option(State opt)
{
    this->option = opt;
}

void Point::set_point_x(int dx)
{
    this->x += dx;
}

void Point::set_point_y(int dy)
{
    this->y += dy;
}

void Point::serialize(std::ostream &os) const
{
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));

    int stateValue = static_cast<int>(option);
    os.write(reinterpret_cast<const char*>(&stateValue), sizeof(stateValue));

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Point в поток");
    }
}

void Point::deserialize(std::istream &is)
{
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    int stateValue;
    is.read(reinterpret_cast<char*>(&stateValue), sizeof(stateValue));
    option = static_cast<State>(stateValue);

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Point из потока");
    }
}


