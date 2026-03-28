#include "visual_terminal.hpp"

void VisualTerminal::draw_game(Map* map, Player* player, Hand* hand) {
    map->print();
    std::cout << "Hp: " << player->getHp() << "\n";
    hand->print_hand();
}