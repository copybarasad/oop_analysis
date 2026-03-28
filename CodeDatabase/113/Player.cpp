#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "GameField.h"
#include "Spell.h"
#include <iostream>


void Player::takeDamage(int amount) {
    m_stats.takeDamage(amount);
    std::cout << "Player took " << amount << " damage! Health: " << m_stats.getHealth() << "/" << m_stats.getMaxHealth() << std::endl;
}

void Player::heal(int amount) {
    int oldHealth = m_stats.getHealth();
    m_stats.heal(amount);
    int healed = m_stats.getHealth() - oldHealth;
    if (healed > 0) {
        std::cout << "Player healed for " << healed << " health! Health: " << m_stats.getHealth() << "/" << m_stats.getMaxHealth() << std::endl;
    }
}

void Player::addScore(int points) {
    m_stats.addScore(points);
    std::cout << "Gained " << points << " score! Total: " << m_stats.getScore() << std::endl;
}

void Player::addMana(int amount) {
    int oldMana = m_stats.getMana();
    m_stats.addMana(amount);
    int gained = m_stats.getMana() - oldMana;
    if (gained > 0) {
        std::cout << "Gained " << gained << " mana! Mana: " << m_stats.getMana() << "/" << m_stats.getMaxMana() << std::endl;
    }
}

bool Player::spendMana(int amount) {
    if (m_stats.spendMana(amount)) {
        std::cout << "Spent " << amount << " mana. Remaining: " << m_stats.getMana() << "/" << m_stats.getMaxMana() << std::endl;
        return true;
    }
    else {
        std::cout << "Not enough mana! Need " << amount << " but have " << m_stats.getMana() << std::endl;
        return false;
    }
}

void Player::attack(std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, const GameField& field) {
    m_combat.attack(enemies, towers, field, m_stats, getPositionX(), getPositionY());
}

void Player::switchAttackType() {
    m_combat.switchAttackType();
    AttackType newType = m_combat.getCurrentAttackType();
    std::cout << "Switched to " << (newType == AttackType::Melee ? "MELEE" : "RANGED") << " attack type. Damage: " << m_combat.getCurrentDamage() << std::endl;
}

bool Player::move(int deltaX, int deltaY, GameField& field) {
    bool result = m_movement.move(deltaX, deltaY, field, *this);

    return result;
}

bool Player::castSpell(int spellIndex, GameField& field) {
    try {
        if (spellIndex < 0 || spellIndex >= static_cast<int>(m_hand.getSpellCount())) {
            std::cout << "Invalid spell index!" << std::endl;
            return false;
        }

        // Получаем заклинание и сразу сохраняем его данные
        Spell* spellPtr = m_hand.getSpell(spellIndex);
        if (!spellPtr) {
            std::cout << "Spell not found!" << std::endl;
            return false;
        }

        // Сохраняем все данные ДО каста
        std::string spellName = spellPtr->getName();
        int manaCost = spellPtr->getManaCost();

        std::cout << "Attempting to cast " << spellName << "..." << std::endl;
        std::cout << "Mana cost: " << manaCost << ", Current mana: " << m_stats.getMana() << std::endl;

        if (m_stats.getMana() < manaCost) {
            std::cout << "Not enough mana to cast " << spellName << "! Need "
                << manaCost << " but have " << m_stats.getMana() << std::endl;
            return false;
        }

        // Кастуем заклинание
        bool castResult = spellPtr->cast(*this, field);

        if (castResult) {
            // Тратим ману
            m_stats.spendMana(manaCost);
            // Убираем заклинание из руки
            m_hand.removeSpell(spellIndex);
            std::cout << "Successfully cast " << spellName << "! Mana spent: " << manaCost << std::endl;
        }
        else {
            std::cout << spellName << " failed to cast!" << std::endl;
        }

        return castResult;
    }
    catch (const std::exception& e) {
        std::cout << "Error during spell casting: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "Unknown error during spell casting!" << std::endl;
        return false;
    }
}

void Player::setEnemiesReference(std::vector<Enemy>& enemies) {
    m_enemies = &enemies;
    std::cout << "Enemies reference set. Total enemies: " << enemies.size() << std::endl;
}

void Player::setTowersReference(std::vector<EnemyTower>& towers) {
    m_towers = &towers;
    std::cout << "Towers reference set. Total towers: " << towers.size() << std::endl;
}

std::vector<Enemy>& Player::getEnemies() {
    if (!m_enemies) {
        throw std::runtime_error("Enemies reference not set");
    }
    return *m_enemies;
}

const std::vector<Enemy>& Player::getEnemies() const {
    if (!m_enemies) {
        throw std::runtime_error("Enemies reference not set");
    }
    return *m_enemies;
}

std::vector<EnemyTower>& Player::getTowers() {
    if (!m_towers) {
        throw std::runtime_error("Towers reference not set");
    }
    return *m_towers;
}

const std::vector<EnemyTower>& Player::getTowers() const {
    if (!m_towers) {
        throw std::runtime_error("Towers reference not set");
    }
    return *m_towers;
}

Player::DamageInfo Player::getDamageInfo() const {
    DamageInfo info;
    info.currentDamage = getDamage();
    info.meleeBase = m_combat.getMeleeDamage();
    info.rangedBase = m_combat.getRangedDamage();
    return info;
}