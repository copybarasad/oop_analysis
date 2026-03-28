#include "../include/entity/hand.h"
#include "../include/core/game_board.h"

Hand::Hand() : active_spell_(nullptr) {}

Hand::~Hand() {
    delete active_spell_;
}

void Hand::equip_spell(Spell* spell) {
    delete active_spell_;
    active_spell_ = spell;
}

void Hand::clear() {
    delete active_spell_;
    active_spell_ = nullptr;
}

bool Hand::use_spell(GameBoard& board) {
    if (!active_spell_) {
        return false;
    }
    
    active_spell_->Use(board);
    return true;
}

Spell* Hand::active_spell_copy() {
    if (!active_spell_) {
        return nullptr;
    }
    
    Spell* copy = active_spell_->Clone();
    return copy;
}