#include "direct_damage_spell.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h" 
#include <iostream>
#include <limits>
#include <cmath> 

DirectDamageSpell::DirectDamageSpell(int damage, float radius) {
	damage_ = damage;
	radius_ = radius;
}

std::string DirectDamageSpell::getName() const{
	return "Заклинание прямого урона";
}

std::string DirectDamageSpell::getDescription() const {
	return "Наносит " + std::to_string(damage_) + " урона одной цели в радиусе " + std::to_string((int)radius_) + " клеток от Игрока.";
}

SpellCastResult DirectDamageSpell::apply(Player* caster, PlayingField& field) {
	std::cout << "Выберите цель для заклинания '" << getName() << "'." << std::endl;
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

	BasicBattler* target = field.getBattlerAt(target_x, target_y);
	if (!target) {
		std::cout << "В указанной клетке нет цели!" << std::endl;
		return SpellCastResult::MISS;
	}

	if (!caster->isAlly(target)) {
		std::cout << caster->getName() << " атакует бойца " << target->getName() << " заклинанием '" << getName() << "'!" << std::endl;
		target->takeDamage(damage_);

		if (!target->isAlive()) {
			caster->processKillReward();
		}
		return SpellCastResult::SUCCESS;
	}
	else {
		std::cout << "Целью может быть только враг!" << std::endl;
		return SpellCastResult::MISS;
	}
}

SpellType DirectDamageSpell::getType() const {
	return SpellType::DirectDamage;
}
