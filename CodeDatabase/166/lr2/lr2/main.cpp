// main.cpp
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <cctype>
#include <set>
#include <cmath>
#include <unistd.h>
#include <termios.h>

#include "position.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"
#include "setup.hpp"

#include "hand.hpp"
#include "traps.hpp"
#include "towers.hpp"
#include "ally.hpp"

#include "spell.hpp"
#include "spells/direct_damage_spell.hpp"
#include "spells/aoe_damage_spell.hpp"
#include "spells/trap_spell.hpp"
#include "spells/summon_spell.hpp"
#include "spells/enhance_spell.hpp"

// ------------------ терминал (raw + ANSI) ------------------

class TerminalRaw {
public:

    termios old{};
    bool active{false};
    void enable() {
        if (active) return;
        tcgetattr(STDIN_FILENO, &old);
        termios raw = old;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN]  = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        std::cout << "\x1b[?25l\x1b[?7l\x1b[2J\x1b[H" << std::flush;
        active = true;
    }
    void disable() {
        if (!active) return;
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        std::cout << "\x1b[?7h\x1b[?25h\x1b[0m\n" << std::flush;
        active = false;
    }
    ~TerminalRaw() { disable(); }
};

 int read_key_immediate() {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;
    if (c == 0x1B) {
        unsigned char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return 27;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return 27;
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 'w';
                case 'B': return 's';
                case 'C': return 'd';
                case 'D': return 'a';
            }
        }
        return 0;
    }
    return std::tolower(c);
}

// ------------------ мир ------------------

class World {
public:

    Field field;
    Hero hero;
    std::vector<Enemy> enemies;
    std::vector<Spawner> spawners;
    std::vector<Trap> traps;
    std::vector<Tower> towers;
    std::vector<Ally> allies;

    int kills{0};
    int awardsGiven{0};
    Hand hand;
    BoostState boosts;

    World(Field f, Hero h, std::vector<Enemy> es, std::vector<Spawner> ss, std::size_t handCap)
        : field(std::move(f)), hero(std::move(h)), enemies(std::move(es)), spawners(std::move(ss)), hand(handCap) {}
};

