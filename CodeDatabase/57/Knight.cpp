#include <SFML/Graphics.hpp>
#include "Knight.hpp"
#include <iostream>
#include <random>

Knight::Knight(int startX, int startY, int offsetX, int offsetY, int level) : Entity("./entity/Berr.png",
     startX, startY, offsetX, offsetY, 50, 100, "Berr", friends, level) {
        std::cout << "Knight created at cell (" << startX << ", " << startY << ") with offset (" 
          << offsetX << ", " << offsetY << ")" << std::endl;

     }

void Knight::move_towards_target(Field& field, InterfaceGameLogger& logger_consol) {
    Entity* target = find_first_entity_by_status(field, enumes);

    if (target) {
        sf::Vector2i direction = find_target_direction(*target);
        move(direction.x, direction.y, field, logger_consol);
    } else {
            
        return;
    }
}

void Knight::random_move(Field& field, InterfaceGameLogger& logger_consol) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);
    
    int dx = 0, dy = 0;
    switch(dist(gen)) {
        case 0: dx = -1; break;
        case 1: dx = 1; break;
        case 2: dy = -1; break;
        case 3: dy = 1; break;
    }
    
    this->move(dx, dy, field, logger_consol);
}