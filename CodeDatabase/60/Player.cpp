#include <Player.hpp>
#include <Constants.hpp>
#include <string>
#include <Entity.hpp>
#include <memory>
#include <Floor.hpp>
#include <Menu.hpp>
#include <Enemy.hpp>
#include <Utils.hpp>
#include <BattleController.hpp>

Player::Player(int x, int y) : LivingEntity(x, y, Entity::Type::PLAYER, PL_HEALTH, 0), damage(MELEE_DAMAGE), weaponMode(MELEE_MODE), 
    level(1), expToLvlUp(100), mp(PL_MP), mpMax(PL_MAX_MP), scale(1.0f) {}

void Player::resetAttackFlag() {wasAttacked = false;}

bool Player::getWasAttacked() const {return wasAttacked;}

void Player::setWasAttacked(bool val) {wasAttacked = val;}

bool Player::getInTrap() {return inTrap;}

int Player::getMp() const {return mp;}

void Player::setInTrap() {
    inTrap = true;
}

void Player::unSetInTrap() {
    inTrap = false;
}

void Player::useSkill(int cost) {
    mp -= cost;
}

void Player::giveMp(int quantity) {
    if (mp < mpMax) {
        mp += quantity;
    }
}

int Player::getLvl() const {
    return level;
}

void Player::giveExp(int exp) {
    this->exp += exp;

    while (this->exp >= expToLvlUp) {
        lvlUp();
        this->exp -= expToLvlUp;
        expToLvlUp += 100;
    }
}

int Player::getExpToLvlUp() const {
    return expToLvlUp;
}

int Player::getExp() const {
    return exp;
}

void Player::lvlUp() {
    level += 1;

    health += static_cast<int>(10 * scale); 
    damage += static_cast<int>(5 * scale);
    mpMax += static_cast<int>(2 * scale);

    if (level % 5 == 0) {
        Menu::openLearnMenu(gauntlet);
    }
}

float Player::getScale() const { 
    return scale;
}

void Player::setScale(float value) { 
    this->scale = value;
}

bool Player::handleInput(std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, int ch, const ScreenSize* screen, int height, int width) {
    if (ch == SPACE) {return true;}

    int nextX = x;
    int nextY = y;
    bool moved = false;
    switch (ch) {
        case UP:
            if (nextY > screen->yMax/2 - height/2 + 1) {nextY--; moved = true;}
            break;
        case DOWN:
            if (nextY < screen->yMax/2 + height/2 - 2) {nextY++; moved = true;}
            break;
        case LEFT:
            if (nextX > screen->xMax/2 - width/2 + 1) {nextX--; moved = true;}
            break;
        case RIGHT:
            if (nextX < screen->xMax/2 + width/2 - 2) {nextX++; moved = true;}
            break;
        case TAB:
            switchMode();
            napms(200);
            flushinp();
            return true;
        case 'k':
            moved = gauntlet.activateMapSkill(Menu::openSpellMenu(gauntlet.getKnownSpells()), std::static_pointer_cast<Player>(shared_from_this()), ceils, entities, screen);
            return moved;
        case 'v':
            lvlUp();
            return true;
        default:
            return false;
    }

    if (!moved) {
        return false;
    }

    auto& targetCell = ceils[nextY - screen->yMax/2 + height/2][nextX - screen->xMax/2 + width/2];
    auto targetType = targetCell.getEntity()->getType();

    if (targetType == Entity::Type::FLOOR) {

        ceils[y - screen->yMax/2 + height/2][x - screen->xMax/2 + width/2].setEntity(std::make_shared<Floor>(x, y));

        x = nextX;
        y = nextY;

        ceils[y - screen->yMax/2 + height/2][x - screen->xMax/2 + width/2].setEntity(shared_from_this());
        return true;
    } 

    else if (targetType == Entity::Type::ENEMY) {
        wasAttacked = true;

        auto enemy_ptr = std::static_pointer_cast<Enemy>(targetCell.getEntity());

        BattleController battle(screen, ceils, std::static_pointer_cast<Player>(shared_from_this()), enemy_ptr, BattleController::BattleTurn::PLAYER, height, width);
        int result = battle.start(); 

        if (result == 1) {

            int enemyX = enemy_ptr->getX();
            int enemyY = enemy_ptr->getY();

            ceils[y - screen->yMax/2 + height/2][x - screen->xMax/2 + width/2].setEntity(std::make_shared<Floor>(x, y));

            this->x = enemyX;
            this->y = enemyY;

            ceils[y - screen->yMax/2 + height/2][x - screen->xMax/2 + width/2].setEntity(shared_from_this());
        }

        return true;
    } 

    else if (targetType == Entity::Type::SPIKED_TRAP) {
        setInTrap();
        ceils[y - screen->yMax/2 + height/2][x - screen->xMax/2 + width/2].setEntity(std::make_shared<Floor>(x, y));
        x = nextX;
        y = nextY;
        ceils[y - screen->yMax/2 + height/2][x - screen->xMax/2 + width/2].setEntity(shared_from_this());
        return true;
    }

    return false;
}

