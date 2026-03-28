//
// Created by bob on 10/4/25.
//

#include "Cell.h"


Cell::Cell(const State &state): state(state) {}

State Cell::getState() const { return this->state; }

void Cell::setState(const State &state) {this->state = state; }

void Cell::save(Saver &saver) const {
    int value = static_cast<int>(state);
    saver.save(value);
}

void Cell::load(Reader &reader) {
    int value;
    reader.read(value);
    state = static_cast<State>(value);
}

