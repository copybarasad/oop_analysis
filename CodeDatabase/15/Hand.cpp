#include "Hand.h"
#include <algorithm>
#include <random>

Hand::Hand(size_t max_size) : max_size_(max_size) {}

bool Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells_.size() < max_size_) {
        spells_.push_back(std::move(spell));
        return true;
    }
    return false;
}

ISpell* Hand::getSpell(size_t index) const {
    if (index < spells_.size()) {
        return spells_[index].get();
    }
    return nullptr;
}

size_t Hand::getSpellCount() const {
    return spells_.size();
}

void Hand::removeRandomSpells(int count) {
    if (spells_.empty() || count <= 0) return;
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(spells_.begin(), spells_.end(), g);
    
    int remove_cnt = std::min((int)spells_.size(), count);
    spells_.erase(spells_.begin(), spells_.begin() + remove_cnt);
}

size_t Hand::getMaxSize() const { return max_size_; }
void Hand::setMaxSize(size_t size) { max_size_ = size; }
void Hand::clear() { spells_.clear(); }