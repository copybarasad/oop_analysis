#include "SpellHand.hpp"
#include <stdexcept>

SpellHand::SpellHand(std::size_t capacity)
	: spells_(),
	  capacity_(capacity) {}

SpellHand::~SpellHand() {}

std::size_t SpellHand::size() const noexcept {
	return spells_.size();
}

std::size_t SpellHand::capacity() const noexcept {
	return capacity_;
}

bool SpellHand::addSpell(std::unique_ptr<SpellBase> spell) {
	if (!spell) {
		return false;
	}
	if (spells_.size() >= capacity_) {
		return false;
	}
	std::size_t duplicates = 0;
	for (const auto& existing : spells_) {
		if (existing && existing->hotkey() == spell->hotkey()) {
			++duplicates;
			if (duplicates >= 2) {
				return false;
			}
		}
	}
	spells_.push_back(std::move(spell));
	return true;
}

const SpellBase& SpellHand::spellAt(std::size_t index) const {
	if (index >= spells_.size()) {
		throw std::out_of_range("SpellHand::spellAt: bad index");
	}
	return *spells_[index];
}

bool SpellHand::castSpell(std::size_t index, SpellBase::SpellContext& context) {
	if (index >= spells_.size()) {
		return false;
	}

	std::unique_ptr<SpellBase>& card = spells_[index];
	if (!card) {
		return false;
	}

	bool used = card->cast(context);

	if (used) {
		spells_.erase(spells_.begin() + static_cast<std::ptrdiff_t>(index));
		return true;
	}

	return false;
}

int SpellHand::findSpellIndexByHotkey(int hotkey) const {
	for (std::size_t i = 0; i < spells_.size(); ++i) {
		if (spells_[i] && spells_[i]->hotkey() == hotkey) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

const std::vector<std::unique_ptr<SpellBase>>& SpellHand::cards() const noexcept {
	return spells_;
}

const SpellBase* SpellHand::tryGetSpell(std::size_t index) const noexcept {
	if (index >= spells_.size()) {
		return nullptr;
	}
	if (!spells_[index]) {
		return nullptr;
	}
	return spells_[index].get();
}

bool SpellHand::hasSpellHotkey(int hotkey) const noexcept {
	for (const auto& ptr : spells_) {
		if (ptr && ptr->hotkey() == hotkey) {
			return true;
		}
	}
	return false;
}

void SpellHand::clear() noexcept {
	spells_.clear();
}

void SpellHand::removeAt(std::size_t index) {
	if (index >= spells_.size()) {
		throw std::out_of_range("SpellHand::removeAt: bad index");
	}
	spells_.erase(spells_.begin() + static_cast<std::ptrdiff_t>(index));
}
