#include "player_hand.h"
#include "player.h"
#include <iostream>

PlayerHand::PlayerHand(size_t max_size){
	max_size_ = max_size;
}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell){
	if (isFull()) {
		std::cout << "Рука заполнена! Невозможно получить новое заклинание." << std::endl;
		return false;
	}

	if (spell) {
		std::cout << "Игрок получает новое заклинание: " << spell->getName() << "!" << std::endl;
		spells_in_hand_.push_back(std::move(spell));
		return true;
	}
	return false;
}

SpellCastResult PlayerHand::useSpell(size_t index, Player* caster, PlayingField& field) {
	if (index < spells_in_hand_.size()) {
		SpellCastResult result = spells_in_hand_[index]->apply(caster, field);

		if (result == SpellCastResult::SUCCESS) {
			std::cout << "Карточка заклинания '" << spells_in_hand_[index]->getName() << "' потрачена." << std::endl;
			spells_in_hand_.erase(spells_in_hand_.begin() + index);
		}
		else {
			std::cout << "Карточка заклинания осталась в руке." << std::endl;
		}
		return result;
	}
	else {
		std::cout << "Некорректный номер заклинания!" << std::endl;
		return SpellCastResult::INVALID_TARGET;
	}
}

void PlayerHand::show() const {
	std::cout << "\n--- Заклинания в руке (" << spells_in_hand_.size() << " из " << max_size_ << " доступно) ---" << std::endl;

	if (spells_in_hand_.empty()) {
		std::cout << "У вас нет заклинаний." << std::endl;
	}
	else {
		for (size_t i = 0; i < spells_in_hand_.size(); ++i) {
			std::cout << i + 1 << ". " << spells_in_hand_[i]->getName()
				<< " (" << spells_in_hand_[i]->getDescription() << ")" << std::endl;
		}
	}
	std::cout << "---------------------------------" << std::endl;
}

bool PlayerHand::isFull() const {
	return spells_in_hand_.size() >= max_size_;
}

size_t PlayerHand::getSpellsCount() const {
	return spells_in_hand_.size();
}

size_t PlayerHand::getMaxSize() const {
	return max_size_;
}

const std::vector<std::unique_ptr<Spell>>& PlayerHand::getSpells() const {
	return spells_in_hand_;
}

void PlayerHand::restoreSpell(std::unique_ptr<Spell> spell) {
	if (!isFull() && spell) {
		spells_in_hand_.push_back(std::move(spell));
	}
}