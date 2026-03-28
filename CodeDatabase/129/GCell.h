#pragma once
#include <ncurses.h>
#include "../game/manager/Cell.h"
#include "../saving/Saving.h"

class GCell : public Cell{
private:
    int x0;
    int y0;
public:
    GCell(); 
    GCell(int i_x0, int i_y0);
    ~GCell();

    void draw();
};


