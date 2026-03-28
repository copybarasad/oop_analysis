#ifndef IENTITY_H
#define IENTITY_H

#include <memory>
#include <utility>

enum class EntityType { UNDEFINED, PLAYER, ENEMY, BARRACKS, TOWER, ALLY };

class IEntity {
public:
	virtual ~IEntity() = default;
	
	virtual EntityType getType() const = 0;
	virtual void causeDamage(int damage) = 0;
	virtual std::pair<int, int> getHealth() const = 0;
	virtual bool alive() const = 0;
	virtual std::unique_ptr<IEntity> clone() const = 0;
};

#endif
