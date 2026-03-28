#include "TrapManager.h"
#include "GameField.h"
#include "EntityManager.h"
#include "FieldCell.h"
#include "Trap.h"
#include <iostream>

TrapManager::TrapManager(GameField& game_field, EntityManager& em)
	: field(game_field), entity_manager(em) {}

void TrapManager::checkTraps(int entity_index) {
	if (!entity_manager.hasEntity(entity_index)) {
		return;
	}
	
	FieldCell& cell = field.getCell(entity_index);
	
	if (cell.hasTrap()) {
		Trap* trap = cell.getTrap();
		
		if (trap && trap->isActive()) {
			int trap_damage = trap->getDamage();
			entity_manager.causeDamageToEntity(entity_index, trap_damage);
			
			std::cout << "⚠ ЛОВУШКА! Сущность на позиции получила " << trap_damage << " урона!\n";
			
			trap->trigger();
			cell.removeTrap();
		}
	}
}