void Player::draw() {
    if (isBeingTargeted) {
        attron(COLOR_PAIR(1));
    }

    mvaddch(y, x, sprite);

    if (isBeingTargeted) {
        attroff(COLOR_PAIR(1));
    }
}

void Player::drawUI() const {

    attroff(A_REVERSE);

    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);

    Utils::printMultiline(0, 0, Utils::readFileToString(KNIGHT));
    Utils::printMultiline(0, 32, Utils::readFileToString(HP));
    Utils::printNumbers(0, 47, health);
    Utils::printMultiline(5, 32, Utils::readFileToString("../assets/bar/mp.txt"));
    Utils::printNumbers(5, 48, mp);
    Utils::printMultiline(10, 32, Utils::readFileToString("../assets/bar/charges.txt"));
    Utils::printNumbers(10, 47, empowermentCharges);
    Utils::printMultiline(15, 32, Utils::readFileToString(DMG));
    Utils::printNumbers(15, 54, damage);
    Utils::printMultiline(20, 32, Utils::readFileToString("../assets/bar/level.txt"));
    Utils::printNumbers(20, 52, level);
    mvprintw(26, 32, "%0.1f", scale);

    if (weaponMode == MELEE_MODE) {
        Utils::printMultiline(0, xMax - 30, Utils::readFileToString(SWORD));
    } else {
        Utils::printMultiline(0, xMax - 30, Utils::readFileToString(BOW));
    }

    std::string expText = std::to_string(exp) + "/" + std::to_string(expToLvlUp);
    int startX = 15 - (expText.length() / 2);

    mvprintw(23, startX, "%s", expText.c_str());
}

int Player::attack() {
    return damage;
}

bool Player::getMode() {
    return weaponMode;
}

void Player::switchMode() {
    if (weaponMode == MELEE_MODE) {
        weaponMode = RANGE_MODE;
        damage = RANGE_DAMAGE;
    } else {
        weaponMode = MELEE_MODE;
        damage = MELEE_DAMAGE;
    }
}

void Player::setTargeted(bool targeted) {
    isBeingTargeted = targeted;
}

bool Player::isTargeted() const {
    return isBeingTargeted;
}

void Player::addEmpowermentCharge() {
    empowermentCharges++;
}

int Player::getEmpowermentCharges() const {
    return empowermentCharges;
}

void Player::clearEmpowermentCharges() {
    empowermentCharges = 0;
}

int Player::getMpMax() const {
    return mpMax;
}

const std::vector<std::shared_ptr<Spell>>& Player::getSpells() const {
    return gauntlet.getKnownSpells();
}

int Player::getDamage() const {
    return damage;
}

void Player::clearSpells() {
    gauntlet.clear(); 
}

void Player::learnSpell(std::shared_ptr<Spell> newSpell) {
    gauntlet.learnSpell(newSpell);
}

void Player::setMp(int value) {
    mp = value;
}

void Player::setMpMax(int value) {
    mpMax = value;
}

void Player::setLvl(int value) {
    level = value;
}

void Player::setExp(int value) {
    exp = value;
}

void Player::setExpToLvlUp(int value) {
    expToLvlUp = value;
}

void Player::setDamage(int value) {
    damage = value;
}

void Player::setMode(bool isRangeMode) {
    weaponMode = isRangeMode;
}

void Player::setEmpowermentCharges(int value) {
    empowermentCharges = value;
}

void Player::prepareForNextLevel(int newLevel) { 

    health = PL_HEALTH + (level * static_cast<int>(10 * scale)); 

    gauntlet.randomlyRemoveHalfSpells();

    scale = 1.0f + (newLevel - 1) * 0.2f;
}