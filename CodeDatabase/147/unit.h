#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <iostream>
#include "position.h"
#include "direction.h"
#include "cell.h" 

// Unit — базовый класс для всех игровых объектов, имеющих здоровье и способных наносить
// урон. Класс объявлен абстрактным, так как содержит хотя бы одну чисто виртуальную
// функцию (getSymbol), поэтому напрямую экземпляры Unit созданы быть не могут.
//
// Инварианты класса:
// 1. _health >= 0
// 2. _damage >= 0
// 3. После любого вызова public-метода инварианты сохраняются.
class Unit {
public:
	Unit(int health, int damage);
	virtual ~Unit() = default;

	// Копирование и перемещение разрешены по умолчанию 
	Unit(const Unit&) = default;
	Unit(Unit&&) noexcept = default;
	Unit& operator=(const Unit&) = default;
	Unit& operator=(Unit&&) noexcept = default;

	int get_health() const;
	int get_damage() const;
	CellType get_type() const;

	bool isAlive() const;

	// Геттеры и сеттеры для позиции
    Position getPosition() const;
    void setPosition(const Position& newPosition);
    
    // Метод для изменения урона и хп
	void change_health(int value);
	void change_damage(int value);


	// Нанести текущему объекту урон. Если урон больше текущего здоровья,
	// здоровье обнуляется, но не уходит в отрицательные значения.
	void takeDamage(int value);
	void freeze();
	bool isFrozen() const;

protected:
	bool _isFrozen;
	int _health;
	int _damage;
	CellType _type;
	Position _position;  // Каждый юнит знает свою позицию

};

#endif // UNIT_H
