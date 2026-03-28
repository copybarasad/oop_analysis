#ifndef COMMANDS
#define COMMANDS

#include <fstream>
#include "GameLogic.h"
#include "PlayerConfig.h"

#include "JsonSerializer.h"

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute(GameLogic&) = 0;
};

class MoveCommand : public ICommand
{
	std::pair<int, int> steps;
public:
	MoveCommand(std::pair<int, int>);
	void Execute(GameLogic&) override;
};

class AttackEnemyCommand : public ICommand
{
	int pointsForKilling;
	int numberOfCurrentAttackedEnemy;
public:
	AttackEnemyCommand(int, int);
	void Execute(GameLogic&) override;
};

class AttackSpawnerCommand : public ICommand
{
	int pointsForKilling;
public:
	AttackSpawnerCommand(int);
	void Execute(GameLogic&) override;
};

class AttackTowerCommand : public ICommand
{
	int pointsForKilling;
public:
	AttackTowerCommand(int);
	void Execute(GameLogic&) override;
};

class SwitchTypeOfAttackCommand : public ICommand
{
public:
	void Execute(GameLogic&) override;
};

class BuySpellCommand : public ICommand
{
	int numberOfSpell;
public:
	BuySpellCommand(int);
	void Execute(GameLogic&) override;
};

class UseSpellCommand : public ICommand
{
	int numberOfSpell;
	std::pair<int, int> targetPos;
public:
	UseSpellCommand(int, std::pair<int, int>);
	void Execute(GameLogic&) override;
};

class SaveCommand : public ICommand
{
	PlayerConfig dataAboutPlayer;
	std::string fileName;
public:
	SaveCommand(PlayerConfig&, std::string&);
	void Execute(GameLogic&) override;
};

class LoadCommand : public ICommand
{
	PlayerConfig& dataAboutPlayer;
	std::string fileName;
public:
	LoadCommand(PlayerConfig&, std::string&);
	void Execute(GameLogic&) override;
};

#endif
