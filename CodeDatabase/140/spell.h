#pragma once
#include <string>
#include <iostream>

using namespace std;

class Field;

class Spell {
public:
    virtual ~Spell() = default;
    virtual void use(Field& field, int caster_x, int caster_y, int target_x, int target_y) = 0;
    virtual Spell* clone() const = 0;
    virtual string get_name() const = 0;
    virtual void upgrade() {}
};