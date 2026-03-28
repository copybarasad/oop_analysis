#include <SFML/Graphics.hpp>
#include "Shadow.hpp"
#include <iostream>


Shadow::Shadow(int startX, int startY, int offsetX, int offsetY, int level) : Enemy("./entity/enemy/Shadow.png",
     startX, startY, offsetX, offsetY, 70, 300, "Shadow", level) {}