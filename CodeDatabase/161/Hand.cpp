#include "Hand.h"

Hand::Hand(int limit)
	: limit_(std::max(limit, 5)), upgrade_coef_(0) {}

Hand::Hand()
	: Hand(5) {}

bool Hand::addSpell(std::shared_ptr<ISpell> spell) {
	if (!spell || spells_.size() >= static_cast<size_t>(limit_))
		return false;

	spells_.push_back(spell);
	return true;
}

bool Hand::removeSpell(size_t index) {
	if (index >= spells_.size())
		return false;

	spells_.erase(spells_.begin() + index);
	return true;
};


std::shared_ptr<ISpell> Hand::getSpell(size_t index) {
	if (index >= spells_.size())
		return nullptr;

	if (upgrade_coef_ != 0) {

		spells_[index]->upgrade(upgrade_coef_);
		upgrade_coef_ = 0;
	}

	return spells_[index];
}

void Hand::setUpgradeCoef(int coef) {
	if (upgrade_coef_ != 0)
		return;

	upgrade_coef_ = std::max(coef, 1);
}

int Hand::isUpgradeEnabled() const {
	return upgrade_coef_;
}

size_t Hand::size() const {
	return spells_.size();
}

size_t Hand::capacity() const {
	return limit_;
}

bool Hand::isFull() const {
	return spells_.size() >= static_cast<size_t>(limit_);
}

void Hand::setRandomSpell(){
	if(isFull())
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> numSpell(0, 4);

	int num = numSpell(gen);

	switch (num) {
	case 0:
		spells_.push_back(std::make_shared<DirDamageSpell>());
		break;
	case 1:
		spells_.push_back(std::make_shared<AreaDmgSpell>());

		break;
	case 2:
		spells_.push_back(std::make_shared<EnhacementSpell>());

		break;
	case 3:
		spells_.push_back(std::make_shared<SummSpell>());

		break;
	case 4:
		spells_.push_back(std::make_shared<TrapSpell>());

		break;
	}
}

void Hand::removeRandomHalf() {
	if (spells_.empty())
		return;

	size_t to_remove = spells_.size() / 2;

	std::random_device rd;
	std::mt19937 gen(rd());

	for (size_t i = 0; i < to_remove; ++i) {
		std::uniform_int_distribution<> dist(0, spells_.size() - 1);
		size_t index = dist(gen);
		spells_.erase(spells_.begin() + index);
	}
}

void Hand::GiveStartingSpellIfNewGame() {
	if (spells_.empty()) {
		setRandomSpell();
	}
}

void Hand::clear() {
	spells_.clear();
}