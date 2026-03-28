#include "Creature.h"
#include "Cell.h"
#include "Field.h"

Creature::Creature(char e, int hp, int damage) : emblem(e), current_hp(hp), current_damage(damage) {}
    
int Creature::getIs_dead() const { return is_dead; }

void Creature::deal_damage(int damage) {
    current_hp -= damage;
    if(current_hp <= 0){
        is_dead = true;
    }
}

void Creature::set_current_cell(Cell* cell_ptr) { current_cell = cell_ptr; }

int Creature::getDamage() const { return current_damage; }

int Creature::getLoadX() const { return load_x; }

int Creature::getLoadY() const { return load_y; }

Cell* Creature::get_current_cell() const { return current_cell; }

bool Creature::move(int dx, int dy, Field *game_field) {
    if (!current_cell){
        return false;
    }
    int current_x = current_cell->getX();
    int current_y = current_cell->getY();
    int new_x = current_x + dx;
    int new_y = current_y + dy;

    Cell* next_cell = game_field->get_cell(new_x, new_y); 

    if(next_cell && (next_cell->getObject() == static_cast<char>(ObjectType::Empty) || next_cell->getObject() == static_cast<char>(ObjectType::Spike))){
        if(next_cell->getObject() == static_cast<char>(ObjectType::Spike)){
            this->deal_damage(SPIKE_DAMAGE);
        }
        ObjectType obj_type = (emblem == static_cast<char>(ObjectType::Player)) ? ObjectType::Player : ObjectType::Enemy;
        game_field->place_object_field(new_x, new_y, obj_type, this);
        game_field->remove_object_field(current_x, current_y);
        this->current_cell = next_cell;
        return true;
    }
    return false;
}

void Creature::save(std::ofstream& ofs) const {
    ofs << current_hp << " ";
    ofs << current_damage << " ";
    ofs << is_dead << " ";
    if (current_cell) {
        ofs << current_cell->getX() << " " << current_cell->getY() << " ";
    } else {
        ofs << "-1 -1 "; 
    }
}

void Creature::load(std::ifstream& ifs) {
    ifs >> current_hp;
    ifs >> current_damage;
    ifs >> is_dead;
    ifs >> load_x >> load_y; 
}