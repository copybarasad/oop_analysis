#include "EnemyTower.h"
#include <cmath>

// Реализация магической башни
// Содержит логику атаки башни, использует конфигурацию
//  для настройки урона и времени перезарядки.

// Конструктор башни - использует конфигурацию для настроек
EnemyTower::EnemyTower(const Position& pos, const TowerConfig& config)
    : Entity(config.health, pos, "EnemyTower", config.reward),
      attackRange(config.attackRange), 
      spellDamage(config.spellDamage),
      spellCooldown(config.spellCooldown), 
      currentSpellCooldown(0), 
      canCastSpell(true) {}

// Обновление состояния башни - уменьшение кд заклинаний
void EnemyTower::update() {
    if (currentSpellCooldown > 0) {
        currentSpellCooldown--;
        if (currentSpellCooldown == 0) {
            canCastSpell = true;  // Заклинание снова доступно
        }
    }
}

// Попытка кастовать заклинание по игроку
bool EnemyTower::tryCastSpell(Player& player, GameField& field) {
    // Проверяем условия для каста: может кастовать, жива, игрок жив
    if (!canCastSpell || !isAlive() || !player.isAlive()) {
        return false;
    }
    
    // Проверяем что игрок в радиусе атаки
    if (!isPlayerInRange(player)) {
        return false;
    }
    
    // Наносим урон игроку
    player.takeDamage(spellDamage);
    canCastSpell = false;  // Башня не может кастовать
    currentSpellCooldown = spellCooldown;  // Устанавливаем кд
    
    (void)field; // Подавляем предупреждение о неиспользуемом параметре
    return true;
}

// Получение урона - базовая реализация
void EnemyTower::takeDamage(int damageAmount) {
    Entity::takeDamage(damageAmount);
}

// Проверка находится ли игрок в радиусе атаки башни
bool EnemyTower::isPlayerInRange(const Player& player) const {
    return distanceTo(player) <= attackRange;
}