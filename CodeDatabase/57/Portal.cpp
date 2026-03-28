#include <SFML/Graphics.hpp>
#include "Portal.hpp"
#include <algorithm>
#include <iostream>

Portal::Portal(int x, int y, int offsetX, int offsetY, int spawn_frequency, int level) : Entity::Entity("./spawn/portal.png",
      x, y, offsetX, offsetY, 0, 50, "Portal", enumes, level), spawn_frequency(spawn_frequency) {}
      

Enemy* Portal::generate_emply(std::vector<std::vector<Cell*>>& vector_Cell, Field& field) {
    for(int x = location.x - RADIUS_SPAWN; x <= location.x + RADIUS_SPAWN; x++) {
        for(int y = location.y - RADIUS_SPAWN; y <= location.y + RADIUS_SPAWN; y++) {
            sf::Vector2u grid_size = field.get_grid_size();
            if(0 <= x && x < int(grid_size.x) && 0 <= y && y < int(grid_size.y)) {

                if(!vector_Cell[x][y]->Cell_get_use() && vector_Cell[x][y]->get_status_cell() == ordinary) {

                    sf::Vector2u offset = field.get_offset();
                    Goblin* new_enemy = new Goblin(x, y, offset.x, offset.y);
                    new_enemy->setLevel(level, true);
                    vector_Cell[x][y]->Cell_send_use(true, new_enemy);
                    return new_enemy;

                }

            }
        }
    }
    return nullptr;
}
