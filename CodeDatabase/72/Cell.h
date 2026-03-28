//
// Created by bob on 10/4/25.
//

#ifndef GAME_TERM_CELL_H
#define GAME_TERM_CELL_H
#include "../TargetSelection.h"
#include "../utils /ISerializable.h"


enum class State{Free, Slow, Obstacle};

class Cell: public ISerializable {

    State state;

public:

    Cell(const State & state = State::Free);

    State getState() const;

    void setState(const State & state);

    void save(Saver &saver) const override;

    void load(Reader &reader) override;

};


#endif //GAME_TERM_CELL_H