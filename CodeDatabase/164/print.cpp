#include "print.h" 

void Console_renderer::print_hero(Hero& hero, unsigned int win_points) override {
    std::cout << "hp:       " << hero.hp << "\ncan move: ";
    if(hero.can_move()){
        std::cout << "yes";
    }else{
        std::cout << "no";
    }
    std::cout << "\nattack:   ";
    if(hero.curent_attack == attack_type::melee){
        std::cout << "melee\n";
    }else{
        std::cout << "ranged\n";
    }
    std::cout << "money:    " << hero.money << "/" << win_points << '\n';
};

void Console_renderer::print_hand(Hand& hand) override {
    std::cout << "You have " << hand.count << " spells";
    if(hand.count == 0){
        std::cout << "." << std::endl;
        return;
    }

    std::cout << ":\n";
    for(unsigned int index = 0; index < hand.count; index++){
        std::cout << "\t" << index + 1 << " - ";
        spell_type type = hand.hand[index]->get_type();
        switch(type){
            case spell_type::direct_damage:
                std::cout << "direct damage\n";
                break;
            case spell_type::area_damage:
                std::cout << "area damage\n";
                break;
            case spell_type::trap:
                std::cout << "set trap\n";
                break;
            case spell_type::call_ally:
                std::cout << "call ally\n";
                break;
            case spell_type::upgrade:
                std::cout << "upgrade next spell\n";
                break;
        }
    }
};

void Console_renderer::print_field(Field& field) override {
    std::cout << "\033c";
    long long unsigned int len = 2 + 2 * field.width;
    std::string str{"   a b c d e f g h i j k l m n o p q r s t u v w x y z", len};
    std::cout << str << std::endl;
    for(unsigned int y = 0; y < field.height; y++){
        std::cout << std::setw(2) << y + 1;
        for(unsigned int x = 0; x < field.width; x++){
            char c;
            if(field.cells[y][x].occupied){
                switch(field.cells[y][x].occupied->get_type()){
                    case character_type::hero:
                        c = '&';
                        break;
                    case character_type::ally:
                        c = '@';
                        break;
                    case character_type::enemy:
                        c = '$';
                        break;
                    case character_type::tower:
                        c = 'T';
                        break;
                    case character_type::base:
                        c = '*';
                        break;
                }
            }else if(field.cells[y][x].trap){
                c = '#';
            }else{
                switch(field.cells[y][x].type){
                    case cell_type::usual:
                        c = '.';
                        break;
                    case cell_type::delay:
                        c = '~';
                        break;
                    case cell_type::impassable:
                        c = 'x';
                        break;
                }
            }
            std::cout << " " << c;
        }
        std::cout << std::endl;
    }
};

void Console_renderer::print_message(const std::string& message) override {
    std::cout << message << std::endl;
};
    
void Console_renderer::print_menu(std::vector<std::string>& options) override {
    for(size_t i = 0; i < options.size(); i++) {
        std::cout << "\t" << options[i] << std::endl;
    }
};