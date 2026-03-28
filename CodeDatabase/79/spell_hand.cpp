#include "spell_hand.h"
#include "default_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include "player.h"
#include "place.h"
#include "enemy.h"
#include "tower.h"

SpellHand::SpellHand(){
    create_random_spell();
}

int SpellHand::add_spell(std::unique_ptr<IMagic_spell_card> spell){
    if (spells.size() >= max_size){
        return 0;
    }
    spells.push_back(std::move(spell));
    return 1;
}

int SpellHand::use_spell(int num, Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    if (num > getSize()){
        return 0;
    }
    IMagic_spell_card* spell = spells[num - 1].get();
    int res = spell->attack(player, target_x, target_y, place, enemies, tower, magic_tower);
    return res;
}


void SpellHand::create_random_spell(){
    srand(time(NULL)); 
    int spell_type = rand() % (6 - 1 + 1) + 1;
    switch(spell_type){
        case 1:
            add_spell(std::make_unique<area_spell>());
            break;
        case 2:
            add_spell(std::make_unique<Trap_spell>());
            break;
        default:
            add_spell(std::make_unique<default_spell>());
            break;
    }
}

int SpellHand::buy_spell(Player& player){
    if (is_full()){
        std::cout << "Все заклинания уже куплены" << std::endl;
        return 0;
    }
    bool flag_def_sp = true;
    bool flag_area_sp = true;
    bool flag_trap_sp = true;
    std::cout << "Ваши заклинания:" << std::endl;
    for (size_t i = 0; i < spells.size(); i++){
        IMagic_spell_card* spell = spells[i].get();
        int type = spell->getType();
        std::cout << spell->getName() << '\t' << spell->getDescription() << std::endl;
        switch (type){
            case 1:
                flag_def_sp = false;
                break;
            case 2:
                flag_area_sp = false;
                break;
            case 3:
                flag_trap_sp = false;
                break;
        }
    }
    int spelltype;
    std::cout << "Для покупки доступны заклинания:" << std::endl;
    if (flag_def_sp) std::cout << "1 - Заклинание прямого урона. Наносит прямой единоразовый урон\n Цена - 3 очка" << std::endl;
    if (flag_area_sp) std::cout << "2 - Огненный шар. Запускает огненный шар на клетку и наносит урон всем врагам в области поражения\nЦена - 8 очков" << std::endl;
    if (flag_trap_sp) std::cout << "3 - Ловушка. Создаёт на клетке шипы, когда враг наступает на них он получает единоразовый урон.\nЦена - 4 очка." << std::endl;
    std::cout << "Ваши очки: " << player.get_points() << std::endl;
    std::cout << "Ваш выбор: ";
    while (!(std::cin >> spelltype)){
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Введены некорректные данные" << std::endl;
        std::cout << "Введите номер заклинания: ";
                        }
    switch(spelltype){
        case(1):
            if(flag_def_sp && player.get_points() >= 3){
                player.increase_points(-3);
                add_spell(std::make_unique<default_spell>());
                return 1;
            }
            
        case(2):
            if(flag_area_sp && player.get_points() >= 8){
                player.increase_points(-8); 
                add_spell(std::make_unique<area_spell>());
                return 1;
            }
            
        case(3):
            if(flag_trap_sp && player.get_points() >= 4){
                player.increase_points(-4);
                add_spell(std::make_unique<Trap_spell>());
                return 1;
            }
        default:
            std::cout << "\nВыбрано неверное заклинание или не хватает очков, не удалось совершить покупку" << std::endl;
            return 0;
    }
    std::cout << "\nВыбрано неверное заклинание или не хватает очков, не удалось совершить покупку" << std::endl;
    return 0;
}

void SpellHand::getInfo(){
    for (int i = 0; i < spells.size(); i++){
        IMagic_spell_card* spell = spells[i].get();
        std::cout << "Номер заклинания - " << i + 1 << ". " << spell->getName() << ' ' << spell->getDescription() << std::endl;
    }   
}

int SpellHand::getSize() const{
    return spells.size();
}
int SpellHand::getMaxSize() const{
    return max_size;
}
const IMagic_spell_card* SpellHand::getSpell(int num) const{
    if (num >= getSize()) return nullptr;
    return spells[num].get();
}
bool SpellHand::is_full() const{
    return spells.size() >= max_size;
}
int SpellHand::remove_spell(int index) {
    if (index < 0 || index >= spells.size()) {
        return 0;
    }
    spells.erase(spells.begin() + index);
    return 1;
}

int SpellHand::remove_random_spell() {
    if (spells.empty()) {
        return 0;
    }
    srand(time(NULL));
    int random_index = rand() % spells.size();
    spells.erase(spells.begin() + random_index);
    return 1;
}

std::vector<int> SpellHand::get_spell_types() const {
    std::vector<int> types;
    for (const auto& spell : spells) {
        types.push_back(spell->getType());
    }
    return types;
}

void SpellHand::load_spells(const std::vector<int>& spell_types) {
    spells.clear();
    for (int type : spell_types) {
        switch(type) {
            case 1:
                add_spell(std::make_unique<default_spell>());
                break;
            case 2:
                add_spell(std::make_unique<area_spell>());
                break;
            case 3:
                add_spell(std::make_unique<Trap_spell>());
                break;
            default:
                add_spell(std::make_unique<default_spell>());
                break;
        }
    }
}

bool SpellHand::upgrade(int spell_ind, int value){
    if (spell_ind < 0 || spell_ind >= spells.size()) {
        return false;
    }
    
    IMagic_spell_card* spell = spells[spell_ind].get();
    int type = spell->getType();
    
    switch(type) {
        case 1:
            static_cast<default_spell*>(spell)->upgrade(value);
            break;
        case 2:
            static_cast<area_spell*>(spell)->upgrade(value);
            break;
        case 3:
            static_cast<Trap_spell*>(spell)->upgrade(value);
            break;
        default:
            return false;
    }
    return true;
}
