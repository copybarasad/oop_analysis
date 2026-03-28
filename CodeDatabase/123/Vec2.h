#ifndef STRUCTS
#define STRUCTS

#include "SFML/Graphics.hpp"

using Vec2 = sf::Vector2i;

// struct Vec2
// {
//     int x;
//     int y;

//     Vec2()
//     {
//         x = 0;
//         y = 0;
//     }

//     Vec2(int x, int y) : x(x), y(y)
//     {
//     }

//     Vec2 operator+(Vec2 p)
//     {
//         return Vec2(p.x + x, p.y + y);
//     }

//     void operator+=(Vec2 p)
//     {
//         *this = *this + p;
//     }

//     Vec2 operator-(Vec2 p)
//     {
//         return Vec2(x - p.x, y - p.y);
//     }

//     void operator-=(Vec2 p)
//     {
//         *this = *this - p;
//     }

//     bool operator==(Vec2 p)
//     {
//         return x == p.x && y == p.y;
//     }

//     bool operator!=(Vec2 p)
//     {
//         return !(x == p.x && y == p.y);
//     }
// };

#endif