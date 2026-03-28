#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Cell.hpp"

class Field {
private:
    unsigned int clamped_x, clamped_y;
    int offsetX, offsetY;
    std::vector<std::vector<Cell*>> cells;
    const int CELL_SIZE = 57;
    const unsigned int MIN_SIZE_FIELD = 10u;
    const unsigned int MAX_SIZE_FIELD = 25u;
    const float CHANCE_OF_SPAWNING_IMPASSABLE_CELL = 0.1f;
    const float CHANCE_OF_SPAWNING_SLOWING_CELL = 0.2f;

public:
    Field(sf::Vector2u size);
    ~Field();
    
    std::vector<std::vector<Cell*>>& get_cells() { return cells; }
    sf::Vector2u get_offset() const { return {static_cast<unsigned int>(offsetX), 
                                             static_cast<unsigned int>(offsetY)}; }
    sf::Vector2u get_grid_size() const { return {clamped_x, clamped_y}; }
    int get_cell_size() const { return CELL_SIZE; }
    Entity* findEnemyInRadius(int x, int y, int radius);
    void placeTrap(int x, int y, int damage);
    void ramoveTrap(int x, int y);
    Field(Field& other);
    Field(Field&& other);
    Field& operator=(const Field& other);
    Field& operator=(Field&& other);
    
private:
    void setupSprites();
    std::vector<std::vector<Cell*>> generate_Field(int width, int height);
    

    void deepCopy(const Field& other);
    Cell* createCellByStatus(Status s);
    void setCellType(int x, int y, Status s);
};