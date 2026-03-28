#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class Entity;

enum Status {
    slow,
    ordinary,
    impassable,
    trap
};

class Cell {
protected:
    Status status;
    bool use;
    sf::Texture texture;
    Entity* entity;
    sf::Sprite sprite;
    
public:
    Cell();
    Cell(std::string puth_png, Status speed);
    
    bool Cell_get_use() const;
    Entity* Cell_get_entity() const;
    void Cell_send_use(bool use, Entity* entity = nullptr);
    sf::Sprite& getSprite();
    Status get_status_cell() { return status; }
    void set_status_cell(Status new_status) { status = new_status; }
};

