#pragma once
#include "basic_battler.h"

class Cell {
public:
	Cell();
	~Cell() = default;

	bool isFree();
	void clean();

	BasicBattler* getBattler() const;
	void setBattler(BasicBattler* new_battler);


private:
	BasicBattler* battler_;
};
