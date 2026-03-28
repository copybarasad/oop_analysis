#include "include/Player.h"
#include "../../items/include/Hand.h"
#include <random>
#include <iostream>

Player::Player(int hp, int baseMeleeDamage) {
	hp_ = hp > 0 ? hp : 0;
    maxHp_ = hp_;
	meleeDamage_ = baseMeleeDamage;
	rangedDamage_ = static_cast<int>(baseMeleeDamage * DMG_COEFF);
	damage_ = meleeDamage_;
	hand_ = std::make_unique<Hand>(5); // лимит по количеству карт (слотов)
}

Player::~Player() = default;

int Player::hp() const { return hp_; }
int Player::maxHp() const { return maxHp_; }
int Player::damage() const { return damage_; }

void Player::switchToMelee() { mode_ = AttackMode::Melee; damage_ = meleeDamage_; }
void Player::switchToRanged() { mode_ = AttackMode::Ranged; damage_ = rangedDamage_; }
AttackMode Player::attackMode() const { return mode_; }
bool Player::isSlowed() const { return slowed_; }
void Player::setSlowed(bool v) { slowed_ = v; }

Hand& Player::hand() { return *hand_; }
const Hand& Player::hand() const { return *hand_; }

std::unique_ptr<Hand> Player::takeHand() {
    return std::move(hand_);
}

void Player::setHand(std::unique_ptr<Hand> h) {
    if (h) hand_ = std::move(h);
}

void Player::giveKillReward() {
	++killCounter_;
	if (killCounter_ % 3 == 0) {
		// получение новой карты. Если нет свободного слота — добавим заряд случайной карте.
		if (hand_->hasFreeSlot()) {
			hand_->addRandomSpell();
		} else if (!hand().slots().empty()) {
			std::random_device rd; std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, (int)hand().slots().size() - 1);
			int idx = dist(gen);
			hand().slots()[idx]->addCharges(1);
		}
	}
}

EntityType Player::type() const { return EntityType::Player; }

void Player::save(std::ostream& os) const {
    Entity::save(os);
    os << meleeDamage_ << " " << rangedDamage_ << " " << damage_ << " " 
       << static_cast<int>(mode_) << " " << slowed_ << " " << killCounter_ << " " << maxHp_ << "\n";
    hand_->save(os);
}

void Player::load(std::istream& is) {
    Entity::load(is);
    int modeInt;
    is >> meleeDamage_ >> rangedDamage_ >> damage_ >> modeInt >> slowed_ >> killCounter_;
    // Попытка прочитать maxHp, если доступно (для обратной совместимости)
    // Если чтение не удалось, сбрасываем флаг ошибки и устанавливаем maxHp = hp
    if (!(is >> maxHp_)) {
        is.clear();
        maxHp_ = hp_;
    }
    mode_ = static_cast<AttackMode>(modeInt);
    hand_->load(is);
}

void Player::upgradeMaxHp(int amount) {
    maxHp_ += amount;
    hp_ += amount;
}

void Player::upgradeDamage(int amount) {
    meleeDamage_ += amount;
    rangedDamage_ = static_cast<int>(meleeDamage_ * DMG_COEFF);
    if (mode_ == AttackMode::Melee) damage_ = meleeDamage_;
    else damage_ = rangedDamage_;
}

int Player::baseDamage() const {
    return meleeDamage_;
}
