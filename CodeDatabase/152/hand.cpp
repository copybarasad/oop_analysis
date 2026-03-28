#include "hand.h"

Hand::Hand(size_t max_size) : max_size_(max_size) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    spells_.push_back(std::make_unique<DirectDamageSpell>(20, 1));
    if (spells_.size() < max_size_)
        spells_.push_back(std::make_unique<AreaDamageSpell>(20, 2));
}

void Hand::add_random_spell() {
    if (spells_.size() >= max_size_) {
        std::cout << "Рука заполнена, нельзя добавить новое заклинание.\n";
        return;
    }

    int choice = std::rand() % 2;
    if (choice == 0)
        spells_.push_back(std::make_unique<DirectDamageSpell>(20, 1));
    else
        spells_.push_back(std::make_unique<AreaDamageSpell>(20, 2));

    std::cout << "Игрок получил новое заклинание: " << spells_.back()->name() << "\n";
}

void Hand::use_spell(size_t index, Game& game, GameField& field, Player& player, int row, int col) {
    if (index >= spells_.size()) {
        std::cout << "Некорректный выбор заклинания.\n";
        return;
    }
    spells_[index]->use(game, field, player, row, col);
}

void Hand::show_spells() const {
    std::cout << "Заклинания в руке:\n";
    for (size_t i = 0; i < spells_.size(); ++i)
        std::cout << "  [" << i << "] " << spells_[i]->name() << "\n";
}
