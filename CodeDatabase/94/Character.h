#ifndef CHARACTER_H
#define CHARACTER_H

// Базовый класс для всех персонажей в игре (игрок, враги)
// Определяет основные атрибуты и поведение персонажей
class Character {
private:
    int health;        // Текущее здоровье
    int maxHealth;     // Максимальное здоровье
    int damage;        // Наносимый урон

protected:
    // Защищенные методы для установки здоровья с проверкой корректности
    void setHealth(int newHealth);
    void setMaxHealth(int newMaxHealth);

public:
    // Конструктор с начальным здоровьем и уроном
    Character(int initialHealth, int characterDamage);
    virtual ~Character() = default;
    
    // Правило пяти (конструкторы и операторы копирования/перемещения)
    Character(const Character& other) = default;
    Character(Character&& other) = default;
    Character& operator=(const Character& other) = default;
    Character& operator=(Character&& other) = default;
    
    // Геттеры для основных атрибутов
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    
    // Публичные методы для установки здоровья (добавлены для Game класса)
    void setHealthPublic(int newHealth) { setHealth(newHealth); }
    void setMaxHealthPublic(int newMaxHealth) { setMaxHealth(newMaxHealth); }
    
    // Методы для изменения состояния
    void takeDamage(int damageAmount);  // Получение урона
    void heal(int healAmount);          // Лечение
    bool isAlive() const;               // Проверка жив ли персонаж
    
    // Виртуальные методы для полиморфного поведения
    virtual Character* clone() const = 0;  // Создание копии (прототип)
    virtual void performAction() = 0;      // Выполнение действия (для ИИ врагов)
};

#endif