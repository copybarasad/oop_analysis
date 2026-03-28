#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "Barracks.h"
#include "CowboyTower.h"
#include "Saloon.h"
#include "HandSpell.h"
#include "Constants.h"
#include <iostream>


using namespace GameConstants;

Player::Player(std::string playerName, int startX, int startY)
    : Character(playerName, DEFAULT_PLAYER_HEALTH, DEFAULT_PLAYER_DAMAGE, startX, startY), 
      score(0), coins(0), level(BEGIN_LEVEL_PLAYER), xp(0),
      currentAttackType(AttackType::MELEE),
      meleeDamage(DEFAULT_MELEE_DAMAGE),
      rangedDamage(DEFAULT_RANGED_DAMAGE),
      attackRange(DEFAULT_ATTACK_RANGE) { 
    damage = meleeDamage;
    handSpell = std::make_unique<HandSpell>(MAX_SIZE_HANDSPELL);
    }

void Player::take_damage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << "Игрок " << name << " погиб!" << std::endl;
    } else {
        std::cout << name << " получает " << amount << " урона. Здоровье: " << health << "/" << max_health << std::endl;
    }
}   

bool Player::move(char direction, GameField& field) {
    
    int newX = position.x;
    int newY = position.y;
    
    if(freeze_time > 0){
        freeze_time--;
        std::cout << "Игрок находится в SALOON вы не можете покинуть его ровно 1 ход" << std::endl;
        return false;
    }
    
    switch(direction) {
        case 'w': case 'W': newY--; break;
        case 's': case 'S': newY++; break;
        case 'a': case 'A': newX--; break;
        case 'd': case 'D': newX++; break;
        default:
            std::cout << "Неверное направление!" << std::endl;
            return false; 
    }

    if (field.canMoveTo(newX, newY) and field.getCell(newX, newY).getType() != TRAP) {
        
        if(freeze_time == 0 && inSaloon) {
            field.setCellType(position.x, position.y, SALOON);
            inSaloon = false;
            std::cout << "Игрок покинул SALOON!" << std::endl;
        } else {
            field.setCellType(position.x, position.y, EMPTY);
        }

        position.x = newX;
        position.y = newY;
        
        if(field.getCell(newX, newY).getType() == SALOON) {
            Saloon& saloon = field.findSaloon(newX, newY);
            freeze_time = saloon.getTime_freeze();
            inSaloon = true;
            heal(HEAL_IN_SALOON);
            std::cout << "Игрок вошел в SALOON! Отдохните и восстановите здоровье! Пропуск хода: " << freeze_time << std::endl;
        }
        
        field.setCellType(newX, newY, PLAYER);
        
        std::cout << "Игрок переместился в (" << newX << ", " << newY << ")" << std::endl;
        return true;
    } else {
        std::cout << "Невозможно переместиться в эту клетку!" << std::endl;
        return false;
    }
}

void Player::attack(char direction, GameField& field) {
    int targetX = position.x;
    int targetY = position.y;
    int range = (currentAttackType == AttackType::RANGED) ? attackRange : 1;
    
    std::cout << "АТАКА в направлении " << direction;
    std::cout << " (" << getAttackTypeString() << ", урон: " << damage;
    std::cout << ", дальность: " << range << ")" << std::endl;
    
    bool targetHit = false;
    
    switch (direction) {
        case 'w': case 'W': 
            for (int i = 1; i <= range && !targetHit; i++) {
                targetHit = checkAndAttack(targetX, targetY - i, field);
            }
            break;
        case 's': case 'S': 
            for (int i = 1; i <= range && !targetHit; i++) {
                targetHit = checkAndAttack(targetX, targetY + i, field);
            }
            break;
        case 'a': case 'A': 
            for (int i = 1; i <= range && !targetHit; i++) {
                targetHit = checkAndAttack(targetX - i, targetY, field);
            }
            break;
        case 'd': case 'D': 
            for (int i = 1; i <= range && !targetHit; i++) {
                targetHit = checkAndAttack(targetX + i, targetY, field);
            }
            break;
        default:
            std::cout << "Неверное направление атаки!" << std::endl;
            return;
    }
    if (freeze_time > 0) freeze_time--;
    
    if (!targetHit) {
        std::cout << "Атака в пустоту!" << std::endl;
    }
}

bool Player::checkAndAttack(int x, int y, GameField& field) {
    
    if (field.isValidPosition(x, y)) {
        Type_cell cell = field.getCell(x, y).getType();

        if(cell == ENEMY){
            Enemy& enemy = field.findEnemy(x, y);
            damageEnemy(enemy, damage);
            return true;
        } else if (cell == BARRACK) {
            Barracks& barrack = field.findBarracks(x, y);
            damageBaracks(barrack, damage);
            return true;
        } else if(cell == COWBOYTOWER){
            CowboyTower& tower = field.findCowboyTower(x, y);
            damageCowboyTower(tower, damage);
            return true;
        }
    }else{
        return false;
    }
}

void Player::damageEnemy(Enemy& enemy, int inpDamage) {
    enemy.take_damage(inpDamage);
    std::cout << "Попадание по " << enemy.getName() << "! Нанесено " << inpDamage << " урона!" << std::endl;
    if (!enemy.isAlive()) {
        std::cout << "Враг " << enemy.getName() << " побежден!" << std::endl;
        takeAwards(ENEMY);
    }
}

