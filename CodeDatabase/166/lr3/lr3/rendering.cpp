#include "rendering.hpp"

#include <algorithm>
#include <iostream>

#include "terminal.hpp"
#include "world.hpp"

namespace {
char base_cell_symbol(const Cell& c) {
    switch (c.getType()) {
        case CellType::Empty: return '.';
        case CellType::Wall:  return '#';
        case CellType::Slow:  return '~';
    }
    return '?';
}

char char_for_cell(const World& w, const Position& p) {
    if (w.hero.getPosition() == p && !w.hero.isDead()) return 'H';
    if (is_enemy_at(w, p)) return 'E';
    if (is_spawner_at(w, p)) return 'S';
    if (trap_index_at(w, p)  >= 0) return 'T';
    if (tower_index_at(w, p) >= 0) return 'B';
    if (is_ally_at(w, p)) return 'A';
    return base_cell_symbol(w.field.at(p.getX(), p.getY()));
}
} // namespace

void draw_cell(const World& w, const Position& p) {
    if (!w.field.inBounds(p.getX(),p.getY())) return;
    int sx = ORIGIN_X + p.getX()*2, sy = ORIGIN_Y + p.getY();
    cursor_move(sy, sx);
    std::cout << char_for_cell(w, p) << ' ';
}

void redraw_all_cells(const World& w) {
    const int W = w.field.getWidth(), H = w.field.getHeight();
    for (int y=0;y<H;++y) for (int x=0;x<W;++x) draw_cell(w, {x,y});
}

void draw_panel(const World& w, int start_col) {
    int y = 1, x = start_col;
    int eDmgLvl = 1 + std::max(1, w.level);

    cursor_move(y++, x); std::cout << "level: " << w.level << "     ";
    cursor_move(y++, x); std::cout << "hp:    " << w.hero.getHp() << "     ";
    cursor_move(y++, x); std::cout << "dmg:   " << w.hero.getDmg() << " E=" << eDmgLvl << " B=" << std::max(1, w.level);

    int eMax = max_enemy_hp_for_level(w.level);
    int sMax = max_spawner_hp_for_level(w.level);
    cursor_move(y++, x); std::cout << "max HP:E=" << eMax << "  S=" << sMax << "     ";

    cursor_move(y++, x); std::cout << "враги: " << alive_enemies(w) << "    ";
    cursor_move(y++, x); std::cout << "убито: " << w.kills << "    ";
    cursor_move(y++, x); std::cout << "boosts: x" << w.boosts.stacks << "     ";

    y++;
    cursor_move(y++, x);
    std::cout << "карты (1.." << (int)w.hand.capacity() << "):";
    for (std::size_t i=0;i<w.hand.capacity();++i) {
        cursor_move(y++, x);
        if (i < w.hand.size()) std::cout << "[" << (i+1) << "] " << w.hand.cardName(i) << "            ";
        else                    std::cout << "[" << (i+1) << "] (пусто)              ";
    }
    cursor_move(y++, x); std::cout << "wasd:ход e:ближняя r:дальняя";
    cursor_move(y++, x); std::cout << "1.." << (int)w.hand.capacity() << ":карта c:отмена";
    cursor_move(y++, x); std::cout << "q:выход";
}

void draw_world_initial(const World& w) {
    const int W = w.field.getWidth(), H = w.field.getHeight();
    std::cout << "\x1b[2J\x1b[H";
    for (int x=0;x<=W*2+1;++x) { cursor_move(ORIGIN_Y-1, ORIGIN_X-1+x); std::cout << (x==0||x==W*2+1?'+':'-'); }
    for (int y=0;y<H;++y) {
        cursor_move(ORIGIN_Y+y, ORIGIN_X-1);     std::cout << '|';
        cursor_move(ORIGIN_Y+y, ORIGIN_X+W*2);   std::cout << '|';
    }
    for (int x=0;x<=W*2+1;++x) { cursor_move(ORIGIN_Y+H, ORIGIN_X-1+x); std::cout << (x==0||x==W*2+1?'+':'-'); }
    redraw_all_cells(w);
    int panel_col = ORIGIN_X + W*2 + 4;
    draw_panel(w, panel_col);
    std::cout.flush();
}

void draw_aim_cursor_single(const Position& p) {
    int sx = ORIGIN_X + p.getX()*2, sy = ORIGIN_Y + p.getY();
    cursor_move(sy, sx);
    std::cout << "\x1b[7m" << 'X' << ' ' << "\x1b[0m";
}

bool inBoundsSquare(const Field& f, const Position& tl, int side) {
    for (int dy=0; dy<side; ++dy)
        for (int dx=0; dx<side; ++dx)
            if (!f.inBounds(tl.getX()+dx, tl.getY()+dy)) return false;
    return true;
}

void draw_square_overlay(const World& w, const Position& tl, int side) {
    if (!inBoundsSquare(w.field, tl, side)) return;
    for (int dy=0; dy<side; ++dy)
        for (int dx=0; dx<side; ++dx) {
            Position p{tl.getX()+dx, tl.getY()+dy};
            int sx = ORIGIN_X + p.getX()*2, sy = ORIGIN_Y + p.getY();
            cursor_move(sy, sx);
            char ch = char_for_cell(w, p);
            std::cout << "\x1b[7m" << ch << ' ' << "\x1b[0m";
        }
}

