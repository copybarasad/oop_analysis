#include "Human.h"
#include "GameArea.h"
#include <cstdlib>
#include <random>


bool Human::moving(std::string direction, GameArea& gamearea) {
    Coords new_coords = coords;

    if (direction == "top") new_coords.y--;
    else if (direction == "down") new_coords.y++;
    else if (direction == "right") new_coords.x++;
    else if (direction == "left") new_coords.x--;
    else return false;

    std::string obj_in_new_cell = gamearea.can_move_to(new_coords);

    if (obj_in_new_cell == "Edge")
        return false;

    if (stunned) {
        gamearea.move_human(coords, new_coords, this);
        return false;
    }

    if (obj_in_new_cell == "Empty" && gamearea.get_cell_type(new_coords) != CellType::BLOCKED)
        gamearea.move_human(coords, new_coords, this);
    else if (obj_in_new_cell == "Empty" && gamearea.get_cell_type(new_coords) == CellType::BLOCKED) {
        std::cout << "Cell is blocked. You cant move here.";
        return false;
    }
    else if (obj_in_new_cell != "Empty") {
        try {
            bool win = attack(new_coords, gamearea);
            if (!win) {
                return false;
            }
        }
        catch (const PlayerDyeException& e) {
            std::cout << e.what() << std::endl;
        }

    }
    set_coords(new_coords.x, new_coords.y);
    return true;
}

double Human::get_damage()
{
    return damage;
}

double Human::damage_calculation()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    double out_damage = damage;
    bool random_plus_or_minus = distrib(gen);
    if (random_plus_or_minus)
    {
        out_damage += rand() % ((int)damage - 5);
    }
    else
    {
        out_damage -= rand() % ((int)damage - 5);
    }
    return out_damage;
}

void Human::set_stunned(bool new_stunned)
{
    stunned = new_stunned;
}

bool Human::stunned_or_not()
{
    return stunned;
}