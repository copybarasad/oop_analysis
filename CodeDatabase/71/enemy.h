#pragma once
#include "basic_battler.h"

class Enemy : public BasicBattler{
public:
	~Enemy() = default;
	using BasicBattler::BasicBattler;
	Faction getFaction() const override;
};
