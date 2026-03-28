#include "Field.hpp"
#include "Managers/Entity_Manager.hpp"
#include "./Systems/Render_System.hpp"
#include "./Field/OrdinaryCell.hpp"
#include "./Field/SlowingCell.hpp"
#include "./Field/ImpassableCell.hpp"
#include "./Field/TrapCell.hpp"
#include <random>

Field::Field(sf::Vector2u size) {
    
    clamped_x = std::clamp(size.x, MIN_SIZE_FIELD, MAX_SIZE_FIELD);
    clamped_y = std::clamp(size.y, MIN_SIZE_FIELD, MAX_SIZE_FIELD);
    
    cells = generate_Field(clamped_x, clamped_y);
    
    setupSprites();
}

Field::~Field() {
    for (unsigned int x = 0; x < clamped_x; ++x) {
        for (unsigned int y = 0; y < clamped_y; ++y) {
            delete cells[x][y];
        }
    }
}

void Field::setupSprites() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    offsetX = (desktop.width - clamped_x * CELL_SIZE) / 2;
    offsetY = (desktop.height - clamped_y * CELL_SIZE) / 2;

    for (unsigned int x = 0; x < clamped_x; ++x) {
        for (unsigned int y = 0; y < clamped_y; ++y) {
            if (cells[x][y]) {
                cells[x][y]->getSprite().setPosition(
                    offsetX + x * CELL_SIZE, 
                    offsetY + y * CELL_SIZE
                );
            }
        }
    }
}

std::vector<std::vector<Cell*>> Field::generate_Field(int width, int height) {
    std::vector<std::vector<Cell*>> map(width, std::vector<Cell*>(height));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double r = dist(gen);
            
            if (r < CHANCE_OF_SPAWNING_IMPASSABLE_CELL) {
                map[x][y] = new ImpassableCell();
            } else if (r < CHANCE_OF_SPAWNING_SLOWING_CELL) {
                map[x][y] = new SlowingCell();
            } else {
                map[x][y] = new OrdinaryCell();
            }
        }
    }
    return map;
}

Entity* Field::findEnemyInRadius(int x, int y, int radius) {
    for(int seek_x = x - radius; seek_x <= x + radius; seek_x++) {
        for(int seek_y = y - radius; seek_y <= y + radius; seek_y++) {
            if(cells[seek_x][seek_y]->Cell_get_use() && seek_x != x && seek_y != y) {
                return cells[seek_x][seek_y]->Cell_get_entity();
            }
        }
    }
    return nullptr;
}

void Field::placeTrap(int x, int y, int damage) {
    TrapCell* new_trap = new TrapCell(damage);
    delete cells[x][y];
    cells[x][y] = new_trap;

    cells[x][y]->getSprite().setPosition(
            offsetX + x * CELL_SIZE,
            offsetY + y * CELL_SIZE
        );


}

void Field::ramoveTrap(int x, int y) { 
    if(cells[x][y]->get_status_cell() == trap) {
        OrdinaryCell* new_trap = new OrdinaryCell();
        delete cells[x][y];
        cells[x][y] = new_trap;

        cells[x][y]->getSprite().setPosition(
            offsetX + x * CELL_SIZE,
            offsetY + y * CELL_SIZE
        );
    }
}

Field::Field(Field& other) {
    deepCopy(other);
}

void Field::deepCopy(const Field& other) {
    clamped_x = other.clamped_x;
    clamped_y = other.clamped_y;
    offsetX = other.offsetX;
    offsetY = other.offsetY;
    
    cells.resize(clamped_x, std::vector<Cell*>(clamped_y, nullptr));
    for (unsigned int x = 0; x < clamped_x; ++x) {
        for (unsigned int y = 0; y < clamped_y; ++y) {
            if (other.cells[x][y]) {
                int speed = other.cells[x][y]->get_status_cell();
                if (speed == impassable) {
                    cells[x][y] = new ImpassableCell();
                } else if (speed == slow) {
                    cells[x][y] = new SlowingCell();
                } else {
                    cells[x][y] = new OrdinaryCell();
                }
            }
        }
    }
    setupSprites();
}

Field::Field(Field&& other) : clamped_x(other.clamped_x),
      clamped_y(other.clamped_y), offsetX(other.offsetX),
      offsetY(other.offsetY), cells(std::move(other.cells)) {
    other.clamped_x = 0;
    other.clamped_y = 0;
    other.offsetX = 0;
    other.offsetY = 0;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        for (auto& row : cells) {
            for (auto& cell : row) {
                delete cell;
            }
        }
        cells.clear();
        
        deepCopy(other);
    }
    return *this;
}

Field& Field::operator=(Field&& other) {
    std::swap(clamped_x, other.clamped_x);
    std::swap(clamped_y, other.clamped_y);
    std::swap(offsetX, other.offsetX);
    std::swap(offsetY, other.offsetY);
    std::swap(cells, other.cells);
    return *this;
}

Cell* Field::createCellByStatus(Status s) {
    switch(s) {
        case impassable: return new ImpassableCell();
        case slow: return new SlowingCell();
        case ordinary:
        default: return new OrdinaryCell();
    }
}

void Field::setCellType(int x, int y, Status s) {
    if (cells[x][y]) {
        delete cells[x][y];
    }
    cells[x][y] = createCellByStatus(s);
}