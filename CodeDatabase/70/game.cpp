#include "game.h"

Game::Game() : enemyId(0), heroId(0)
{
	Field field;
	Hero hero;
	EnemyManager enemyManager;
	EnemyBuildingManager enemyBuildingManager;
	std::pair<int, int> heroPos;
	initSpells();
}
Game::Game(char enemyId, char heroId) : enemyId(enemyId), heroId(heroId), stage(1)
{
	Field field;
	Hero hero;
	EnemyManager enemyManager;
	EnemyBuildingManager enemyBuildingManager;
	std::pair<int, int> heroPos;
	initSpells();
}

void Game::initSpells()
{
	heroSpellFactory = new SpellFactory();
    enemySpellFactory = new SpellFactory();
	heroSpellFactory->initSpellsForHero();
	enemySpellFactory->initSpellsForEnemies();
}

int Game::generateRandomNumberInRange(int a, int b)
{
	srand(time(NULL));
    int number = (rand() % (b - a + 1)) + a;
    return number;
}

std::pair<int, int> Game::findRandomEmptyPlace()
{
	int x = generateRandomNumberInRange(0, field.getWidth()-1);
	int y = generateRandomNumberInRange(0, field.getHeight()-1);
	while (enemyManager.isEnemyInPos(x,y) or enemyBuildingManager.isBuildingInPos(x,y))
	{
		x = generateRandomNumberInRange(0, field.getWidth()-1);
		y = generateRandomNumberInRange(0, field.getHeight()-1);
		//std::cout << x << " " << y << std::endl;
	}
	std::pair<int, int> pair = {x, y};
	return pair;
}

void Game::generateRandomStage()
{
	int width = 15 + stage;
	int height = 10 + stage;
	field = Field(width, height);
	//GENERATE ENEMEIES
	std::pair<int, int> cords;
	for(size_t i = 0; i < stage; ++i)
	{
		cords = findRandomEmptyPlace();
		enemyManager.addEnemy(100, stage*4, stage*3, cords.first, cords.second);
	}
	//GENERATE TOWERS
	for(size_t i = 0; i < stage/5; ++i)
	{
		cords = findRandomEmptyPlace();
		enemyBuildingManager.addBuilding(2, stage*30, 200, cords.first, cords.second);
	}
	//PLACING HERO
	cords = findRandomEmptyPlace();
	heroPos.first = cords.first;
	heroPos.second = cords.second;
}

void Game::gameInit()
{
	hero = Hero(100, 200, 15, 70, 5);
	hero.addSpell(heroSpellFactory->createRandomSpell());
	heroSpellFactory = new SpellFactory();
    enemySpellFactory = new SpellFactory();
    heroSpellFactory->initSpellsForHero();
	enemySpellFactory->initSpellsForEnemies();
	generateRandomStage();
	notifyObservers();
}

bool Game::gameConditions()
{
	return (hero.getHealth() > 0) and (stage<=10);
}

bool Game::stageConditions()
{
	return (hero.getHealth() > 0) and (!enemyManager.empty());
}

void Game::toNextStage()
{
	stage+=1;
	generateRandomStage();
	hero.setHealth(100);
	hero.getHand().deleteRandomHalf();
	notifyObservers();
}

void Game::enemyTurn()
{
	std::map<std::pair<int, int>, Enemy> enemies = enemyManager.getEnemies();
	int x;
	int y;
	int new_x;
	int new_y;
	for (const auto& [enemyPos, enemy] : enemies)
	{
		x = enemyPos.first;
		y = enemyPos.second;
		new_x = x;
		new_y = y;
		if (x < heroPos.first)
		{
			new_x = x+1;
		} else if (x > heroPos.first)
		{
			new_x = x-1;
		}
		if (new_x == x)
		{
			if (y < heroPos.second)
			{
				new_y = y+1;
			} else if (y > heroPos.second)
			{
				new_y = y-1;
			}	
		}
		if (new_x == heroPos.first and new_y == heroPos.second)
		{
			int damage = enemyManager.getEnemyDamage(x, y);
			hero.receiveDamage(damage);
		} else if (field.isCellPassable(new_x, new_y) and !enemyManager.isEnemyInPos(new_x, new_y)
			and !enemyBuildingManager.isBuildingInPos(new_x, new_y))
		{
			enemyManager.setEnemyPos(x, y, new_x, new_y);
			if(field.isCellTrapped(new_x, new_y))
			{
				attackEnemy(new_x, new_y, field.useTrap(new_x,new_y));
			}
		}
	}
	notifyObservers();
}

void Game::showHeroPos()
{
	std::cout << std::format("Your position: {} {}.", heroPos.first, heroPos.second) << std::endl;
}

void Game::swapHeroRange()
{
	hero.swapRanges();
	notifyObservers();
}

bool Game::setHeroPos(int x, int y)
{
	bool flag = false;
	if (x<0 or x>(field.getWidth()-1) or y<0 or y>(field.getHeight()-1) or enemyManager.isEnemyInPos(x, y)
	 	or field.isCellPassable(x, y) == false or enemyBuildingManager.isBuildingInPos(x,y))
	{
		std::cout << "You can't get there."<< std::endl;
		return flag;
	}
	heroPos.first = x;
	heroPos.second = y;
	checkHeroInBuildingRange();
	flag = true;

	return flag;
	notifyObservers();
}

