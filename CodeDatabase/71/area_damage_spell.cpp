#include "area_damage_spell.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include <iostream>
#include <limits>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int damage, float radius) {
	damage_ = damage;
	radius_ = radius;
}

std::string AreaDamageSpell::getName() const {
	return "Заклинание урона по площади";
}

std::string AreaDamageSpell::getDescription() const {
	return "Наносит " + std::to_string(damage_) + " урона в области 2x2 в радиусе " + std::to_string((int)radius_) + " клеток от Игрока.";
}

SpellCastResult AreaDamageSpell::apply(Player* caster, PlayingField& field) {
	std::cout << "Выберите левую верхнюю клетку для области 2x2." << std::endl;
	std::cout << "Введите координаты в формате \"x y\" : ";
	int target_x, target_y;
	std::cin >> target_x >> target_y;

	if (std::cin.fail()) {
		std::cout << "Некорректный ввод координат. Попробуйте еще раз." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return SpellCastResult::INVALID_TARGET;
	}

	if (target_x < 0 || target_x >= field.getWidth() || target_y < 0 || target_y >= field.getHeight()) {
		std::cout << "Ошибка: Координаты (" << target_x << ", " << target_y << ") находятся вне игрового поля!" << std::endl;
		return SpellCastResult::MISS;
	}

	auto caster_coords = field.getBattlerCoords(caster);
	double distance = std::sqrt(std::pow(caster_coords.first - target_x, 2) + std::pow(caster_coords.second - target_y, 2));

	if (distance > radius_) {
		std::cout << "Цель находится слишком далеко! (Расстояние: " << distance << ", Максимальное расстояние: " << radius_ << ")" << std::endl;
		return SpellCastResult::MISS;
	}

	std::cout << caster->getName() << " вызывает '" << getName() << "' в области ("
		<< target_x << ", " << target_y << ")..." << std::endl;

	int enemies_hit = 0;
	for (int y = target_y; y < target_y + 2; ++y) {
		for (int x = target_x; x < target_x + 2; ++x) {

			if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
				BasicBattler* target = field.getBattlerAt(x, y);
				if (target && !caster->isAlly(target)) {
					std::cout << target->getName() << " получает " << damage_ << " урона!" << std::endl;
					target->takeDamage(damage_);
					enemies_hit++;

					if (!target->isAlive()) {
						caster->processKillReward();
					}
				}
			}
		}
	}
	if (enemies_hit == 0) {
		std::cout << "Заклинание никого не задело." << std::endl;
	}
	return SpellCastResult::SUCCESS;
}

SpellType AreaDamageSpell::getType() const {
	return SpellType::AreaDamage;
}