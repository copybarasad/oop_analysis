#include "spell.hpp"

Spell::Spell(char symbol): symbol(symbol) {}

char Spell::get_symbol() const {
    return symbol;
}