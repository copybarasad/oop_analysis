#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "../../entites/Player.h"
#include "../../entites/Spells/DirDamageSpell.h"
#include "../../entites/Spells/TrapSpell.h"
#include "../../entites/Spells/AreaDmgSpell.h"
#include "../../entites/Spells/EnhacementSpell.h"
#include "../../entites/Spells/SummSpell.h"


#include "../../map/Field.h"

class World;

class PlayerManager {
public:
	void SpawnPlayer(Field&, std::shared_ptr<Player>);

	bool ChangeAttackMode(std::shared_ptr<Player>);
	bool Attack(std::shared_ptr<Player>, int, int, Field&, World&);
	bool Move(std::shared_ptr<Player>, MoveType, Field&);
	bool BuySpell(std::shared_ptr<Player>);
	bool CastSpell(std::shared_ptr<Player>, size_t, int, int, Field&, World&);

	void ManagePlayerTurn(std::shared_ptr<Player>, Field&, World&);

private:
	std::shared_ptr<Entity> findTargetAt(int, int, Field&, World&);
	bool isInAttackRange(std::shared_ptr<Player>, int, int, Field&);
};

#endif // PLAYERMANAGER_H