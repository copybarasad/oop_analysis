#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <fstream>
#include <utility>
#include "game.h"
#include "enemyManager.h"
#include "enemyBuildingManager.h"
#include "field.h"
#include "spellFactory.h"
#include "CLIVisualizer.h"
#include "observer.h"

class Serializer
{
public:
	Serializer();
	static Serializer& getInstance();
	void saveGame(std::string filename, const Game* game);
	Game* loadGame(std::string filename);
};
#endif