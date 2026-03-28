#pragma once
#include "Interfaces/GameBoard.h"
#include "CollisionDetector.h"
#include "Settings/PointSystem.h"
#include "Settings/GBSettings.h"
#include "DataTypes/SpellType.h"
#include "storage/JsonManager.h"

class ExecuteUseCase
{
private:
	GameBoard* gameBoard = nullptr;
	JsonManager* jsonManager = nullptr;
	GBSettings* gameBoardSettings = nullptr;
	CollisionDetector collisionDetector;
	PointSystem pointSystem;
public:
	ExecuteUseCase(int width, int height, int cellSize);
	void saveGame();
	bool loadGame(unsigned short& level);
	void restartGame();
	Point newGame(int level = 1, int addHealth = 0, int addDamage = 0);
	void checkSpawnNewEnemy();
	void increasePlayerSteps();
	void changePlayerLongFight();
	void changePlayerFastMode();
	const PointSystem& getPointSystem() const;
	const GameBoard* getGameBoard() const;
	bool checkGameEnd() const;
	bool checkGameWin() const;
	bool checkGameOver() const;
	bool executePlayer(const Point& vector);
	bool executeEntity();
	bool executePlayerAttack(Point& vector);
	void takeSpell(int index);
	bool addSpell(SpellType type);
	bool useSpell();
	bool useSpell(const Point& destination);
	bool refresh();
	~ExecuteUseCase();
private:
	bool executeAlly();
	bool executeArcherTower();
	bool refreshTraps();
	bool refreshEntities();
};