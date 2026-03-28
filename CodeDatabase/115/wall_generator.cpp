#include "wall_generator.h"
#include "playing_field.h"
#include "cell.h"
#include <cstdlib>
#include <ctime>

void WallGenerator::generateWalls(Playing_field& field) {
    generate_border_walls(field);
    generate_random_walls(field);
}

void WallGenerator::generate_border_walls(Playing_field& field) {
    int width = field.get_width();
    int length = field.get_length();
    
    for (int x = 0; x < width; x++) {
        field.get_cell(x, 0).set_empty(false); 
        field.get_cell(x, length - 1).set_empty(false);
    }
    
    for (int y = 0; y < length; y++) {
        field.get_cell(0, y).set_empty(false);
        field.get_cell(width - 1, y).set_empty(false);
    }
}

void WallGenerator::generate_random_walls(Playing_field& field) {
    int width = field.get_width();
    int length = field.get_length();
    
    int wall_count = (width * length) / 10;
    
    for (int i = 0; i < wall_count; i++) {
        int x = std::rand() % (width - 2) + 1;
        int y = std::rand() % (length - 2) + 1;
        
        if (x == 5 && y == 5) {
            continue;
        }
        
        if (field.get_cell(x, y).is_empty()) {
            field.get_cell(x, y).set_empty(false); 
        }
    }
}