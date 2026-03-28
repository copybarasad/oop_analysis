#include "../../headers/objects/Player.h"
#include "../../headers/spells/SpellHand.h"
#include "../../headers/spells/DirectDamageSpell.h"
#include "../../headers/spells/AreaDamageSpell.h"
#include "../../headers/spells/TrapSpell.h"
#include <sstream>
#include <iostream>

using namespace std;

Player::Player(int startX, int startY, int handSize, int lvl): 
    position(startX, startY), max_health(100), health(100), 
    melee_damage(30), ranged_damage(20), ranged_range(3), 
    score(0), current_attack(AttackType::MELEE), is_switch_weapon(false),
    spellHand(handSize), enemies_defeated(0), level(lvl) {
    max_health += (lvl - 1) * 20;
    health = max_health;
    melee_damage += (lvl - 1) * 5;
    ranged_damage += (lvl - 1) * 3;
}

void Player::takeDamage(int mount_damage) {
    health -= mount_damage;
    if(health < 0) health = 0;
}

void Player::heal(int amount_heal) {
    health += amount_heal;
    if (health > max_health) health = max_health;
}

void Player::addScore(int points) {
    score += points;
}

void Player::addEnemyDefeated() {
    enemies_defeated++;
    if (enemies_defeated % 3 == 0) {
        cout << "You earned a new spell!\n";
        spellHand.addRandomSpell();
    }
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::moving(const Position& new_pos){
    position = new_pos;
}

void Player::switchWeapon(){
    if(current_attack == AttackType::MELEE){
        current_attack = AttackType::RANGED;
    }
    else{
        current_attack = AttackType::MELEE;
    }
    is_switch_weapon = true;
}

bool Player::getIsSwitchingWeapon() const {return is_switch_weapon; }

AttackType Player::getAtackType() const {return current_attack; }

int Player::getDamage() const {
    return (current_attack == AttackType::MELEE) ? melee_damage : ranged_damage;
}

int Player::getRangedRange() const { return ranged_range; }

string Player::getWeaponName() const {
    return (current_attack == AttackType::MELEE) ? "MELEE_WEAPON" : "RANGED_WEAPON";
}

int Player::getHealth() const { return health; }

int Player::getMaxHealth() const { return max_health; }

int Player::getScore() const { return score; }

Position Player::getPosition() const { return position; }

int Player::getLevel() const { return level; }

SpellHand& Player::getSpellHand() { return spellHand; }

bool Player::buySpell() {
    if (spellHand.getSpellCount() >= spellHand.getMaxSize()) {
        cout << "Spell hand is full! Cannot buy more spells.\n";
        return false;
    }
    
    if (score >= 20) {
        score -= 20;
        spellHand.addRandomSpell();
        cout << "Bought a new spell! Remaining score: " << score << "\n";
        return true;
    } else {
        cout << "Not enough score! Need 20, have " << score << "\n";
        return false;
    }
}

void Player::prepareForNextLevel() {
    int currentSpellCount = spellHand.getSpellCount();

    int spellsToRemove = currentSpellCount / 2;
    if (spellsToRemove == 0 && currentSpellCount > 0) {
        spellsToRemove = 1;
    }
    
    cout << "Removing " << spellsToRemove << " spells during level transition...\n";
    
    spellHand.removeRandomSpells(spellsToRemove);
    level++;
    
    max_health += 20;
    health = max_health;
    melee_damage += 5;
    ranged_damage += 3;
    score = 0;
}

string Player::serialize() const {
    stringstream ss;
    ss << "Player " << position.toString() << " " << health << " " << max_health << " "
       << score << " " << melee_damage << " " << ranged_damage << " " << ranged_range << " "
       << static_cast<int>(current_attack) << " " << enemies_defeated << " " << level;
    return ss.str();
}

shared_ptr<Player> Player::deserialize(const string& data) {
    stringstream ss(data);
    string type;
    ss >> type;
    if (type != "Player") throw invalid_argument("Not a Player");
    
    string posStr;
    int health, max_health, score, melee_damage, ranged_damage, ranged_range;
    int attackType, enemies_defeated, level;
    if(!(ss >> posStr >> health >> max_health >> score >> melee_damage >> ranged_damage 
       >> ranged_range >> attackType >> enemies_defeated >> level)){
        throw invalid_argument("Invalid player data format");
    }
    
    Position pos = Position::fromString(posStr);
    auto player = make_shared<Player>(pos.getX(), pos.getY(), 3, level);
    player->health = health;
    player->max_health = max_health;
    player->score = score;
    player->melee_damage = melee_damage;
    player->ranged_damage = ranged_damage;
    player->ranged_range = ranged_range;
    player->current_attack = static_cast<AttackType>(attackType);
    player->enemies_defeated = enemies_defeated;
    return player;
}