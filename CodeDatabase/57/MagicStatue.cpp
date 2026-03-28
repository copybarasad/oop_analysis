#include <SFML/Graphics.hpp>
#include "MagicStatue.hpp"
#include <algorithm>
#include <iostream>
#include "./Systems/InterfaceGameLogger.hpp"

MagicStatue::MagicStatue(int x, int y, int offsetX, int offsetY, int level) : Enemy::Enemy("./spawn/statue.png", x, y, offsetX, offsetY, 40, 200, "MagicStatue", level) {
    attackSpell = new FireballSpell(this->atack, RADIUS_ATTACK);
}
      

void MagicStatue::attack(Field& field, InterfaceGameLogger& logger_consol, int frequency) {
    if(frequency % ATTACK_FREQUENCY != 0) return;
    std::vector<std::vector<Cell*>>& vector_Cell = field.get_cells();
    for(int cellX = this->location.x - RADIUS_ATTACK; cellX <= this->location.x + RADIUS_ATTACK; cellX++) {
        for(int cellY = this->location.y - RADIUS_ATTACK; cellY <= this->location.y + RADIUS_ATTACK; cellY++) {

            if (cellX >= 0 && cellX < int(field.get_grid_size().x) && cellY >= 0 && (cellY < int(field.get_grid_size().y))) {
                if(vector_Cell[cellX][cellY]->Cell_get_use() && vector_Cell[cellX][cellY]->Cell_get_entity()->get_status() == friends) {
                    logger_consol.log("Magic statue spell hit " + vector_Cell[cellX][cellY]->Cell_get_entity()->get_name() + " for " + std::to_string(this->atack) + " damage!\n");
                    attackSpell->cast(*this, field, logger_consol, cellX, cellY);
                }
            }
        }
    }
} 