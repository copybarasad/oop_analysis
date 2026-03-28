#include "move.hpp"

// Move::Move(Position pos): 
//     from(pos), to(pos) {}

// Move::Move(Position from, Position to): 
//     from(from), to(to) {}

// Move::Move(Position from, Position to, bool attack): 
//     from(from), to(to), attack(attack) {}

Move::Move(Position from, Position to, MoveType type): 
    from(from), to(to), type(type) {}

Move::Move(Position from, Position to, MoveType type, int spell_index): 
    from(from), to(to), type(type), spell_index(spell_index) {}