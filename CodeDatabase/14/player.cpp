#include "player.hpp"
#include "GameBoard.hpp"
#include "SpellFactory.hpp"

Player::Player(int x, int y, int start_health)
    : Entity(x, y), health(start_health), score(0), 
      current_attack_type(MELEE), is_slowed(false),
      player_hand(new Hand(PLAYER_HAND_SIZE)),
      kill_count(0), enhancement_stacks(0), damage_bonus(0) {}

Player::~Player() { delete player_hand; }

int Player::getHealth() const { return health; }
int Player::getScore() const { return score; }
int Player::getDamage() const {
    return ((current_attack_type == MELEE) ? MELEE_DAMAGE : RANGED_DAMAGE) + damage_bonus;
}
double Player::getAttackRange() const {
    return (current_attack_type == MELEE) ? MELEE_RANGE : RANGED_RANGE;
}
void Player::takeDamage(int damage) { health -= damage; }
void Player::addScore(int points) { score += points; }
void Player::removeScore(int points) { score -= points; if(score < 0) score = 0; }
bool Player::isAlive() const { return health > 0; }
void Player::switchAttackType() { current_attack_type = (current_attack_type == MELEE) ? RANGED : MELEE; }
Player::AttackType Player::getAttackType() const { return current_attack_type; }
void Player::setSlowed(bool slowed) { is_slowed = slowed; }
bool Player::isSlowed() const { return is_slowed; }
Hand* Player::getHand() const { return player_hand; }
void Player::incrementKillCount() { kill_count++; }
int Player::getKillCount() const { return kill_count; }
void Player::resetKillCount() { kill_count = 0; }
void Player::addEnhancementStack() { enhancement_stacks++; }
int Player::getEnhancementStacks() const { return enhancement_stacks; }
void Player::clearEnhancementStacks() { enhancement_stacks = 0; }
void Player::upgradeDamage(int bonus) { damage_bonus += bonus; }
void Player::removeFromBoard(GameBoard& board) { (void)board; }

