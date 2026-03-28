#include "DirectDamageSpell.h"
#include "GameField.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int dmg, int r)
	: damage(dmg), radius(r), enhancedRadius(0) {}

bool DirectDamageSpell::cast(GameField& field, int targetX, int targetY) {
    int actualRadius = radius + enhancedRadius;
    bool hit = false;
    
    for (int dx = -actualRadius; dx <= actualRadius; ++dx) {
        for (int dy = -actualRadius; dy <= actualRadius; ++dy) {
            int checkX = targetX + dx;
            int checkY = targetY + dy;
            
			if (std::abs(dx) <= actualRadius && std::abs(dy) <= actualRadius &&
				field.isValidPosition(checkX, checkY)) {
                
				auto& enemies = field.getEnemies();
				for (auto it = enemies.begin(); it != enemies.end(); ++it) {
					if (it->first.x == checkX && it->first.y == checkY) {
						it->second.takeDamage(damage);
						std::cout << "Прямое заклинание нанесло " << damage
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
						std::cout << "Прямое заклинание попало по зданию в позиции ("
							<< checkX << ", " << checkY << ")!" << std::endl;
						hit = true;
					}
				}
            }
        }
    }
    
	if (!hit) {
		std::cout << "Заклинание прямого урона не попало ни в одну цель!" << std::endl;
	}

	enhancedRadius = 0;
	return hit;
}

void DirectDamageSpell::enhance() {
	enhancedRadius++;
}

std::string DirectDamageSpell::getName() const {
	return "Прямой урон";
}

SpellId DirectDamageSpell::getId() const {
	return SpellId::DIRECT_DAMAGE;
}