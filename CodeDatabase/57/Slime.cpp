#include <SFML/Graphics.hpp>
#include "Slime.hpp"
#include <iostream>


Slime::Slime(int startX, int startY, int offsetX, int offsetY, int level) : Enemy("./entity/enemy/Slime.png",
     startX, startY, offsetX, offsetY, 20, 400, "Slime", level) {}