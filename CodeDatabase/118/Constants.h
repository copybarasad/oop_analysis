#ifndef CONSTANTS_H
#define CONSTANTS_H

int constexpr ENCRYPTION_KEY = 0x3A7B9A;

int constexpr PLAYER_MELEE_DAMAGE = 25;
int constexpr PLAYER_RANGED_DAMAGE = 15;
int constexpr PLAYER_HEALTH = 100;

int constexpr ENEMY_DAMAGE = 5;
int constexpr ENEMY_HEALTH = 40;
int constexpr ENEMY_KILL_SCORE = 20;

int constexpr SPAWNER_HEALTH = 150;
int constexpr SPAWNER_COOLDOWN = 6;
int constexpr SPAWNER_RADIUS = 1;
int constexpr SPAWNER_KILL_SCORE = 100;

int constexpr STANDART_HAND_CAP = 5;

int constexpr TRAP_DAMAGE = 10;

int constexpr TOWER_HEALTH = 80;
int constexpr TOWER_DEBUFF = 15;
int constexpr TOWER_RADIUS = 4;
int constexpr TOWER_KILL_SCORE = 80;

int constexpr DIRECT_DAMAGE = 20;
int constexpr AREA_DAMAGE = 10;

int constexpr DIRECT_COST = 40;
int constexpr AREA_COST = 60;
int constexpr TRAP_COST = 50;

int constexpr STANDART_SPELL_RADIUS = 4;

enum TypeOfAttack { MELEE, RANGED };

enum TypeOfCell { ORDINAL, SLOWING, IMPASSABLE, TRAP };

enum Command
{
	UNKNOWN_COMMAND,
	MOVE_FORWARD,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	ATTACK_ENEMY,
	ATTACK_SPAWNER,
	ATTACK_TOWER,
	BUY_SPELL,
	USE_SPELL_WITH_NUMBER,
	SWITCH_TYPE_OF_ATTACK,
	SAVE,
	LOAD,
	QUIT,
	NEW_GAME
};

#endif