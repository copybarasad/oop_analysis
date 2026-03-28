#include <SFML/Graphics.hpp>
#include "Demon.hpp"
#include <iostream>


Demon::Demon(int startX, int startY, int offsetX, int offsetY, int level) : Enemy("./entity/enemy/Demon.png",
     startX, startY, offsetX, offsetY, 57, 400, "Demon", level) {}