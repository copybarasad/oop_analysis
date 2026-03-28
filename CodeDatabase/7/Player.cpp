#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "Node.h"
#include <iostream>
#include <cmath>
#include <limits>
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"

// Конструктор
Player::Player(std::string name, Field* field, int startX, int startY, int handSize) 
    : Character(name, field, startX, startY, 150), 
      points(100), 
      combatType(0), 
      combatPower(25), 
      radius(1), 
      hand(handSize), 
      damageBonus(0) {}

// Движение
bool Player::move(int dx, int dy) {
    int newX = x + dx;
    int newY = y + dy;
    
    if (!field->areCoordinatesValid(newX, newY)) {
        std::cout << "Нельзя выйти за пределы поля." << std::endl;
        return false;
    }
    
    Node& destinationNode = field->getNode(newX, newY);
    if (!destinationNode.isPassable()) {
        std::cout << "Путь преграждает препятствие." << std::endl;
        return false;
    }
    
    if (destinationNode.getType() == Type::ENEMY) {
        std::cout << "Нельзя наступить на клетку с врагом." << std::endl;
        return false;
    }
    
    if (destinationNode.getType() == Type::TRAP) {
        int trapDmg = destinationNode.triggerTrap();
        std::cout << "Вы попали в ловушку и получили " << trapDmg << " урона!" << std::endl;
        this->takeDamage(trapDmg);
    }
    
    // Обновляем позицию на поле
    field->getNode(x, y).setType(Type::EMPTY);
    setPosition(newX, newY);
    field->getNode(x, y).setType(Type::PLAYER);
    
    return true;
}

// Смена режима боя
void Player::switchCombatMode() {
    if (combatType == 0) {
        combatType = 1;
        combatPower = 10 + damageBonus;
        radius = 4;
        std::cout << name << " переключился на ДАЛЬНИЙ бой." << std::endl;
    } else {
        combatType = 0;
        combatPower = 25 + damageBonus;
        radius = 1;
        std::cout << name << " переключился на БЛИЖНИЙ бой." << std::endl;
    }
}

// Атака
bool Player::attack(Enemy& target) {
    int distance = std::max(std::abs(x - target.getX()), std::abs(y - target.getY()));
    if (distance <= radius) {
        std::cout << name << " атакует " << target.getName() << "!" << std::endl;
        target.takeDamage(combatPower);
        addPoints(10);
        return true;
    } else {
        return false;
    }
}

// Геттеры и сеттеры очков
int Player::getPoints() const { return points; }
void Player::addPoints(int amount) { points += amount; }

// --- ВОТ ЭТИХ МЕТОДОВ НЕ ХВАТАЛО ---

// Обычный геттер руки
Hand& Player::getHand() { return hand; }

// Константный геттер руки (для рендереров)
const Hand& Player::getHand() const { return hand; }

// Установка поля (для загрузки игры)
void Player::setField(Field* newField) {
    this->field = newField;
}

// Восстановление здоровья (для перехода уровня)
void Player::restoreHealth() {
    lives = maxLives;
}

// ------------------------------------

int Player::getCombatPower() const { return combatPower; }

void Player::resetPosition(int newX, int newY) {
    setPosition(newX, newY);
}

// Магия
void Player::buyRandomSpell() {
    if (points >= 20) {
        points -= 20;
        std::cout << "Вы потратили 20 очков на получение случайного заклинания." << std::endl;
        addRandomSpell();
    } else {
        std::cout << "Недостаточно очков для покупки заклинания (нужно 20)." << std::endl;
    }
}

void Player::addRandomSpell() {
    int spellType = rand() % 3;
    std::unique_ptr<Spell> newSpell;
    if (spellType == 0) {
        newSpell = std::make_unique<DirectDamageSpell>("Огненный шар", 15, 5, 30);
    } else if (spellType == 1) {
        newSpell = std::make_unique<AreaDamageSpell>("Цепная молния", 25, 4, 15);
    } else {
        newSpell = std::make_unique<TrapSpell>("Морозная ловушка", 10, 3, 20);
    }
    hand.addSpell(std::move(newSpell));
}

// Сохранение и загрузка
void Player::save(std::ostream& os) const {
    os << name << std::endl;
    os << lives << " " << maxLives << " " << points << std::endl;
    os << x << " " << y << std::endl;
    os << combatType << " " << combatPower << " " << radius << " " << damageBonus << std::endl;
    hand.save(os);
}

void Player::load(std::istream& is, Field* newField) {
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(is, name);
    is >> lives >> maxLives >> points;
    is >> x >> y;
    is >> combatType >> combatPower >> radius >> damageBonus;
    this->field = newField;
    hand.load(is);
}

// Прокачка
void Player::upgradeMaxHealth(int amount) {
    maxLives += amount;
    lives = maxLives; // Лечим при прокачке
    std::cout << "Максимальное здоровье увеличено до " << maxLives << "!" << std::endl;
}

void Player::upgradeDamage(int amount) {
    damageBonus += amount;
    if (combatType == 0) {
        combatPower = 25 + damageBonus;
    } else {
        combatPower = 10 + damageBonus;
    }
    std::cout << "Бонусный урон увеличен до " << damageBonus << "!" << std::endl;
}