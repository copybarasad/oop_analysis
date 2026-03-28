#include "visitor.h"
#include "gamemodel.h"

Visitor::Visitor(GameModel* gm)
{
    this->gm = gm;
}

bool Visitor::visitEnemy(int nx, int ny, int damage, int radius, int width_attack)
{
    int x = nx / gm->get_cell_height();
    int y = ny / gm->get_cell_width();


    if(abs(gm->get_enemy().get_coord_x() - x) <= width_attack
        && abs(gm->get_enemy().get_coord_y() - y) <= width_attack
        && x <= gm->get_map().get_height() && y <= gm->get_map().get_width()
        && x >= 0 && y >= 0 && abs(gm->get_player().get_x() - x) <= radius
        && abs(gm->get_player().get_y() - y) <= radius)
    {
        gm->get_enemy().subtract_health(damage);
        gm->notify();
        return true;
    }
    return false;
}

bool Visitor::visitGame(int nx, int ny, int count)
{
    int x = nx / gm->get_cell_height();
    int y = ny / gm->get_cell_width();

    if(x < gm->get_map().get_height() && x >= 0 && y < gm->get_map().get_width() && y >= 0
        && gm->get_map().get_point(x, y).get_point_opt() != lock
        && gm->get_map().get_point(x, y).get_point_opt() != slow)
    {
        for(int i = 0; i < count; i++){
            gm->add_leon(x, y);
        }
        gm->notify();
        return true;
    }
    return false;
}

bool Visitor::visitMap(int nx, int ny)
{
    int x = nx / gm->get_cell_height();
    int y = ny / gm->get_cell_width();

    if(x < gm->get_map().get_height() && x >= 0 && y < gm->get_map().get_width() && y >= 0
        && gm->get_map().get_point(x, y).get_point_opt() != lock
        && gm->get_map().get_point(x, y).get_point_opt() != slow)
    {
        gm->get_map().set_point(x, y).set_option(trap);
        gm->notify();
        return true;
    }
    return false;
}

bool Visitor::visitMapForImprove(int nx, int ny)
{
    int x = nx / gm->get_cell_height();
    int y = ny / gm->get_cell_width();

    if(x < gm->get_map().get_height() && x >= 0 && y < gm->get_map().get_width() && y >= 0) return true;
    return false;
}

bool Visitor::visitPlayer(int price)
{
    if(gm->get_player().get_money() >= price){
        gm->get_player().spend_money(price);
        gm->notify();
        return true;
    }
    return false;
}

void Visitor::visitButtons(int number_spell, int count)
{
    gm->get_buttons().set_color(number_spell, count);
    gm->notify();
}
