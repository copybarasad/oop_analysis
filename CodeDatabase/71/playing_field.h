#pragma once
#include "basic_battler.h"
#include "cell.h"
#include "player.h"
#include "direction.h"
#include <vector>
#include <utility>

enum class MoveResult {
	SUCCESS_MOVE,
	SUCCESS_ATTACK,
	TARGET_KILLED,
	PLAYER_DIED,
	ALLY_IN_THE_WAY,
	BOUNDARY_HIT,
	INVALID_BATTLER
};


class PlayingField {
public:

	PlayingField(int height, int width);
	~PlayingField() = default;
	PlayingField(const PlayingField& other); 
	PlayingField& operator = (const PlayingField& other); 
	PlayingField(PlayingField&& other) noexcept; 
	PlayingField& operator = (PlayingField&& other) noexcept;

	void clearAllEnemies();

	MoveResult move(BasicBattler* batttler, Direction direction);
	MoveResult moveLeft(BasicBattler* battler);
	MoveResult moveRight(BasicBattler* battler);
	MoveResult moveUp(BasicBattler* battler);
	MoveResult moveDown(BasicBattler* battle);

	void placeBattler(int x, int y, BasicBattler* battler);

	int getWidth() const;
	int getHeight() const;
	std::pair<int, int> getBattlerCoords(BasicBattler* battler) const;
	BasicBattler* getBattlerAt(int x, int y) const;

private:
	std::vector<std::vector<Cell>> field;
	int height_;
	int width_;
};
