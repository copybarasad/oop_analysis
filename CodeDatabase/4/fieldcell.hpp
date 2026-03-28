#pragma once

enum Type { EMPTY, HARD, SLOW, TRAP };
enum EntityType { HERO, MOB, TOWER, ALLY, ATTACK_TOWER, ZERO }; 

class FieldCell {
private:
    Type type;
    EntityType entity;
public:
    int id;

    FieldCell(Type temp_type);
    FieldCell(Type temp_type, EntityType temp_entity, int number);
    EntityType get_entity() const;
    Type get_type() const;
    void set_type(Type t) { type = t; }
};