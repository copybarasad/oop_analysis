#include "Hand.h"
#include "DamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include <random>

Hand::Hand(size_t capacity) 
    : capacity_(capacity) 
{
    GenerateRandomSpell();
}

bool Hand::AddSpell(std::unique_ptr<Spell> s) 
{
    if (spells_.size() >= capacity_) {
        return false;
    }

    spells_.push_back(std::move(s));
    return true;
}


bool Hand::UseSpell(size_t index, Player& caster, Game& game, Position target)
{
    if (index >= spells_.size())
        return false;

    auto& spell = spells_[index];

    if (dynamic_cast<BuffSpell*>(spell.get()) == nullptr)
    {
        spell->ApplyBuffs(caster.GetAccumulatedBuff());
        caster.ClearBuffs();
    }
    bool success = spell->Apply(caster, game, target);

    if (success)
    {
        std::cout << "Spell " << spell->GetName() << " used and removed from hand.\n";
        spells_.erase(spells_.begin() + index);
    }
    else
    {
        std::cout << "Spell " << spell->GetName() << " failed and remains in hand.\n";
    }

    return success;
}

std::vector<std::string> Hand::GetSpellNames() const 
{
    std::vector<std::string> out;

    for (auto const& s : spells_) {
        out.push_back(s->GetName());
    }

    return out;
}


Spell* Hand::GetSpell(size_t index)
{
    if (index < spells_.size()) {
        return spells_[index].get(); 
    }
    return nullptr;
}

size_t Hand::Size() const 
{ 
    return spells_.size(); 
}

size_t Hand::Capacity() const 
{ 
    return capacity_; 
}

void Hand::GenerateRandomSpell() {
    auto spell = CreateRandomSpell();
    if (spell) {
        AddSpell(std::move(spell));
    }
}

std::unique_ptr<Spell> Hand::CreateRandomSpell() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 4);
    int t = dist(gen);

    switch (t) {
        case 0: return std::make_unique<DamageSpell>("Damage spell", 10, 3);
        case 1: return std::make_unique<AreaDamageSpell>("Area damage spell", 10, 2);
        case 2: return std::make_unique<TrapSpell>("Trap", 4);
        case 3: return std::make_unique<SummonSpell>("Ally", 1, 3);
        default: return std::make_unique<BuffSpell>("Buff Spell");
    }
}


void Hand::Clear() { 
    spells_.clear(); 
}

void Hand::RemoveAt(size_t index)
{
    if (index < spells_.size()) {

        spells_.erase(spells_.begin() + index);
    }
}



//*****
std::unique_ptr<Spell> Hand::CreateSpellByName(const std::string& name)
{
    // Match names used in CreateRandomSpell and Save logic
    if (name == "DamageSpell" || name == "Damage spell")       return std::make_unique<DamageSpell>("DamageSpell", 10, 3);
    if (name == "AreaDamageSpell" || name == "Area damage spell")   return std::make_unique<AreaDamageSpell>("AreaDamageSpell", 10, 2);
    if (name == "SummonSpell" || name == "Ally")       return std::make_unique<SummonSpell>("SummonSpell", 1, 2);
    if (name == "BuffSpell" || name == "Buff Spell" || name == "Buff")         return std::make_unique<BuffSpell>("BuffSpell");
    
    // Fix: correct type and handle "Trap"
    if (name == "TrapSpell" || name == "Trap")         return std::make_unique<TrapSpell>("TrapSpell", 5); 

    throw std::runtime_error("Unknown spell name: " + name);
}
