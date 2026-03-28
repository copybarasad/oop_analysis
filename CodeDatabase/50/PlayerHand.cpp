#include "PlayerHand.h"

PlayerHand::PlayerHand(int capacity) 
        : handCapacity(capacity), 
          fireballProt(std::make_unique<FireballSpell>(10,3,2)),
          trapProt(std::make_unique<TrapSpell>(10,2,1)),
          explosionProt(std::make_unique<ExplosionSpell>(8,2,3)){
    srand(static_cast<unsigned>(time(nullptr)));
    
    addRandomSpell();
    std::cout << "Ваша рука создана. Добавлено начальное заклинание!\n";
}

PlayerHand::PlayerHand(PlayerHand&& other) noexcept
    : handCapacity(other.handCapacity),
      spells(std::move(other.spells)),
      fireballProt(std::move(other.fireballProt)),
      trapProt(std::move(other.trapProt)),
      explosionProt(std::move(other.explosionProt))
{
    other.handCapacity = 0;
}

PlayerHand& PlayerHand::operator=(PlayerHand&& other) noexcept {
    if (this == &other) return *this;

    // очистим старые заклинания!
    for (auto* s : spells) delete s;
    spells.clear();

    handCapacity = other.handCapacity;
    spells = std::move(other.spells);

    fireballProt = std::move(other.fireballProt);
    trapProt = std::move(other.trapProt);
    explosionProt = std::move(other.explosionProt);

    other.handCapacity = 0;

    return *this;
}

void PlayerHand::showSpellList() const {
    std::cout << "\n=== Заклинания игрока ===\n";
    if (spells.empty()) {
        std::cout << "Нет заклинаний.\n";
        return;
    }

    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ") " << spells[i]->getName()
                  << " | урон: " << (spells[i]->getDamage())
                  << " | мана: " << spells[i]->getManaCost() << "\n";
    }
}

ISpell* PlayerHand::getSpell(int index) const {
    if (index < 0 || index >= (int)spells.size()) {
        std::cout << "Некорректный индекс.\n";
        return nullptr;
    }
    return spells[index];
}

bool PlayerHand::removeSpell(int index) {
    if (index < 0 || index >= (int)spells.size()) {
        std::cout << "Некорректный индекс для удаления.\n";
        return false;
    }
    delete spells[index]; // освобождаем память
    spells.erase(spells.begin() + index);
    std::cout << "Заклинание было использовано и удалено из руки.\n";
    return true;
}

PlayerHand::~PlayerHand()  {
    for (auto s : spells) delete s;
    spells.clear();
}

void PlayerHand::addRandomSpell() {
    if (isFull()) {
        std::cout << "Рука заполнена, новое заклинание не помещается.\n";
        return;
    }

    int random = rand() % 3; // пока 2 типа
    ISpell* newSpell = nullptr;


    if (random == 0){
        newSpell = fireballProt->clone();
    }else if(random == 1){
        newSpell = explosionProt->clone();
    }else if (random == 2){
        newSpell = trapProt->clone();
    }
        

    addSpell(newSpell);
}

bool PlayerHand::addSpell(ISpell* spell) {
    if (!spell) return false;
    if (isFull()) {
        std::cout << "Рука переполнена! Нельзя добавить заклинание.\n";
        delete spell; // чтобы не утекло
        return false;
    }
    spells.push_back(spell);
    std::cout << "Добавлено заклинание: " << spell->getName() << "\n";
    return true;
}

void PlayerHand::removeRandomHalf() {
    int count = getCount();
    int toRemove = count / 2;
    for (int i = 0; i < toRemove && getCount() > 0; ++i) {
        int index = rand() % getCount();
        removeSpell(index);
    }
}


//save and load
void PlayerHand::save(std::ostream& out) const {
    out << handCapacity << ' ' << spells.size() << '\n';
    for (auto* spell : spells) {
        out << spell->getName() << " " << spell->getDamage() << '\n';
    }
}

void PlayerHand::load(std::istream& in) {
    for (auto* s : spells) {
        delete s;
    }
    spells.clear();

    int capacity = 0;
    std::size_t count = 0;
    in >> capacity >> count;
    if (!in || capacity <= 0) {
        throw InvalidSaveDataException("Ошибка чтения руки игрока");
    }
    handCapacity = capacity;

    std::string dummy;
    std::getline(in, dummy); // добиваем строку после count

    for (std::size_t i = 0; i < count; ++i) {
        std::string name;
        double dmg;
        in >> name >> dmg;
        if (!in) {
            throw InvalidSaveDataException("Ошибка чтения имени заклинания");
        }

        ISpell* spell = nullptr;
        if (name == "FireBall") {
            fireballProt->setDamage(dmg);
            spell = fireballProt->clone();
        } else if (name == "TrapSpell") {
            trapProt->setDamage(dmg);
            spell = trapProt->clone();
        } else if (name == "Explosion") {
            explosionProt->setDamage(dmg);
            spell = explosionProt->clone();
        }

        if (spell) {
            addSpell(spell);
        }
    }
}