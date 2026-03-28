#include <SFML/Graphics.hpp>
#include "Goblin.hpp"
#include <iostream>


Goblin::Goblin(int startX, int startY, int offsetX, int offsetY, int level) : Enemy("./entity/enemy/Goblin.png",
     startX, startY, offsetX, offsetY, 50, 10, "Goblin", level) {}