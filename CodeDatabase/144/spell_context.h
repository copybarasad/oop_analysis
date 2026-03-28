#ifndef SPELL_CONTEXT_H
#define SPELL_CONTEXT_H

#include <vector>
#include <string>

class Player;
class Field;
class EnemyManager;
class BuildingManager;
class Damageable;
class Trap;
class Ally;
class Target;

struct SpellTarget {
	std::string name;
	Damageable* targetObject;
	int positionX;
	int positionY;
};

struct AreaSelection {
	bool cancelled = true;
	int targetX = 0;
	int targetY = 0;
};

class SpellContext {
public:
	virtual ~SpellContext() = default;

	virtual Player& getPlayer() = 0;
	virtual Field& getField() = 0;
	virtual EnemyManager& getEnemyManager() = 0;
	virtual BuildingManager& getBuildingManager() = 0;

	virtual void addTrap(const Trap& trap) = 0;
	virtual void addAlly(const Ally& ally) = 0;
	virtual bool damageEntitiesAtPosition(int x, int y, int damageAmount) = 0;

	virtual bool isCellOccupiedByEnemy(int x, int y) const = 0;
	virtual bool isCellOccupiedByAlly(int x, int y) const = 0;

	virtual void handleTargetDefeat(Damageable& target) = 0;

	// Методы для получения целей (заклинания не должны искать цели сами)
	virtual std::vector<SpellTarget> getTargetsInRange(int range) = 0;
	virtual void displayTargets(const std::vector<SpellTarget>& targets) = 0;
	virtual bool trySelectTarget(const std::vector<SpellTarget>& targets, int& selectedIndex) = 0;
	virtual bool requestDirection(const std::string& prompt, int& directionX, int& directionY) = 0;
	virtual AreaSelection selectArea(int areaSize) = 0;
	
	// Методы для уведомления о событиях заклинаний (сущность только информирует, контекст решает, логировать ли)
	virtual void notifySpellDamage(const std::string& spellName, const std::string& targetName, int damage, int targetHealth, int targetMaxHealth) = 0;
	virtual void notifySpellTargetDefeated(const std::string& targetName) = 0;
	virtual void notifySpellAreaHit(const std::string& spellName, int x, int y, int damage) = 0;
	virtual void notifySpellNoTargets(const std::string& spellName) = 0;
	virtual void notifyAllySummoned(int count, int x, int y) = 0;
	virtual void notifyTrapPlaced(const std::string& spellName, int x, int y) = 0;
};

#endif

