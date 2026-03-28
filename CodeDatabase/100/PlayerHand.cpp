#include "PlayerHand.h"

PlayerHand::PlayerHand(size_t maxHandSize) : maxSize(maxHandSize > 0 ? maxHandSize : 3) { }

// Проверка на правильно индекса заклинания в руке
bool PlayerHand::isValidIndex(size_t index) const { return index < spells.size(); }

// Методы для управления заклинаниями
// Добавить заклинание в руку
bool PlayerHand::addSpell(std::shared_ptr<Spell> spell) {
    if (!spell || isFull()) return false;

    spells.push_back(spell);
    return true;
}

// Удалить заклинание из руки
bool PlayerHand::removeSpell(size_t index) {
    if (!isValidIndex(index)) return false;

    spells.erase(spells.begin() + index);
    return true;
}

// Использовать заклинание: вызывает use у конкретного класс - результат bool, если использовано, то удляет
bool PlayerHand::useSpell(size_t index, Player& player, GameState& state, int targetX, int targetY) {
    if (!isValidIndex(index)) return false;

    bool spellUsed = spells[index]->use(player, state, targetX, targetY);

    if (spellUsed) return removeSpell(index);
    return false;
}

// Добавить рандомное заклинание в руку
void PlayerHand::addRandomSpell(std::shared_ptr<Spell> spell) {
    if (!spell || isFull()) return;

    spells.push_back(spell);
}

// Очистить руку заклинаний
void PlayerHand::clearHand() { spells.clear(); }

// Геттеры
std::shared_ptr<const Spell> PlayerHand::getSpell(size_t index) const {
    if (!isValidIndex(index)) { return nullptr; }
    return spells[index];
}

const std::vector<std::shared_ptr<Spell>>& PlayerHand::getAllSpells() const { return spells; }

bool PlayerHand::isFull() const { return spells.size() >= maxSize; }

bool PlayerHand::isEmpty() const { return spells.empty(); }

size_t PlayerHand::getCurrentSize() const { return spells.size(); }

size_t PlayerHand::getMaxSize() const { return maxSize; }

// Метод для улучшения: вызывает upgrade у конкретного заклинания
bool PlayerHand::upgradeSpell(size_t index) {
    if (!isValidIndex(index)) {
        return false;
    }
    
    auto spell = spells[index];
    if (spell) {
        spell->upgrade();
        return true;
    }
    
    return false;
}