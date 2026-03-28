#include "banana_trap.hpp"
#include "world.hpp"
#include "player.hpp"

std::string BananaTrap::get_name() {return name;};

bool BananaTrap::cast(World* world){
    std::cout << "enter x, y separated by space: ";
    int x, y;

    if(!(std::cin >> x >> y)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "error enter" << std::endl;
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(!can_place(world, x, y)) return false;

    Cell& cell = world->get_cell(x, y);
    cell.set_type_cell(TypeCell::TRAP);

    return true;
}

bool BananaTrap::can_place(World* world, int x, int y){
    if (!world->is_valid_coord(x, y)){
        std::cout << "x or y out of range" << std::endl;
        return false;
    }
    Cell& cell = world->get_cell(x, y);

    if(cell.get_type_cell() != TypeCell::NORMAL){
        std::cout << "you can set a trap ONLY normal cell" << std::endl;
        return false;
    }
    if(cell.get_entity()){
        std::cout << "you can set a trap ONLY an unoccupied cell" << std::endl;
        return false;
    }

    return true;
}