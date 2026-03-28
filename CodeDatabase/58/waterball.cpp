#include "waterball.hpp"
#include "world.hpp"
#include "cell.hpp"

WaterBall::WaterBall(int range, float coef) {
    if (range < 1 || range > 5){
        range = BASE_RANGE_WATERBALL;
    }
    this->range = range;

    if (coef < 0 || coef > 1000){
        coef = BASE_COEF_WATERBALL;
    }
    this->coef = coef;
}


std::string WaterBall::get_name() {return name;};
float WaterBall::get_coef() {return coef;};
bool WaterBall::cast(World* world) {
    Player& player = world->get_player();
    Cell* target_cell = get_enemy_to_attack(world);

    if (!target_cell) return false;

    int dmg = get_damage(player);

    if(!world->magic_damage(dmg, target_cell)) return false;

    return true;
}

Cell* WaterBall::get_enemy_to_attack(World* world){
    Player& player = world->get_player();
    Cell* player_cell_ptr = world->get_cell(&player);
    int target_num = 0;

    std::map<int, Cell*> cells_enemies;

    for(int y = player_cell_ptr->get_y() - 3; y < player_cell_ptr->get_y() + 3; y++){
        for(int x = player_cell_ptr->get_x() - 3; x < player_cell_ptr->get_x() + 3; x++){
            if (!world->is_valid_coord(x, y)) continue;
            Entity* entity_in_cell_ptr = world->get_cell(x, y).get_entity();
            if (!entity_in_cell_ptr) continue;
            if (entity_in_cell_ptr->get_symbol() == 'P') continue;

            std::cout << "# " << target_num << " (" << x << ", " << y << ")" << std::endl;
            cells_enemies[target_num] = &world->get_cell(x, y);
            target_num++;
        }
    }
    if (cells_enemies.empty()){
        std::cout << "not avaible enemy to attack" << std::endl;
        return nullptr;
    }

    int target;
    std::cout << "enter your target: ";

    if(!(std::cin >> target)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "invalid num" << std::endl;
        return nullptr;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (cells_enemies.find(target) == cells_enemies.end()){
        std::cout << "not found" << std::endl;
        return nullptr;
    }

    return cells_enemies[target];
}

int WaterBall::get_range() {return range;};

int WaterBall::get_damage(Entity& entity) {
    return entity.get_base_damage() * coef;
}