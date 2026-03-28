#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

#include "IEntity.h"

struct EntityData {
	EntityType type;
	int index;
	int current_hp;
	int max_hp;
	int damage;
	
	EntityData(EntityType entity_type, int entity_index, int curr_hp, int m_hp, int dmg)
		: type(entity_type), index(entity_index), current_hp(curr_hp), 
		  max_hp(m_hp), damage(dmg) {}
};

#endif