bool Game::heroAttacks(int x, int y)
{
	if(!isCellInRange(heroPos.first, heroPos.second, x, y, hero.getRange()))
	{
		std::cout << "Point's not in hero scope." << std::endl;
		return false;
	}
	if(enemyManager.isEnemyInPos(x, y))
	{
		return attackEnemy(x, y, hero.getDamage());
	} else if(enemyBuildingManager.isBuildingInPos(x, y))
	{
		return attackBuilding(x, y, hero.getDamage());
	} else
	{
		std::cout << "No enemies there." << std::endl;
		return false;
	}
}

bool Game::attackBuilding(int x, int y, int damage)
{
	int reward = enemyBuildingManager.attackBuilding(x, y, damage);
	if (reward != 0)
	{
		hero.receivePoints(reward);
		std::cout << std::format("Building destroyed! +{} points!", reward) << std::endl;
	}
	notifyObservers();
	return true;
}

bool Game::attackEnemy(int x, int y, int damage)
{
	int reward = enemyManager.attackEnemy(x, y, damage);
	if (reward != 0)
	{
		hero.receivePoints(reward);
		std::cout << std::format("Enemy killed! +{} points!", reward) << std::endl;
	}
	notifyObservers();
	return true;
}

bool Game::isCellInRange(int x, int y, int cell_x, int cell_y, int range)
{
	if (std::abs(x - cell_x) > range or std::abs(y - cell_y) > range)
	{
		return false;
	}
	return true;
}

// SPELLS
bool Game::buySpell()
{
	std::cout << "Available spells to buy:" << std::endl;
	heroSpellFactory->showSpells();
	int id;
	std::cout << "Choose spell to buy (-1 to exit): ";
	InputGetter::getInstance().getNumberInRange(&id, 0, heroSpellFactory->getSize());
    if(id == -1)
    {
    	return false;
    }
    Spell* spell = heroSpellFactory->createSpell(id);
    if(hero.getPoints() >= spell->getPrice())
    {
    	hero.spendPoints(spell->getPrice());
    	if (!hero.addSpell(spell))
    	{
    		return false;
    	}
    	std::cout << "You've bought spell! " << "-" << spell->getPrice() << " points!" << std::endl;
    	notifyObservers();
    	return true;
    }
    std::cout << "Can't buy spell!" << std::endl;
    return false;
}

////

bool Game::heroUseSpell()
{
	hero.showHand();
	if (hero.getHandSize() < 1) return false;
	int id;
	bool flag;
	std::cout << "Choose spell to use: ";
	InputGetter::getInstance().getNumberInRange(&id, 0, hero.getHandSize());
  	Spell* spell = hero.getSpell(id);
  	const int HERO_ID = 0;
    if(!spell->useSpell(*this, HERO_ID))
    {
    	return false;
    } else
    {
    	hero.removeSpell(id);
    	notifyObservers();
    	return true;
    }
}

//BUILDINGS
void Game::checkHeroInBuildingRange()
{
	const int TOWER_ID = 1;
	const auto& buildings = enemyBuildingManager.getBuildings();
	for (const auto& [buildingPos, building] : buildings)
	{
		int range = building.getRange();
		if(isCellInRange(buildingPos.first, buildingPos.second, heroPos.first, heroPos.second, range))
		{
		    srand(time(NULL));
		    int attackChance = rand() % 2;
		    if (attackChance == 0)
		    {
		    	Spell* spell = enemySpellFactory->createRandomSpell();
		    	enemyBuildingManager.addBuildingSpell(buildingPos.first, buildingPos.second, spell);
		    	spell = enemyBuildingManager.getBuildingSpell(buildingPos.first, buildingPos.second);
		    	spell->useSpell(*this, TOWER_ID);
		    }
		}
	}
	notifyObservers();
}
// SPELLS FIXING

const EnemyManager& Game::getEnemyManager() const {return enemyManager;}
const EnemyBuildingManager& Game::getEnemyBuildingManager() const {return enemyBuildingManager;}
const std::pair<int, int>& Game::getHeroPos() const {return heroPos;}
const Hero& Game::getHero() const {return hero;}
const Field& Game::getField() const {return field;}
Hero& Game::getHero(){return hero;}
Field& Game::getField(){return field;}
SpellFactory* Game::getHeroSpellFactory() const {return heroSpellFactory;}
SpellFactory* Game::getEnemySpellFactory() const {return enemySpellFactory;}
char Game::getEnemyId() const {return enemyId;}
char Game::getHeroId() const {return heroId;}
size_t Game::getStage() const {return stage;}
std::vector<Observer<Game>*> Game::getObservers() const {return observers;}
//SETTERS

void Game::setEnemyManager(EnemyManager enemyManager){this->enemyManager = enemyManager;}
void Game::setEnemyBuildingManager(EnemyBuildingManager enemyBuildingManager){this->enemyBuildingManager = enemyBuildingManager;}
void Game::setHeroPos(std::pair<int, int> heroPos){this->heroPos = heroPos;}
void Game::setHero(Hero hero){this->hero = hero;}
void Game::setField(const Field& field){this->field = field;}
void Game::setHeroSpellFactory(SpellFactory* heroSpellFactory){this->heroSpellFactory = heroSpellFactory;}
void Game::setEnemySpellFactory(SpellFactory* enemySpellFactory){this->enemySpellFactory = enemySpellFactory;}
void Game::setEnemyId(char enemyId){this->enemyId = enemyId;}
void Game::setHeroId(char heroId){this->heroId = heroId;}
void Game::setStage(size_t value){stage = value;}
void Game::setObservers(std::vector<Observer<Game>*> observers){this->observers = observers;}

Game::~Game()
{
	delete heroSpellFactory;
	delete enemySpellFactory;
}
// OBSERVER DEPENDENCIES

// void Game::notifyObservers() override {
//     for (Observer<Game>* observer : observers) {
//         observer->update(*this);
//     }
// }