#pragma once
#include <vector>
#include <memory>
#include <cstddef>
#include "SpellBase.hpp"

class SpellHand {
public:
	explicit SpellHand(std::size_t capacity);
	SpellHand(const SpellHand&) = delete;
	SpellHand& operator=(const SpellHand&) = delete;
	SpellHand(SpellHand&&) noexcept = default;
	SpellHand& operator=(SpellHand&&) noexcept = default;
	~SpellHand();

	std::size_t size() const noexcept;
	std::size_t capacity() const noexcept;

	const SpellBase& spellAt(std::size_t index) const;

	bool castSpell(std::size_t index, SpellBase::SpellContext& context);

	int findSpellIndexByHotkey(int hotkey) const;
	const std::vector<std::unique_ptr<SpellBase>>& cards() const noexcept;
	const SpellBase* tryGetSpell(std::size_t index) const noexcept;

	bool addSpell(std::unique_ptr<SpellBase> spell);

	bool hasSpellHotkey(int hotkey) const noexcept;
	void clear() noexcept;
	void removeAt(std::size_t index);

private:
	std::vector<std::unique_ptr<SpellBase>> spells_;
	std::size_t capacity_;
};
