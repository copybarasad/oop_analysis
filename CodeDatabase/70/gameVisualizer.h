#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "game.h"
#include "observer.h"

template<typename Visualizer>
class GameVisualizer : public Observer<Game>
{
private:
	Visualizer visualizer;
public:
	void update(const Game& game) override {
        visualizer.updateView(game);
    }
};

#endif