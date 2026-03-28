#include "tower.hpp"
#include "game_world/map.hpp"
#include <iostream>

Tower::Tower(int x, int y) : Entity(x, y, Faction::Enemy, 5, 0) {}
