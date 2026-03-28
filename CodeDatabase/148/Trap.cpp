#include "Trap.h"
#include "GameEventSystem.h"
#include "GameEvent.h"

/**
 * @file Trap.cpp
 * @brief Реализация класса ловушки
 */

/**
 * @brief Конструктор ловушки
 * @param posX X-координата
 * @param posY Y-координата
 * @param trapDamage Урон ловушки
 */
Trap::Trap(int posX, int posY, int trapDamage)
    : GameEntity(1, trapDamage, posX, posY), activated(false) {
    // У ловушки 1 HP (символическое), после активации исчезает
}

/**
 * @brief Активировать ловушку
 * @return Урон, который наносит ловушка
 */
int Trap::activate() {
    if (activated) {
        return 0;
    }
    
    activated = true;
    health = 0; // Ловушка "умирает" после активации
    
    // Логируем активацию ловушки
    auto [tx, ty] = getPosition();
    EntityEvent event(EventType::TRAP_ACTIVATED, "Trap", tx, ty,
                      "Trap activated at (" + std::to_string(tx) + "," + std::to_string(ty) + 
                      ") dealing " + std::to_string(damage) + " damage");
    GameEventSystem::getInstance().notifyEvent(event);
    
    return damage;
}


