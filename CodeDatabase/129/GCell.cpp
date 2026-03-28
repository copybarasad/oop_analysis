#include "GCell.h"

GCell::GCell() : Cell() {}
GCell::GCell(int i_x0, int i_y0) : Cell() { //i_?0 - индекс ячейки
    x0 = i_x0 * 3; // в одной клетке по оси x 3 символа
    y0 = i_y0; //по оси у 1 символ
}
GCell::~GCell() {}


void GCell::draw() {
    if(is_there_player) {
        //mvaddch(тыкаем курсор сюда и по координатам(y, x)
        //вводим символ

        mvaddch(y0 + 1, x0 + 1, '^' | A_REVERSE);
        mvaddch(y0 + 1, x0 + 2, '3' | A_REVERSE);
        mvaddch(y0 + 1, x0 + 3, '^' | A_REVERSE);
    } 

    else if(is_there_enemy) {
        mvaddch(y0 + 1, x0 + 1, '>' | A_REVERSE);
        mvaddch(y0 + 1, x0 + 2, ':' | A_REVERSE);
        mvaddch(y0 + 1, x0 + 3, '(' | A_REVERSE);
    }

    else if(impassible){
        mvaddch(y0 + 1, x0 + 1, '#');
        mvaddch(y0 + 1, x0 + 2, '#');
        mvaddch(y0 + 1, x0 + 3, '#');
    }

    else if(trap){
        mvaddch(y0 + 1, x0 + 1, '_');
        mvaddch(y0 + 1, x0 + 2, '_');
        mvaddch(y0 + 1, x0 + 3, '_');
    }

    else if(enemy_tower){
        mvaddch(y0 + 1, x0 + 1, '>' | A_REVERSE);
        mvaddch(y0 + 1, x0 + 2, '_' | A_REVERSE);
        mvaddch(y0 + 1, x0 + 3, '<' | A_REVERSE);
    }


    else {
        mvaddch(y0 + 1, x0 + 1, ' ');
        mvaddch(y0 + 1, x0 + 2, ' ');
        mvaddch(y0 + 1, x0 + 3, ' ');
    }
}
