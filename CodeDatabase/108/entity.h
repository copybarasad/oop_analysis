#ifndef OOP_ENTITY_H
#define OOP_ENTITY_H

#include "position.h"
#include <string>


class Entity {
protected:
    Position position;
    int health;
    int max_health;
    int damage;
    std::string name;

public:
    explicit Entity(std::string name, const Position& position, int health, int damage);   //NEW: добавлено имя для вывода, кого бью

    // виртуальный деструктор
    // гарантирует вызов деструкторов производных классов
    virtual ~Entity() = default;

    void take_damage(int damage);

    bool is_alive() const;
    Position get_position() const;
    int get_health() const;
    int get_max_health() const;
    int get_damage() const;
    std::string get_name() const;

    void set_position(const Position& new_position);
    void set_max_health(int new_max_health);
    void set_health(int new_health);
};

#endif //OOP_ENTITY_H