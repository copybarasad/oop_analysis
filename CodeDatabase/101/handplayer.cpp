#include "handplayer.h"
#include <QString>
#include <ctime>
#include <cstdlib>
#include "buttons.h"


handPlayer::handPlayer(int size_hand) {
    spellLight = new Lightning();
    spellTrap = new Trap();
    spellPoison = new Poison();
    spellTeammate = new Teammate();
    spellImprove = new Improve();
    max_size = size_hand;
    current_size = 1;
    list.push_back(spellLight);
    list.push_back(spellTrap);
    list.push_back(spellPoison);
    list.push_back(spellTeammate);
    list.push_back(spellImprove);

}

handPlayer::~handPlayer() {
    delete spellLight;
    delete spellTrap;
    delete spellPoison;
    delete spellTeammate;
    delete spellImprove;
}

void handPlayer::find_and_use(Visitor& visitor, std:: string str, int x, int y) /// ПОМЕНЯТЬ В КОДЕ POS НА X, Y
{
    int number_spell = std::stoi(str);
    spellCard* target_spell = list[number_spell];

    auto target_it = std::find(mySpell.begin(), mySpell.end(), target_spell);
    auto improve_it = std::find(mySpell.begin(), mySpell.end(), spellImprove);

        // Покупка карты
    if (y / DOT_WIDTH > FIELD_WIDTH - 1) {
        if (current_size < max_size && visitor.visitPlayer(target_spell->get_price())) {
            mySpell.push_back(target_spell);
            int count = std::count(mySpell.begin(), mySpell.end(), target_spell);
            visitor.visitButtons(number_spell, count);
            current_size++;
        }
    }

    else if(y / DOT_WIDTH < FIELD_WIDTH && improve_it != mySpell.end() && target_it != mySpell.end()){

        // Улучшаем карту
        (*target_it)->upgrade(spellImprove->get_level());
        (*target_it)->use_card(visitor, x, y);

        // Удаляем улучшаемую карту (если это не карта улучшения)
        if(target_spell != spellImprove) {
            mySpell.erase(target_it);
            current_size--;
        }

        // Удаляем карту улучшения
        mySpell.erase(improve_it);
        int count = std::count(mySpell.begin(), mySpell.end(), list[4]);
        visitor.visitButtons(4, count);
        int count2 = std::count(mySpell.begin(), mySpell.end(), target_spell);                            
        visitor.visitButtons(number_spell, count2);
        current_size --;
    }

    // Использование уже купленной карты
    else if (x / DOT_HEIGHT < FIELD_HEIGHT && y / DOT_WIDTH < FIELD_WIDTH && target_it != mySpell.end())
    {
        if((*target_it)->use_card(visitor, x, y)){
            mySpell.erase(target_it);
            int count = std::count(mySpell.begin(), mySpell.end(), target_spell);
            visitor.visitButtons(number_spell, count);
            current_size--;
        }
    }
}

Poison& handPlayer::get_card_poison()
{
    return *spellPoison;
}

Trap& handPlayer::get_trap_card()
{
    return *spellTrap;
}

Lightning& handPlayer::get_card_lightning()
{
    return *spellLight;
}

Improve& handPlayer::get_improve_card()
{
    return *spellImprove;
}

Teammate& handPlayer::get_teammate_card()
{
    return *spellTeammate;
}



void handPlayer::first_random_card(Visitor& visitor)
{
    std::srand(std::time(nullptr));
    int random_num = std::rand() % 5;
    mySpell.push_back(list[random_num]);
    visitor.visitButtons(random_num, 1);
    current_size = 1;
}

void handPlayer::remove_random_card(Visitor &visitor)
{
    if (mySpell.empty()) return;

    std::srand(std::time(nullptr));
    int random_num = std::rand() % mySpell.size();

    spellCard* cardToRemove = mySpell[random_num];

    mySpell.erase(mySpell.begin() + random_num);

    int count = std::count_if(mySpell.begin(), mySpell.end(),
                              [cardToRemove](spellCard* card) {
                                  return card == cardToRemove;
                              });

    visitor.visitButtons(random_num, count);
    current_size -= 1;
}

int handPlayer::get_current_size()
{
    return mySpell.size();
}

void handPlayer::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&max_size), sizeof(max_size));
    os.write(reinterpret_cast<const char*>(&current_size), sizeof(current_size));

    for (spellCard* card : list) {
        card->serialize(os);
    }
    size_t mySpellSize = mySpell.size();
    os.write(reinterpret_cast<const char*>(&mySpellSize), sizeof(mySpellSize));

    for (spellCard* card : mySpell) {
        int index = -1;
        for (size_t i = 0; i < list.size(); ++i) {
            if (list[i] == card) {
                index = static_cast<int>(i);
                break;
            }
        }
        os.write(reinterpret_cast<const char*>(&index), sizeof(index));
    }

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи handPlayer в поток");
    }
}

void handPlayer::deserialize(std::istream& is){
    is.read(reinterpret_cast<char*>(&max_size), sizeof(max_size));
    is.read(reinterpret_cast<char*>(&current_size), sizeof(current_size));

    for (spellCard* card : list) {
        card->deserialize(is);
    }

    size_t mySpellSize;
    is.read(reinterpret_cast<char*>(&mySpellSize), sizeof(mySpellSize));

    mySpell.clear();
    for (size_t i = 0; i < mySpellSize; ++i) {
        int index;
        is.read(reinterpret_cast<char*>(&index), sizeof(index));

        if (index >= 0 && index < static_cast<int>(list.size())) {
            mySpell.push_back(list[index]);
        }
    }

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения handPlayer из потока");
    }
}

void handPlayer::clear_hand() {
    for(auto card : mySpell) {
        delete card;
    }
    mySpell.clear();
    current_size = 0;
}
