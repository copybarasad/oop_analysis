#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"

// Player — класс игрока. Хранит счёт очков и переопределяет символ отображения.
// Координаты игрока хранятся исключительно в классе GameField (по ТЗ), поэтому
// здесь их нет.
class Player : public Unit {
public:
	explicit Player(int health = 100, int damage = 20);
	
	void addScore(int value);
	int get_score() const;
	void change_attack_version();
	char get_attack_version() const;
	void set_score(int value);
private:
	char attack_version {'M'};
	int _score {0};
};

#endif // PLAYER_H
