#pragma once
#include "Entity.h"
#include "AttackMode.h"
#include "struct/dataPlayer.h"


class Player : public Entity {
private:
	AttackMode attackMode;
	int score;
	int murders;
public:
	Player();
	Player(dataPlayer data);
	~Player() override;
	

	void SwitchAttackMode();
	void AddScore(int s);
	int GetScore() const;
	int GetMurders();

	AttackMode GetAttackMode();
	
};
