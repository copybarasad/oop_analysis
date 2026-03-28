#include "hand.h"

Hand::Hand(size_t maxSize) : _maxSize(maxSize) {
    addRandomSpell();
}

void Hand::addRandomSpell() {
    if (_spells.size() >= _maxSize) {
        std::cout << "Рука полна, нельзя добавить новое заклинание.\n";
        return;
    }
    _spells.push_back(SpellFactory::createRandomSpell());
}

void Hand::addSpell(std::unique_ptr<ISpell> spell) {
    
    if (_spells.size() >= _maxSize) {
        std::cout << "Рука полна, нельзя добавить новое заклинание.\n";
        return;
    }
    _spells.push_back(std::move(spell));
}



void Hand::removeUsedSpells() {
    _spells.erase(
        std::remove_if(_spells.begin(), _spells.end(),
            [](const std::unique_ptr<ISpell>& s) { return s->isConsumed(); }),
        _spells.end()
    );
}

ISpell* Hand::getSpell(size_t index) const {
    if (index < _spells.size())
        return _spells[index].get();
    return nullptr;
}

void Hand::printHand() const {
    std::cout << "Заклинания в руке:\n";
    for (size_t i = 0; i < _spells.size(); ++i) {
        std::cout << i + 1 << ") " << _spells[i]->name()
                  << ( _spells[i]->isConsumed() ? " (использовано)" : "" )
                  << std::endl;
    }
}

// Конструктор копирования
Hand::Hand(const Hand& other) : _maxSize(other._maxSize) {
    for (const auto& spell : other._spells) {
        if (spell) {
            _spells.push_back(spell->clone());
        }
    }
}

// Оператор присваивания
Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        _maxSize = other._maxSize;
        _spells.clear();
        for (const auto& spell : other._spells) {
            if (spell) {
                _spells.push_back(spell->clone());
            }
        }
    }
    return *this;
}


    