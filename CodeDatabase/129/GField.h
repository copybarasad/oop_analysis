#pragma once
#include <ncurses.h>
#include <vector>
#include "../saving/Saving.h"
#include "../game/manager/Field.h"
#include "GCell.h"

class GField {
private:
    std::vector<std::vector<GCell>> gcells;
    int height;
    int width;
public:
    GField();
    GField(int h, int w);
    ~GField();

    void sync_field(Field field);

    void draw_border();
    void draw();

    Point get_sizes();

};
