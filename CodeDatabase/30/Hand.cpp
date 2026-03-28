#include "include/Hand.h"
#include "include/DirectDamageSpell.h"
#include "include/AreaDamageSpell.h"
#include "include/TrapSpell.h"
#include "include/SummonSpell.h"
#include "include/BuffSpell.h"
#include "../gamecore/include/Manager.h"
#include "../gamecore/include/GameException.h"
#include "../gamecore/include/Balance.h"
#include <random>
#include <algorithm>

Hand::Hand(int maxSlots) : maxSlots_(maxSlots) {
    addRandomSpell();
    current_ = 0;
}

void Hand::addSpell(std::unique_ptr<Spell> s) {
    if (!s) return;
	if (!hasFreeSlot()) return;
    slots_.push_back(std::move(s));
}

void Hand::addRandomSpell() {
	if (!hasFreeSlot()) return;
	std::vector<SpellType> pool = {SpellType::DirectDamage, SpellType::AreaDamage, SpellType::Trap, SpellType::Summon, SpellType::Buff};
	std::random_device rd; std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, (int)pool.size()-1);
	SpellType t = pool[dist(gen)];
	switch(t) {
		case SpellType::DirectDamage: 
            addSpell(std::make_unique<DirectDamageSpell>(Balance::SpellDirectDmg, Balance::SpellDirectRange)); 
            break;
		case SpellType::AreaDamage: 
            addSpell(std::make_unique<AreaDamageSpell>(Balance::SpellAreaDmg, Balance::SpellAreaRange)); 
            break;
		case SpellType::Trap: 
            addSpell(std::make_unique<TrapSpell>(Balance::SpellTrapDmg)); 
            break;
		case SpellType::Summon: 
            addSpell(std::make_unique<SummonSpell>(Balance::AllyHP, Balance::AllyDmg)); 
            break;
		case SpellType::Buff: 
            addSpell(std::make_unique<BuffSpell>()); 
            break;
	}
}

void Hand::select(size_t index) {
	if (index < slots_.size()) current_ = index;
}

Spell* Hand::current() {
	if (slots_.empty()) return nullptr;
	return slots_[current_].get();
}

UpgradeState& Hand::upgrades() { return upgrades_; }
void Hand::resetUpgrades() { upgrades_.reset(); }

bool Hand::useCurrent(Manager& mng, EntityId caster, const Position& target) {
    Spell* s = current();
    if (!s) return false;
    
    bool success = false;
    if (s->type() == SpellType::Buff) {
		if (!s->canCast(mng, caster, target, nullptr)) return false;
		if (!s->cast(mng, caster, target, nullptr)) return false;
		upgrades_.stacks += 1; // просто копим счётчик тут
        success = true;
	} else {
	   if (!s->canCast(mng, caster, target, &upgrades_)) return false;
	   bool ok = s->cast(mng, caster, target, &upgrades_);
		   if (ok) resetUpgrades();
		   success = ok;
    }

    if (success && !s->hasCharges()) {
        slots_.erase(slots_.begin() + current_);
        if (current_ >= slots_.size() && !slots_.empty()) {
            current_ = slots_.size() - 1;
        } else if (slots_.empty()) {
            current_ = 0;
        }
    }
    return success;
}

void Hand::removeRandomSpells(double ratio) {
    if (slots_.empty()) return;
    int toRemove = static_cast<int>(std::ceil(slots_.size() * ratio));
    if (toRemove <= 0 && !slots_.empty()) toRemove = 1;
    if (toRemove > static_cast<int>(slots_.size())) toRemove = static_cast<int>(slots_.size());

    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Перемешиваем и удаляем с конца
    std::shuffle(slots_.begin(), slots_.end(), gen);
    for (int i = 0; i < toRemove; ++i) {
        slots_.pop_back();
    }
    // Сбрасываем текущий выбор
    current_ = 0;
}

void Hand::save(std::ostream& os) const {
    os << maxSlots_ << " " << current_ << " " << upgrades_.stacks << "\n";
    os << slots_.size() << "\n";
    for (const auto& s : slots_) {
        s->save(os);
    }
}

void Hand::load(std::istream& is) {
    slots_.clear();
    int count;
    if (!(is >> maxSlots_ >> current_ >> upgrades_.stacks)) return;
    if (!(is >> count)) return;
    
    for (int i = 0; i < count; ++i) {
        int typeInt;
        if (!(is >> typeInt)) break;
        SpellType type = static_cast<SpellType>(typeInt);
        
        std::unique_ptr<Spell> spell;
        switch (type) {
            case SpellType::DirectDamage: spell = std::make_unique<DirectDamageSpell>(); break;
            case SpellType::AreaDamage: spell = std::make_unique<AreaDamageSpell>(); break;
            case SpellType::Trap: spell = std::make_unique<TrapSpell>(); break;
            case SpellType::Summon: spell = std::make_unique<SummonSpell>(); break;
            case SpellType::Buff: spell = std::make_unique<BuffSpell>(); break;
            default: 
                 // Попытка восстановления или выброс исключения.
                 throw GameSaveException("Unknown spell type in Hand::load");
        }
        
        if (spell) {
            spell->load(is);
            slots_.push_back(std::move(spell));
        }
    }
    // Проверка корректности текущего индекса
    if (current_ >= slots_.size()) {
        current_ = slots_.empty() ? 0 : slots_.size() - 1;
    }
}
