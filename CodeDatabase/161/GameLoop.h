#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include "../map/Field.h"
#include "../entites/Player.h"
#include "../logic/World.h"
#include "Visualizer.h"

class GameLoop {
public:
	GameLoop();

private:
	int currentLevel;

	void StartLevel(World&, Field&, std::shared_ptr<Player>);
	bool CheckLevelComplete(Field& field);
	void getLevelMod(World&, Field&, std::shared_ptr<Player>&);

	bool LoadGameMenu(World&, Field&, std::shared_ptr<Player>&);
	void DeleteSaveMenu();
	void LevelUpMenu(std::shared_ptr<Player>&);

	void GenerateLevel(int&, int&, int&);
};

#endif //GAMELOOP_H