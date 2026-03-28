#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "EnemyTower.h"   
#include "Logger.h"
#include <stdexcept>

Player::Player(int startX, int startY, int h, int mDamage, int rDamage)
    : x(startX), y(startY), health(h),
      maxHealth(h),
      score(0),
      meleeDamage(mDamage), rangedDamage(rDamage),
      attackRange(5),
      baseHealth(h),
      baseMelee(mDamage),
      baseRanged(rDamage), currentMode(CombatMode::MELEE), isSlowed(false),
      hand(std::make_unique<PlayerHand>(5)),
      buffCharges(0)
{}

void Player::attack(int dx, int dy, Field& field, 
                    std::vector<Enemy>& enemies, 
                    std::vector<EnemySpawner>& spawners, 
                    std::vector<EnemyTower>& towers) 
{
    int dmg = getDamage();

    if (currentMode == CombatMode::MELEE) {
        int targetX = x + dx, targetY = y + dy;
        CellType type = field.getCellType(targetX, targetY);

        if (type == CellType::ENEMY) {
            for (auto& e : enemies) {
                if (e.isAlive() && e.getX() == targetX && e.getY() == targetY) {
                    e.takeDamage(dmg); return;
                }
            }
        } else if (type == CellType::SPAWNER) {
            for (auto& s : spawners) {
                if (s.isAlive() && s.getX() == targetX && s.getY() == targetY) {
                    s.takeDamage(dmg); return;
                }
            }
        } else if (type == CellType::ENEMY_TOWER) {
            for (auto& t : towers) {
                if (t.isAlive() && t.getX() == targetX && t.getY() == targetY) {
                    t.takeDamage(dmg); return;
                }
            }
        }
    } else {
        for (int i = 1; i <= attackRange; ++i) {
            int projX = x + i * dx, projY = y + i * dy;
            if (!field.isValidPosition(projX, projY) || field.getCellType(projX, projY) == CellType::WALL) break;
            
            CellType type = field.getCellType(projX, projY);
            
            if (type == CellType::ENEMY) {
                for (auto& e : enemies) {
                    if (e.isAlive() && e.getX() == projX && e.getY() == projY) {
                        e.takeDamage(dmg); return;
                    }
                }
            } else if (type == CellType::SPAWNER) {
                for (auto& s : spawners) {
                    if (s.isAlive() && s.getX() == projX && s.getY() == projY) {
                        s.takeDamage(dmg); return;
                    }
                }
            } else if (type == CellType::ENEMY_TOWER) {
                for (auto& t : towers) {
                    if (t.isAlive() && t.getX() == projX && t.getY() == projY) {
                        t.takeDamage(dmg); return;
                    }
                }
            }
        }
    }
}

bool Player::spendScore(int amount) {
    if (score >= amount) {
        score -= amount;
        return true;
    }
    return false;
}


PlayerHand* Player::getHand() const {return hand.get();}
void Player::addBuffCharge() {buffCharges++;}
int Player::getBuffCharges() const { return buffCharges; }
void Player::consumeBuffCharges() { buffCharges = 0; }
int Player::getX() const {return x;}
int Player::getY() const {return y;}
int Player::getHealth() const {return health;}
int Player::getScore() const {return score;}
void Player::addScore(int points) {score += points;}
int Player::getDamage() const { return (currentMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;}
std::string Player::getModeString() const { return (currentMode == CombatMode::MELEE) ? "Melee" : "Ranged";}
CombatMode Player::getMode() const { return currentMode;}
bool Player::isSlowedThisTurn() const { return isSlowed;}
void Player::processTrap() {isSlowed = false;}

MoveResult Player::move(int dx, int dy, Field& field) {
    int nextX = x + dx, nextY = y + dy;
    if (!field.isValidPosition(nextX, nextY)) return MoveResult::INVALID_POSITION;
    CellType targetCell = field.getCellType(nextX, nextY);
    if (targetCell == CellType::EMPTY) {updatePosition(nextX, nextY, field); return MoveResult::SUCCESS;}
    if (targetCell == CellType::SLOW) {updatePosition(nextX, nextY, field); isSlowed = true; return MoveResult::SUCCESS_SLOWED;}
    if (targetCell == CellType::WALL) return MoveResult::WALL;
    return MoveResult::OCCUPIED;
}

void Player::switchMode() {currentMode = (currentMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;}

void Player::updatePosition(int newX, int newY, Field& field) {
    field.setCellType(x, y, CellType::EMPTY);
    x = newX; y = newY;
    field.setCellType(x, y, CellType::PLAYER);
}

void Player::takeDamage(int amount) {health -= amount; if (health < 0) health = 0;}
bool Player::isAlive() const {return health > 0;}

void Player::save(std::ofstream& out) const {
    out << x << " " << y << " " << health << " " << maxHealth << " " << score << " " 
        << meleeDamage << " " << rangedDamage << " " << buffCharges << " " 
        << static_cast<int>(currentMode) << "\n";
    hand->save(out);
}

void Player::load(std::ifstream& in) {
    int modeVal;
    if (!(in >> x >> y >> health >> maxHealth >> score >> meleeDamage >> rangedDamage >> buffCharges >> modeVal)) 
        throw std::runtime_error("Error loading player stats");
    
    currentMode = static_cast<CombatMode>(modeVal);
    hand->load(in);
}

void Player::teleport(int newX, int newY) {x = newX; y = newY;}

void Player::healFull() {health = maxHealth;}

void Player::upgradeDamage(int amount) {meleeDamage += amount; rangedDamage += amount;}

void Player::upgradeHealth(int amount) {
    maxHealth += amount; 
    health += amount;
}

void Player::reset() {
    maxHealth = baseHealth;
    health = baseHealth;
    meleeDamage = baseMelee;
    rangedDamage = baseRanged;
    
    score = 0;
    buffCharges = 0;
    isSlowed = false;
    currentMode = CombatMode::MELEE;
    hand = std::make_unique<PlayerHand>(5);
}