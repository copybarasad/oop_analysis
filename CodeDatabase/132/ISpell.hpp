#pragma once

#include <memory>
#include "position.hpp"
// #include "field_handler.hpp"

class FieldHandler;

class ISpell {
public:
    virtual char get_symbol() const = 0;
    virtual bool use(FieldHandler& field_handler, const Position& from, const Position& target) = 0;
    virtual int apply_enhancement(int current_lvl) = 0;
};
