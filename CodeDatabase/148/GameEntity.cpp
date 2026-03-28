#include "GameEntity.h"
#include <stdexcept>


/**
 * @file GameEntity.cpp
 * @brief Реализация базового класса игровой сущности
 */


/**
 * @brief Конструктор игровой сущности
 * @param startHealth Начальное здоровье
 * @param startDamage Базовый урон
 * @param startX Начальная X-координата
 * @param startY Начальная Y-координата
 * 
 * @throws std::invalid_argument если здоровье <= 0
 */
GameEntity::GameEntity(int startHealth, int startDamage, int startX, int startY)
    : health(startHealth), damage(startDamage), positionX(startX), positionY(startY) {
    if (health <= 0) {  ///< Валидация: здоровье должно быть положительным
        throw std::invalid_argument("Health must be positive");  
    }
}


/**
 * @brief Переместить сущность на новую позицию
 * @param newX Новая X-координата
 * @param newY Новая Y-координата
 */
void GameEntity::move(int newX, int newY) {
    positionX = newX;  ///< Обновляем X-координату
    positionY = newY;  ///< Обновляем Y-координату
}


/**
 * @brief Получить урон
 * @param amount Количество урона
 * 
 * Здоровье не может быть отрицательным.
 */
void GameEntity::takeDamage(int amount) {
    health -= amount;  ///< Вычитаем урон
    if (health < 0) {  
        health = 0;  
    }
}


/**
 * @brief Атаковать другую сущность
 * @param target Указатель на целевую сущность
 * 
 * Атака наносит урон только живым целям.
 */
void GameEntity::attack(GameEntity* target) {
    if (target && target->isAlive()) {  ///< Проверяем валидность и живое ли состояние
        target->takeDamage(damage);  ///< Наносим урон
    }
}


/**
 * @brief Установить новое значение здоровья
 * @param newHealth Новое значение здоровья
 * 
 * @throws std::invalid_argument если здоровье отрицательное
 */
void GameEntity::setHealth(int newHealth) {
    if (newHealth < 0) {  ///< Валидация: здоровье не может быть отрицательным
        throw std::invalid_argument("Health cannot be negative");  ///< Выбрасываем исключение
    }
    health = newHealth;  
}


/**
 * @brief Установить новую позицию
 * @param x Новая X-координата
 * @param y Новая Y-координата
 * 
 * Метод помечен как noexcept, так как не может выбросить исключение.
 */
void GameEntity::setPosition(int x, int y) noexcept {
    positionX = x;  ///< Обновляем X-координату
    positionY = y;  ///< Обновляем Y-координату
}


