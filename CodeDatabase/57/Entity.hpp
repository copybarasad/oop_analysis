#pragma once
#include <SFML/Graphics.hpp>
#include "./Field/Field.hpp"

enum EntityStatus {
    enumes,
    friends
};

class Cell;
class InterfaceGameLogger;
class Hero;
class GameLogger;

class Entity {    

protected:
    sf::Sprite sprite; 
    sf::Texture texture;
    sf::Vector2i location; 
    const int CELL_SIZE = 57;
    int level;
    int bonus_level_spell = 1;
    bool flag_low;
    int offsetX;
    int offsetY;
    int atack;
    int helpth;
    int max_helpth;
    std::string name;
    EntityStatus type_of_creature;

public:
    Entity(std::string puth_png, int startX, int startY, int offsetX, int offsetY,
     int atack, int helpth, std::string name, EntityStatus status, int level = 1);
    virtual ~Entity() = default;
    void updatePosition();
    void attacking_someone(Entity& who, Entity& someone, InterfaceGameLogger& logger_consol);
    void move(int dx, int dy, Field& field, InterfaceGameLogger& logger_consol);
    sf::Sprite& getSprite();
    int get_helpth();
    sf::Vector2i get_location() { return location; }
    std::string get_name() { return name; }
    void change_helpth(int damage, InterfaceGameLogger& logger_consol, std::vector<std::vector<Cell*>>& cells);
    EntityStatus get_status() { return type_of_creature; }
    virtual void move_towards_target(Field& field, InterfaceGameLogger& logger_consol) = 0;
    
    Entity* find_first_entity_by_status(Field& field, EntityStatus target_status);
    sf::Vector2i find_target_direction(Entity& target);

    virtual bool should_attack_on_collision() { return true; }
    void changeLevel(int lv) { level = lv; }
    int getLevel() { return level; }
    void set_location(int x, int y) { location.x = x; location.y = y;}
    void set_helpth(int hp) { helpth = hp; }
    void setLevel(int lvl, bool load = false);
    int get_max_helpth() {return max_helpth; }
    void updateOffsets(int newOffsetX, int newOffsetY);
    void update_parametrs(bool load = false);
    void set_level_spell(int new_level) { bonus_level_spell = new_level; }
    int get_level_spell() { return bonus_level_spell; }
};