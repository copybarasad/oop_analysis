#include "AreaDamageSpell.h"
#include "GameField.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int dmg, int size)
	: damage(dmg), areaSize(size), enhancedSize(0) {}

bool AreaDamageSpell::cast(GameField& field, int targetX, int targetY) {
    int actualSize = areaSize + enhancedSize;
    bool hit = false;
    
	for (int dx = -actualSize; dx <= actualSize; ++dx) {
		for (int dy = -actualSize; dy <= actualSize; ++dy) {
			const int checkX = targetX + dx;
			const int checkY = targetY + dy;

			if (field.isValidPosition(checkX, checkY)) {
				auto& enemies = field.getEnemies();
				for (auto it = enemies.begin(); it != enemies.end(); ++it) {
					if (it->first.x == checkX && it->first.y == checkY) {
						it->second.takeDamage(damage);
						std::cout << "Заклинание площади нанесло " << damage
							<< " урона врагу в позиции (" << checkX << ", " << checkY << ")!" << std::endl;
						hit = true;

						if (!it->second.isAlive()) {
							field.getPlayer().addScore(10);
							field.getGrid()[checkY][checkX].setEnemyPresence(false);
							enemies.erase(it);
							std::cout << "Враг уничтожен!" << std::endl;
							break;
						}
					}
				}

				auto& buildings = field.getBuildings();
				for (auto it = buildings.begin(); it != buildings.end(); ++it) {
					if (it->first.x == checkX && it->first.y == checkY) {
						std::cout << "Заклинание площади попало по зданию в позиции ("
							<< checkX << ", " << checkY << ")!" << std::endl;
						hit = true;
					}
				}
			}
		}
	}
    
	if (!hit) {
		std::cout << "Заклинание урона по площади применено, но не попало ни в одну цель!" << std::endl;
	} else {
		std::cout << "Заклинание урона по площади нанесло урон по области "
			<< (actualSize * 2 + 1) << "x" << (actualSize * 2 + 1) << "!" << std::endl;
	}

	enhancedSize = 0;
	return true;
}

void AreaDamageSpell::enhance() {
	enhancedSize++;
}

std::string AreaDamageSpell::getName() const {
	return "Урон по площади";
}

SpellId AreaDamageSpell::getId() const {
	return SpellId::AREA_DAMAGE;
}