// ------------------ вспомогалки мира ------------------

 const int ORIGIN_Y = 1;
 const int ORIGIN_X = 2;

 inline void cursor_move(int y, int x) { std::cout << "\x1b[" << y << ";" << x << "H"; }

 bool is_enemy_at(const World& w, const Position& p, int* idx=nullptr) {
    for (int i = 0; i < (int)w.enemies.size(); ++i) {
        if (!w.enemies[i].isDead() && w.enemies[i].getPosition() == p) {
            if (idx) {
                *idx = i;
            }
            return true;
        }
    }
    return false;
}

 bool is_spawner_at(const World& w, const Position& p) {
    for (const auto& s : w.spawners) if (s.getPos() == p) return true;
    return false;
}

 int trap_index_at(const World& w, const Position& p) {
    for (int i = 0; i < (int)w.traps.size(); ++i) if (w.traps[i].getPos() == p) return i;
    return -1;
}

 int tower_index_at(const World& w, const Position& p) {
    for (int i = 0; i < (int)w.towers.size(); ++i) if (w.towers[i].getPos() == p) return i;
    return -1;
}

 bool is_ally_at(const World& w, const Position& p) {
    for (const auto& a : w.allies) if (!a.isDead() && a.getPos() == p) return true;
    return false;
}

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
 int alive_enemies(const World& w) {
    return (int)std::count_if(w.enemies.begin(), w.enemies.end(), [](const Enemy& e){return !e.isDead();});
}
 void maybe_award_card(World& w) {
    while (w.kills / 3 > w.awardsGiven) {
        if (!w.hand.drawRandom()) break;
        w.awardsGiven += 1;
    }
}
 void draw_panel(const World& w, int start_col) {
    int y = 1, x = start_col;
    cursor_move(y, x);   std::cout << "hp: " << w.hero.getHp() << "     ";
    cursor_move(y+1, x); std::cout << "враги: " << alive_enemies(w) << "    ";
    cursor_move(y+2, x); std::cout << "убито: " << w.kills << "    ";
    cursor_move(y+3, x); std::cout << "boosts: x" << w.boosts.stacks << "     ";

    cursor_move(y+5, x);
    std::cout << "карты (1.." << (int)w.hand.capacity() << "):";
    for (std::size_t i=0;i<w.hand.capacity();++i) {
        cursor_move(y+6+(int)i, x);
        if (i < w.hand.size()) std::cout << "[" << (i+1) << "] " << w.hand.cardName(i) << "            ";
        else                    std::cout << "[" << (i+1) << "] (пусто)              ";
    }
    cursor_move(y+7+(int)w.hand.capacity(), x);
    std::cout << "wasd:ход e:ближняя r:дальняя";
    cursor_move(y+8+(int)w.hand.capacity(), x);
    std::cout <<  "1.." << (int)w.hand.capacity()
              << ":карта c:отмена";
    cursor_move(y+9+(int)w.hand.capacity(), x);
    std::cout << "q:выход";
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

// ------------------ коллизии ------------------

 bool cell_free_for_enemy(const World& w, const Position& p) {
    if (!w.field.inBounds(p.getX(),p.getY())) return false;
    if (!w.field.passable(p.getX(),p.getY())) return false;
    if (w.hero.getPosition() == p) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    if (trap_index_at(w,p)  >= 0) return false;
    if (tower_index_at(w,p) >= 0) return false;
    if (is_ally_at(w,p)) return false;
    return true;
}
 bool cell_free_for_hero(const World& w, const Position& p) {
    if (!w.field.inBounds(p.getX(),p.getY())) return false;
    if (!w.field.passable(p.getX(),p.getY())) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    if (trap_index_at(w,p)  >= 0) return false;
    if (tower_index_at(w,p) >= 0) return false;
    if (is_ally_at(w,p)) return false;
    return true;
}
 bool cell_free_for_ally(const World& w, const Position& p) {
    if (!w.field.inBounds(p.getX(),p.getY())) return false;
    if (!w.field.passable(p.getX(),p.getY())) return false;
    if (w.hero.getPosition() == p) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    if (trap_index_at(w,p)  >= 0) return false;
    if (tower_index_at(w,p) >= 0) return false;
    if (is_ally_at(w,p)) return false;
    return true;
}

// ------------------ атаки героя ------------------

 bool melee_attack(World& w, std::vector<Position>& to_redraw) {
     const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
    for (const auto& d : dirs) {
        Position p = w.hero.getPosition() + d;
        int idx = -1;
        if (is_enemy_at(w, p, &idx)) {
            w.enemies[idx].takeDamage(w.hero.getDmg());
            if (w.enemies[idx].isDead()) { w.kills += 1; to_redraw.push_back(p); }
            return true;
        }
    }
    return false;
}
 bool ranged_attack(World& w, std::vector<Position>& to_redraw) {
     const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
    int rdmg = std::max(1, w.hero.getDmg() - w.hero.getDmg()/2);
    for (const auto& d : dirs) {
        Position p = w.hero.getPosition();
        for (int step=0; step<3; ++step) {
            p = p + d;
            if (!w.field.inBounds(p.getX(),p.getY()) || !w.field.passable(p.getX(),p.getY())) break;
            int idx=-1;
            if (is_enemy_at(w, p, &idx)) {
                w.enemies[idx].takeDamage(rdmg);
                if (w.enemies[idx].isDead()) { w.kills += 1; to_redraw.push_back(p); }
                return true;
            }
        }
    }
    return false;
}

// ------------------ ходы существ ------------------

 void enemies_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& e : w.enemies) {
        if (e.isDead()) continue;
        Position from = e.getPosition();

        Position step = Enemy::chooseStep(e.getPosition(), w.hero.getPosition());
        Position dst{from.getX() + step.getX(), from.getY() + step.getY()};
        if (!w.field.inBounds(dst.getX(),dst.getY())) continue;

        if (dst == w.hero.getPosition()) {
            w.hero.takeDamage(e.getDmg());
            continue;
        }

        // враг атакует союзника
        int allyIdx = -1;
        for (int i=0; i<(int)w.allies.size(); ++i) {
            if (!w.allies[i].isDead() && w.allies[i].getPos() == dst) { allyIdx = i; break; }
        }
        if (allyIdx >= 0) {
            w.allies[allyIdx].takeDamage(e.getDmg());
            to_redraw.push_back(dst);
            if (w.allies[allyIdx].isDead()) {
                e.setPos(dst);
                to_redraw.push_back(from);
            }
            continue;
        }

        int trapIdx = trap_index_at(w, dst);
        if (trapIdx >= 0) {
            int alive_before = alive_enemies(w);
            e.takeDamage(w.traps[trapIdx].getDamage());
            w.traps.erase(w.traps.begin() + trapIdx);
            to_redraw.push_back(dst);

            if (!e.isDead()) {
                if (!cell_free_for_enemy(w, dst)) continue;
                e.setPos(dst);
                to_redraw.push_back(from);
            } else {
                int alive_after = alive_enemies(w);
                if (alive_after < alive_before) w.kills += (alive_before - alive_after);
                to_redraw.push_back(from);
            }
            continue;
        }

        if (!cell_free_for_enemy(w, dst)) continue;
        e.setPos(dst);
        to_redraw.push_back(from);
        to_redraw.push_back(dst);
    }
}

 void spawners_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& s : w.spawners) {
        auto spawn_pos = s.onTurn([&](const Position& p){ return cell_free_for_enemy(w,p); });
        if (spawn_pos) { w.enemies.emplace_back(Enemy{6,2,*spawn_pos}); to_redraw.push_back(*spawn_pos); }
    }
}

 
void towers_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& t : w.towers) {
        t.onTurn();
        (void)t.tryAttack(w.hero, &to_redraw);
    }
}


 void allies_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& a : w.allies) {
        if (a.isDead()) continue;

        int ei = a.closestEnemyIndex(w.enemies);
        if (ei < 0) continue;

        Position epos = w.enemies[ei].getPosition();
        int dist = Ally::manh(a.getPos(), epos);

        if (dist == 1) {
            w.enemies[ei].takeDamage(a.getDmg());
            if (w.enemies[ei].isDead()) { ++w.kills; to_redraw.push_back(epos); }
            continue;
        }

        Position step = Ally::stepToward(a.getPos(), epos);
        Position dst{a.getPos().getX() + step.getX(), a.getPos().getY() + step.getY()};
        if (cell_free_for_ally(w, dst)) {
            Position from = a.getPos();
            a.setPos(dst);
            to_redraw.push_back(from);
            to_redraw.push_back(dst);
        }
    }
}

