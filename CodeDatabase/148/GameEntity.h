#ifndef GAMEENTITY_H
#define GAMEENTITY_H


#include <utility>
#include <string>


/**
 * @file GameEntity.h
 * @brief Базовый класс для всех игровых сущностей
 * 
 * Определяет общий интерфейс для игровых объектов: игрока, врагов, союзников и башен.
 * Содержит основную функциональность для движения, урона и атак.
 */


/**
 * @class GameEntity
 * @brief Базовая сущность игрового мира
 * 
 * Представляет любой объект с позицией, здоровьем и уроном.
 * Может атаковать другие сущности и получать урон.
 */
class GameEntity {
protected:
    int health;     ///< Текущее здоровье сущности
    int damage;     ///< Базовый урон при атаке
    int positionX;  ///< X-координата на поле
    int positionY;  ///< Y-координата на поле
    
public:
    /**
     * @brief Конструктор сущности
     * @param startHealth Начальное здоровье
     * @param startDamage Базовый урон
     * @param startX Начальная X-координата
     * @param startY Начальная Y-координата
     */
    GameEntity(int startHealth, int startDamage, int startX, int startY);
    
    /// Виртуальный деструктор для безопасного удаления через указатель
    virtual ~GameEntity() = default;
    
    /**
     * @brief Переместить сущность на новую позицию
     * @param newX Новая X-координата
     * @param newY Новая Y-координата
     */
    virtual void move(int newX, int newY);
    
    /**
     * @brief Получить урон от сущности
     * @param amount Количество урона для получения
     */
    virtual void takeDamage(int amount);
    
    /**
     * @brief Атаковать другую сущность
     * @param target Указатель на целевую сущность
     * 
     * Наносит урон цели используя методы getDamage() и takeDamage().
     */
    void attack(GameEntity* target);
    
    /**
     * @brief Получить текущее здоровье
     * @return Значение здоровья
     */
    int getHealth() const noexcept { 
        return health; 
    }
    
    /**
     * @brief Получить урон для атаки
     * @return Значение урона
     * 
     * Virtual метод для поддержки переопределения
     * (например, Player может зависеть от режима атаки).
     */
    virtual int getDamage() const noexcept { 
        return damage; 
    }
    
    /**
     * @brief Получить текущую позицию сущности
     * @return Пара (X, Y) координат
     */
    std::pair<int, int> getPosition() const noexcept { 
        return {positionX, positionY}; 
    }
    
    /**
     * @brief Получить X-координату
     * @return X-позиция
     */
    int getPositionX() const noexcept { 
        return positionX; 
    }
    
    /**
     * @brief Получить Y-координату
     * @return Y-позиция
     */
    int getPositionY() const noexcept { 
        return positionY; 
    }
    
    /**
     * @brief Установить новое значение здоровья
     * @param newHealth Новое здоровье
     */
    void setHealth(int newHealth);
    
    /**
     * @brief Установить новую позицию
     * @param x Новая X-координата
     * @param y Новая Y-координата
     */
    void setPosition(int x, int y) noexcept;
    
    /**
     * @brief Проверить, жива ли сущность
     * @return true если здоровье > 0, false иначе
     */
    bool isAlive() const noexcept { 
        return health > 0; 
    }
    
    /**
     * @brief Получить тип сущности
     * @return Строка с типом (чисто виртуальный метод)
     * 
     * Каждый наследник должен переопределить этот метод.
     */
    virtual std::string getType() const = 0;
};


#endif // GAMEENTITY_H


