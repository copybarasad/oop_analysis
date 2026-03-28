#ifndef INCLUDE_ENTITY
#define INCLUDE_ENTITY

#include "../ui/position.h"

class Entity {
private:
    int actual_health_;
    int max_health_;

    bool frozen_;
    bool on_freeze_cell_;
public:
    Entity() : actual_health_(100), max_health_(100), frozen_(false), on_freeze_cell_(false) {}
    Entity(int health) : actual_health_(health), max_health_(health), frozen_(false), on_freeze_cell_(false) {}
    ~Entity() = default;

    bool is_frozen() const { return this->frozen_; };
    bool on_freeze_cell() const { return this->on_freeze_cell_; };
    void set_frozen(bool frozen) { this->frozen_ = frozen; };
    void set_on_freeze(bool on_freeze) { this->on_freeze_cell_ = on_freeze; };
    void set_max_health(int max_health) {
        max_health_ = max_health;
        if (max_health_ < 1) {
            max_health_ = 1;
        }
        if (actual_health_ > max_health_) {
            actual_health_ = max_health_;
        };
    }

    int health() const { return actual_health_; }
    int max_health() const { return max_health_; }
    bool is_alive() const { return actual_health_ > 0; }

    void get_damage(int damage) {
        actual_health_ -= damage;
        if (actual_health_ < 0) { actual_health_ = 0; };
    }

    void set_health(int health) {
        actual_health_ = health;
        if (actual_health_ > max_health_) { actual_health_ = max_health_; }
        if (actual_health_ < 0) { actual_health_ = 0; }
    }

};


#endif