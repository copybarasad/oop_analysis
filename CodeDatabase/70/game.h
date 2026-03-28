#ifndef GAME_H
#define GAME_H


#include <iostream>
#include "hero.h"
#include "field.h"
#include "enemyManager.h"
#include "enemyBuildingManager.h"
#include "enemyBuilding.h"
#include "spellFactory.h"
#include "Menus/inputGetter.h"
#include "subject.h"
#include <string>
#include <map>
#include <format>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class SpellFactory;

class Game : public Subject<Game>
{
private:
	Field field;
	Hero hero;
	std::pair<int, int> heroPos;
	EnemyManager enemyManager;
	EnemyBuildingManager enemyBuildingManager;
	char enemyId;
	char heroId;
	SpellFactory* heroSpellFactory;
	SpellFactory* enemySpellFactory;
	size_t stage;	

public:
	Game();
	~Game();
	Game(char enemyId, char heroId);
	void gameInit();
	bool gameConditions();
	bool stageConditions();
	std::pair<int, int> getCell();
	void gameCycle(int actions);
	void enemyTurn();
	bool setHeroPos(int x, int y);
	void swapHeroRange();
	void showHeroPos();
	bool heroAttacks(int x, int y);
	bool attackBuilding(int x, int y, int damage);
	bool attackEnemy(int x, int y, int damage);
	bool isCellInRange(int x, int y, int cell_x, int cell_y, int range);

	//SPELLS FIXING
	const EnemyManager& getEnemyManager() const ;
	const EnemyBuildingManager& getEnemyBuildingManager() const ;
	const std::pair<int, int>& getHeroPos() const ;
	const Hero& getHero() const ;
	const Field& getField() const ;
	Hero& getHero();
	Field& getField();
	SpellFactory* getHeroSpellFactory() const ;
	SpellFactory* getEnemySpellFactory() const ;
	char getEnemyId() const ;
	char getHeroId() const ;
	std::vector<Observer<Game>*> getObservers() const;

	//SPELLS
	bool buySpell();
	bool heroUseSpell();
	bool enemyBuildingUseSpell(Spell* spell);
	//BUILDINGS
	void checkHeroInBuildingRange();

	//SETTERS FOR SAVINGS
	void setEnemyManager(EnemyManager enemyManager);
	void setEnemyBuildingManager(EnemyBuildingManager enemyBuildingManager);
	void setHeroPos(std::pair<int, int> heroPos);
	void setHero(Hero hero);
	void setField(const Field& field);
	void setHeroSpellFactory(SpellFactory* heroSpellFactory);
	void setEnemySpellFactory(SpellFactory* enemySpellFactory);
	void setEnemyId(char enemyId);
	void setHeroId(char heroId);
	void setStage(size_t value);
	void setObservers(std::vector<Observer<Game>*> observers);
	//GENERATING
	void generateRandomStage();
	std::pair<int, int> findRandomEmptyPlace();
	int generateRandomNumberInRange(int a, int b);
	void toNextStage();
	size_t getStage() const;
	void initSpells();
	
	//OBSERVERS
	// void notifyObservers() override;
};


#endif
