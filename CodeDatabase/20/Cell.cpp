#include "../include/Cell.h"



void Cell::set_occupied(bool is){ occupied = is; }
void Cell::set_passable(bool is){ passable = is; }
void Cell::set_occupied_by(Entity* et){occupied_by = et;}
void Cell::set_x(int x){ x_pos = x; }
void Cell::set_y(int y){ y_pos = y; }
void Cell::set_traped(bool is){traped = is;}

bool Cell::is_occupied(){ return occupied; }

Entity* Cell::is_occupied_by(){return occupied_by;}


bool Cell::is_passable(){ return passable; }
bool Cell::is_traped(){return traped;   }


int Cell::x_position(){ return x_pos; }
int Cell::y_position(){ return y_pos; }
