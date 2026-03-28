#ifndef CLIVISUALIZER_H
#define CLIVISUALIZER_H


#include <iostream>
#include "field.h"
#include "game.h"
#include "hero.h"
#include <stdlib.h>
#include <utility>


class CLIVisualizer
{
public:
	CLIVisualizer();
	void showField(const Game& game);
	void showStage(int stage);
	void updateView(const Game& game);
	void showHeroStats(const Hero& hero);
};

#endif