// ------------------ прицел / визуал ------------------

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

// ------------------ «почтовые ящики» для спеллов ------------------

namespace {
    std::vector<Position> g_summon_positions;
    int g_summon_hp = 5, g_summon_dmg = 2;
    std::vector<std::pair<Position,int>> g_traps_created;
}

void summon_register_created(const std::vector<Position>& ps, int hp, int dmg) {
    g_summon_positions = ps;
    g_summon_hp = hp;
    g_summon_dmg = dmg;
}

void trap_register_created(const Position& pos, int dmg) {
    g_traps_created.emplace_back(pos, dmg);
}


void flush_created_entities(World& w) {
    for (const auto& p : g_summon_positions) {
        w.allies.emplace_back(Ally{p, g_summon_hp, g_summon_dmg});
        draw_cell(w, p);
    }
    g_summon_positions.clear();

    for (const auto& t : g_traps_created) {
        w.traps.emplace_back(Trap{t.first, t.second});
        draw_cell(w, t.first);
    }
    g_traps_created.clear();
}

// ------------------ main ------------------

int main() {
    GameParams params = prompt_game_params();

    Field fld{std::max(10, std::min(params.getWidth(), 25)),
              std::max(10, std::min(params.getHeight(), 25))};
    Hero hero{std::max(1, params.heroHp), std::max(1, params.heroDmg), Position{0,0}};
    std::vector<Spawner> spawners;

    if (!build_world_from_params(params, fld, hero, spawners)) {
        std::cout << "не удалось построить мир (проверьте ввод)\n";
        return 0;
    }

    std::vector<Enemy> enemies;
    std::size_t handCap = params.handSize > 0 ? (std::size_t)params.handSize : (std::size_t)5;
    World world{std::move(fld), std::move(hero), std::move(enemies), std::move(spawners), handCap};

    {
        auto cell_free_for_building = [&](const Position& p)->bool {
            if (!world.field.inBounds(p.getX(), p.getY())) return false;
            if (!world.field.passable(p.getX(), p.getY())) return false;
            if (world.hero.getPosition() == p) return false;
            if (is_enemy_at(world, p)) return false;
            if (is_spawner_at(world, p)) return false;
            if (trap_index_at(world, p)  >= 0) return false;
            if (tower_index_at(world, p) >= 0) return false;
            if (is_ally_at(world, p)) return false;
            return true;
        };
        const Position dirs[4] = { {1,0},{-1,0},{0,1},{0,-1} };
        for (const auto& s : world.spawners) {
            for (const auto& d : dirs) {
                Position p{s.getPos().getX()+d.getX(), s.getPos().getY()+d.getY()};
                if (cell_free_for_building(p)) {
                    world.towers.emplace_back(Tower{p, 1, 2});
                    break;
                }
            }
        }
    }

    // стартовая карта
    world.hand.drawRandom();

    TerminalRaw term; term.enable();
    draw_world_initial(world);

    auto manh = [](const Position& a, const Position& b){ return std::abs(a.getX()-b.getX())+std::abs(a.getY()-b.getY()); };

    class Aim {
    public:

        bool active{false};
        TargetMode mode{TargetMode::None};
        int radius{0};
        int areaSide{2}; 
        Position origin{0,0};
        Position cur{0,0};
        int cardIndex{-1};
    } aim;

    bool quit = false;

    while (!world.hero.isDead() && !quit) {
        int panel_col = ORIGIN_X + world.field.getWidth()*2 + 4;
        draw_panel(world, panel_col);

        world.hero.beginTurn();
        if (world.hero.skippedThisTurn()) {
            cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
            std::cout << "замедление: пропуск хода      " << std::flush;
            world.hero.endTurn();
        } else {
            bool acted = false;
            while (!acted) {
                int k = read_key_immediate();
                if (!k) continue;
                if (k == 'q') { quit = true; break; }

                std::vector<Position> to_redraw;

                // режим прицеливания по новому интерфейсу
                if (aim.active) {
                    Position prev = aim.cur;

                    if (aim.mode == TargetMode::Adjacent) {
                        const Position h = world.hero.getPosition();
                        Position cand = aim.cur;
                        if (k=='w') cand = Position{h.getX(),   h.getY()-1};
                        if (k=='s') cand = Position{h.getX(),   h.getY()+1};
                        if (k=='a') cand = Position{h.getX()-1, h.getY()};
                        if (k=='d') cand = Position{h.getX()+1, h.getY()};
                        bool ok = world.field.inBounds(cand.getX(),cand.getY()) && world.field.passable(cand.getX(),cand.getY())
                               && !is_enemy_at(world, cand) && !is_spawner_at(world, cand)
                               && trap_index_at(world, cand) < 0 && tower_index_at(world, cand) < 0
                               && !is_ally_at(world, cand);
                        if (ok) aim.cur = cand;
                    } else if (aim.mode == TargetMode::Cell) {
                        if (k=='w') aim.cur.setY(aim.cur.getY() - 1);
                        if (k=='s') aim.cur.setY(aim.cur.getY() + 1);
                        if (k=='a') aim.cur.setX(aim.cur.getX() - 1);
                        if (k=='d') aim.cur.setX(aim.cur.getX() + 1);
                        if (!world.field.inBounds(aim.cur.getX(),aim.cur.getY()) || manh(aim.cur, aim.origin) > aim.radius)
                            aim.cur = prev;
                    } else if (aim.mode == TargetMode::Area2x2) {
                        if (k=='w') aim.cur.setY(aim.cur.getY() - 1);
                        if (k=='s') aim.cur.setY(aim.cur.getY() + 1);
                        if (k=='a') aim.cur.setX(aim.cur.getX() - 1);
                        if (k=='d') aim.cur.setX(aim.cur.getX() + 1);
                        if (!inBoundsSquare(world.field, aim.cur, aim.areaSide) || manh(aim.cur, aim.origin) > aim.radius)
                            aim.cur = prev;
                    }

                    if (k == 'c') {
                        // отмена прицела
                        if (aim.mode == TargetMode::Area2x2) {
                            if (inBoundsSquare(world.field, prev, aim.areaSide)) {
                                for (int dy=0; dy<aim.areaSide; ++dy)
                                    for (int dx=0; dx<aim.areaSide; ++dx)
                                        draw_cell(world, {prev.getX()+dx, prev.getY()+dy});
                            }
                        } else {
                            draw_cell(world, prev);
                        }
                        aim = {};
                        draw_panel(world, panel_col);
                        std::cout.flush();
                        continue;
                    }

                    if (k == 'e') {
                        if (!world.hand.has((std::size_t)aim.cardIndex)) {
                            // не должно случиться, но на всякий — сброс
                            aim = {}; continue;
                        }
                        Spell* s = world.hand.get((std::size_t)aim.cardIndex);
                        bool ok = s->cast(world.hero, world.field, world.enemies, world.spawners, world.boosts, aim.cur);
                        if (ok) {
                            world.hand.remove((std::size_t)aim.cardIndex);
                            world.hero.consume();
                            acted = true;

                            // добавить созданные сущности (призыв/ловушка/башня)
                            flush_created_entities(world);

                            // перерисовать затронутое
                            if (aim.mode == TargetMode::Area2x2) {
                                if (inBoundsSquare(world.field, aim.cur, aim.areaSide)) {
                                    for (int dy=0; dy<aim.areaSide; ++dy)
                                        for (int dx=0; dx<aim.areaSide; ++dx)
                                            draw_cell(world, {aim.cur.getX()+dx, aim.cur.getY()+dy});
                                } else {
                                    redraw_all_cells(world);
                                }
                            } else {
                                draw_cell(world, aim.cur);
                            }

                            // проверить убийства (вдруг заклинание убило кого-то)
                            int before = 0, after = 0; // грубо перещёлкиваем
                            // (быстро для простоты)
                            // лучше сразу пересчитать
                            (void)before; (void)after;
                            maybe_award_card(world);
                        } else {
                            // не получилось применить — просто ничего
                        }
                        aim = {};
                        draw_panel(world, panel_col);
                        std::cout.flush();
                        continue;
                    }

                    if (!(prev == aim.cur)) {
                        if (aim.mode == TargetMode::Area2x2) {
                            if (inBoundsSquare(world.field, prev, aim.areaSide)) {
                                for (int dy=0; dy<aim.areaSide; ++dy)
                                    for (int dx=0; dx<aim.areaSide; ++dx)
                                        draw_cell(world, {prev.getX()+dx, prev.getY()+dy});
                            }
                            draw_square_overlay(world, aim.cur, aim.areaSide);
                        } else {
                            draw_cell(world, prev);
                            draw_aim_cursor_single(aim.cur);
                        }
                        std::cout.flush();
                    }
                    continue;
                }

                // обычные действия
                if (k == 'e') {
                    int alive_before = alive_enemies(world);
                    bool ok2 = melee_attack(world, to_redraw);
                    if (ok2) {
                        world.hero.consume(); acted = true;
                        int alive_after = alive_enemies(world);
                        if (alive_after < alive_before) world.kills += (alive_before - alive_after);
                        maybe_award_card(world);
                    }
                } else if (k == 'r') {
                    int alive_before = alive_enemies(world);
                    bool ok2 = ranged_attack(world, to_redraw);
                    if (ok2) {
                        world.hero.consume(); acted = true;
                        int alive_after = alive_enemies(world);
                        if (alive_after < alive_before) world.kills += (alive_before - alive_after);
                        maybe_award_card(world);
                    }
                } else if (k >= '1' && k <= '9') {
                    int idx = (k - '1');
                    if (idx >= 0 && idx < (int)world.hand.capacity() && world.hand.has((std::size_t)idx)) {
                        Spell* s = world.hand.get((std::size_t)idx);
                        TargetMode tm = s->targeting();
                        int rad = s->radius() + world.boosts.directRadiusPlus; // для Cell/Area добавится boost в Spell’ах

                        aim = {};
                        aim.active = (tm != TargetMode::None);
                        aim.mode   = tm;
                        aim.radius = rad;
                        aim.origin = world.hero.getPosition();
                        aim.cur    = aim.origin;
                        aim.cardIndex = idx;

                        if (tm == TargetMode::None) {
                            bool ok = s->cast(world.hero, world.field, world.enemies, world.spawners, world.boosts, aim.cur);
                            if (ok) {
                                world.hand.remove((std::size_t)idx);
                                world.hero.consume(); acted = true;
                                flush_created_entities(world);
                                redraw_all_cells(world);
                                maybe_award_card(world);
                                draw_panel(world, panel_col);
                                std::cout.flush();
                            }
                            aim = {};
                        } else if (tm == TargetMode::Adjacent) {
                            const Position h = world.hero.getPosition();
                             const Position dirs[4]{{0,-1},{1,0},{-1,0},{0,1}};
                            bool placed = false;
                            for (auto d: dirs) {
                                Position cand{h.getX()+d.getX(), h.getY()+d.getY()};
                                if (world.field.inBounds(cand.getX(),cand.getY()) && world.field.passable(cand.getX(),cand.getY())
                                    && !is_enemy_at(world,cand) && !is_spawner_at(world,cand)
                                    && trap_index_at(world,cand) <0 && tower_index_at(world,cand) <0
                                    && !is_ally_at(world,cand)) { aim.cur=cand; placed=true; break; }
                            }
                            if (!placed) aim.cur = h;
                            draw_aim_cursor_single(aim.cur);
                            draw_panel(world, panel_col);
                            std::cout.flush();
                        } else if (tm == TargetMode::Cell) {
                            draw_aim_cursor_single(aim.cur);
                            draw_panel(world, panel_col);
                            std::cout.flush();
                        } else if (tm == TargetMode::Area2x2) {
                            aim.areaSide = 2 + world.boosts.aoeSizePlus;
                            if (!inBoundsSquare(world.field, aim.cur, aim.areaSide)) {
                                if (aim.cur.getX() > 0 && inBoundsSquare(world.field, {aim.cur.getX()-1, aim.cur.getY()}, aim.areaSide)) aim.cur.setX(aim.cur.getX() - 1);
                                if (!inBoundsSquare(world.field, aim.cur, aim.areaSide) && aim.cur.getY() > 0 &&
                                    inBoundsSquare(world.field, {aim.cur.getX(), aim.cur.getY()-1}, aim.areaSide)) aim.cur.setY(aim.cur.getY() - 1);
                            }
                            draw_square_overlay(world, aim.cur, aim.areaSide);
                            draw_panel(world, panel_col);
                            std::cout.flush();
                        }
                    }
                } else if (k == 'f') {
                    (void)world.hand.drawRandom();
                    draw_panel(world, panel_col);
                    std::cout.flush();
                } else if (k=='w' || k=='a' || k=='s' || k=='d') {
                    Position d{0,0};
                    if (k=='w') d = {0,-1};
                    if (k=='s') d = {0, 1};
                    if (k=='a') d = {-1,0};
                    if (k=='d') d = { 1,0};
                    Position from = world.hero.getPosition();
                    Position dst  = from + d;
                    if (cell_free_for_hero(world, dst)) {
                        world.hero.setPos(dst);
                        if (world.field.at(dst.getX(),dst.getY()).getType() == CellType::Slow) world.hero.markEnteredSlow();
                        world.hero.consume();
                        acted = true;
                        to_redraw.push_back(from);
                        to_redraw.push_back(dst);
                    }
                }

                // частичная перерисовка
                std::set<Position> uniq;
                for (const auto& p : to_redraw) if (uniq.insert(p).second) draw_cell(world, p);
                std::cout.flush();
            }
            if (!quit) world.hero.endTurn();
        }
        if (quit) break;

        std::vector<Position> to_redraw;
        enemies_turn(world, to_redraw);
        spawners_turn(world, to_redraw);
        towers_turn(world, to_redraw);
        allies_turn(world, to_redraw);
        maybe_award_card(world);

        auto rmE = std::remove_if(world.enemies.begin(), world.enemies.end(), [](const Enemy& e){return e.isDead();});
        world.enemies.erase(rmE, world.enemies.end());
        auto rmA = std::remove_if(world.allies.begin(), world.allies.end(), [](const Ally& a){return a.isDead();});
        world.allies.erase(rmA, world.allies.end());

        std::set<Position> uniq;
        for (const auto& p : to_redraw) if (uniq.insert(p).second) draw_cell(world, p);
        std::cout.flush();
    }

    cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
    if (world.hero.isDead()) std::cout << "игра окончена, нажмите любую клавишу..." << std::flush;
    else                     std::cout << "вы вышли из игры, нажмите любую клавишу..." << std::flush;
    (void)read_key_immediate();
    return 0;
}