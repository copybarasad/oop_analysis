#include "GField.h"

GField::GField() {}
GField::GField(int h, int w) {
    height = h;  
    width = w;

    gcells.resize(height);
    for(int i = 0; i < height; i++) {
        gcells[i].resize(width);  
        for(int j = 0; j < width; j++) {
            gcells[i][j] = GCell(i, j);  
        }
    }
}
GField::~GField() {}


void GField::sync_field(Field field) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            gcells[i][j].set_player(field.get_cell(i, j).is_player());
            gcells[i][j].set_enemy(field.get_cell(i, j).is_enemy());
            gcells[i][j].set_impassible(field.get_cell(i, j).is_impassible());
            gcells[i][j].set_trap(field.get_cell(i,j).is_trap());
            gcells[i][j].set_enemy_tower(field.get_cell(i,j).is_enemy_tower());
        }
    }
}


void GField::draw_border() {
    int border_width = width * 3 + 2;   // ширина: ячейки * 3 + границы
    int border_height = height + 2;     // высота: ячейки + границы

    mvaddch(0, 0, ACS_ULCORNER);                    // ┌
    mvaddch(0, border_width - 1, ACS_URCORNER);     // ┐
    mvaddch(border_height - 1, 0, ACS_LLCORNER);    // └
    mvaddch(border_height - 1, border_width - 1, ACS_LRCORNER); // ┘
        
    for(int i = 1; i < border_width - 1; i++) {
        mvaddch(0, i, ACS_HLINE);                   // ─
        mvaddch(border_height - 1, i, ACS_HLINE);   // ─
    }

    for(int j = 1; j < border_height - 1; j++) {
        mvaddch(j, 0, ACS_VLINE);                   // │
        mvaddch(j, border_width - 1, ACS_VLINE);    // │
    }
}

void GField::draw() {
    draw_border();
    for(int i = 0; i < height; i++) {      
        for(int j = 0; j < width; j++) {  
            gcells[i][j].draw();
        }
    }
}


Point GField::get_sizes(){
    return Point(height, width);
}