void Player::damageBaracks(Barracks& barrack, int inpDamage) {
    barrack.takeDamage(inpDamage);
    std::cout << "Попадание по BARRACK! Нанесено " << inpDamage << " урона!" << std::endl;
    if (!barrack.IsNotDestroyed()) {
        std::cout << "BARRACK разрушен!" << std::endl;
        takeAwards(BARRACK);
    }
}

void Player::damageCowboyTower(CowboyTower& tower, int inpDamage) {
    tower.takeDamage(inpDamage);
    std::cout << "Попадание по COWBOYTOWER! Нанесено " << inpDamage << " урона!" << std::endl;
    if (!tower.IsNotDestroyed()) {
        std::cout << "COWBOYTOWER разрушен!" << std::endl;
        takeAwards(COWBOYTOWER);
    }
}

void Player::takeAwards(Type_cell type){
    switch(type){
        case ENEMY:
            std::cout << "Получено: " << XP_PER_KILL << " опыта, "<< COINS_PER_KILL << " монет " << SCORE_PER_KILL << " очков счета" << std::endl;
            add_xp(XP_PER_KILL);
            add_coins(COINS_PER_KILL);
            add_score(SCORE_PER_KILL);
            break;
        case BARRACK:
            std::cout << "Получено: " << XP_PER_BARRACKS_DESTROY << " опыта, "<< COINS_PER_BARRACKS_DESTROY << " монет " << SCORE_PER_BARRACKS_DESTROY << " очков счета" << std::endl;
            add_xp(XP_PER_BARRACKS_DESTROY);
            add_coins(COINS_PER_BARRACKS_DESTROY);
            add_score(SCORE_PER_BARRACKS_DESTROY);
            break;
        case COWBOYTOWER:
            std::cout << "Получено: " << XP_PER_TOWER_DESTROY << " опыта, "<< COINS_PER_TOWER_DESTROY << " монет " << SCORE_PER_TOWER_DESTROY << " очков счета" << std::endl;
            add_xp(XP_PER_TOWER_DESTROY);
            add_coins(COINS_PER_TOWER_DESTROY);
            add_score(SCORE_PER_TOWER_DESTROY);
            break;
        default:
            break;
        
    }
}

bool Player::useSpell(SpellID spellID, int x, int y, GameField& field) {
    if (handSpell) {
        return handSpell->useSpell(spellID, x, y, field, *this);
    }
    return false;
}

bool Player::addSpell(SpellID spellID, int count) {
    if (handSpell) {
        return handSpell->addSpell(spellID, count);
    }
}


void Player::removeRandomSpells(int count) {
    if (handSpell) {
        int totalBefore = handSpell->getCntTotalSpells();
        if (totalBefore <= 1) {
            return;
        }
        int spellsToRemove = std::min(count, totalBefore);
        std::cout << "Удаляется " << spellsToRemove << " из " << totalBefore << " заклинаний..." << std::endl;
        
        handSpell->removeRandomSpells(spellsToRemove);
    }
}

void Player::switchAttackType() {
    if (currentAttackType == AttackType::MELEE) {
        currentAttackType = AttackType::RANGED;
        damage = rangedDamage;
        std::cout << "Переключение на ДАЛЬНИЙ БОЙ! Урон: " << damage << std::endl;
    } else {
        currentAttackType = AttackType::MELEE;
        damage = meleeDamage;
        std::cout << "Переключение на БЛИЖНИЙ БОЙ! Урон: " << damage << std::endl;
    }
    if (freeze_time > 0) freeze_time--;
}

void Player::add_coins(int amount) { coins += amount; }

void Player::add_score(int amount) { score += amount; }

void Player::add_xp(int amount) {
    xp += amount;
    if (xp >= level * XP_PER_LEVEL) {
        xp -= level * XP_PER_LEVEL;
        level++;
        max_health += HEALTH_PER_LEVEL;
        health = max_health;
        std::cout << "Уровень Повышен! Теперь уровень " << level << std::endl;
    }
}



int Player::getScore() const { return score; }
int Player::getCoins() const { return coins; }
int Player::getLevel() const { return level; }
int Player::getXp() const { return xp; }
AttackType Player::getAttackType() const { return currentAttackType; }
int Player::getAttackRange() const { return attackRange; }
std::string Player::getAttackTypeString() const {
    return (currentAttackType == AttackType::MELEE) ? "Ближний бой" : "Дальний бой";
}
HandSpell* Player::getHandSpell() const { return handSpell.get();}
int Player::getMeleeDamage() const { return meleeDamage; }
int Player::getRangedDamage() const { return rangedDamage; }

void Player::setName(const std::string& newName) { name = newName; }
void Player::setScore(int newScore) { score = newScore; }
void Player::setCoins(int newCoins) { coins = newCoins; }
void Player::setAlive(bool isAlive) { alive = isAlive; }
void Player::upgradeMeleeDamage(int amount) { meleeDamage += amount; }
void Player::upgradeRangedDamage(int amount) { rangedDamage += amount; }
void Player::setLevel(int newLevel) { level = newLevel; }
void Player::setXp(int newXp) { xp = newXp; }
void Player::setAttackType(AttackType type) { currentAttackType = type; }
void Player::setMeleeDamage(int damage) { meleeDamage = damage; }
void Player::setRangedDamage(int damage) { rangedDamage = damage; }
void Player::setAttackRange(int range) { attackRange = range; }