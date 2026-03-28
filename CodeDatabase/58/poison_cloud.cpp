#include "poison_cloud.hpp"
#include "player.hpp"
#include "world.hpp"

std::string PoisonCloud::get_name() {return name;};
float PoisonCloud::get_coef() {return coef;};
int PoisonCloud::get_area() {return area;};
bool PoisonCloud::cast(World* world) {
    Player& player = world->get_player();

    std::cout << "enter x, y separated by space: ";
    int x, y;

    if(!(std::cin >> x >> y)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "error enter" << std::endl;
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(!world->is_valid_coord(x, y)){
        std::cout << "x or y out of range" << std::endl;
        return false;
    }

    for(int dy = y; dy < y + area; dy++){
        for(int dx = x; dx < x + area; dx++){
            if(!world->is_valid_coord(dx, dy)) continue;

            Cell& cell = world->get_cell(dx, dy);
            if (!cell.get_entity()) continue;
            
            if(!world->magic_damage(get_damage(player), &cell)) return false;
        }
    }

    return true;
}

int PoisonCloud::get_damage(Entity& entity) {
    return entity.get_base_damage() * coef;
}