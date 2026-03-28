#pragma once
#include "../Abstractions/PositionEntityContainer.h"
#include "../Entities/ISwitchingDamager.h"

class DamagerContainer : public PositionEntityContainer<IDamager>
{
	std::unordered_map<PositionEntity*, ISwitchingDamager*> _entities;
public:
	bool AddEntity(PositionEntity* entity, ISwitchingDamager* typed);

	bool RemoveEntity(PositionEntity* entity) override;
};