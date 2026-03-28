#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cctype>
#include <unistd.h>
#include <termios.h>

#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"
#include "setup.hpp"

struct TerminalRaw {
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
        std::cout << "\x1b[?25l\x1b[2J\x1b[H" << std::flush; // прячем курсор и чистим экран
        active = true;
    }
    void disable() {
        if (!active) return;
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        std::cout << "\x1b[?25h\x1b[0m\n" << std::flush; // возвращаем курсор
        active = false;
    }
    ~TerminalRaw() { disable(); }
};

// читаем клавишу
static int read_key_immediate() {
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

// мировое состояние
struct World {
    Field field;
    Hero hero;
    std::vector<Enemy> enemies;
    std::vector<Spawner> spawners;
    int kills{0};

    World(Field f, Hero h, std::vector<Enemy> es, std::vector<Spawner> ss)
        : field(std::move(f)), hero(std::move(h)), enemies(std::move(es)), spawners(std::move(ss)) {}
};

static const int ORIGIN_Y = 1;
static const int ORIGIN_X = 2;

static inline void cursor_move(int y, int x) {
    std::cout << "\x1b[" << y << ";" << x << "H";
}

// проверяем занятость клеток
static bool is_enemy_at(const World& w, const Position& p, int* idx=nullptr) {
    for (int i=0;i<(int)w.enemies.size();++i) {
        if (!w.enemies[i].isDead() && w.enemies[i].getPosition() == p) {
            if (idx) *idx = i;
            return true;
        }
    }
    return false;
}
static bool is_spawner_at(const World& w, const Position& p) {
    for (const auto& s : w.spawners) if (s.pos() == p) return true;
    return false;
}

// символ, который в клетке
static char char_for_cell(const World& w, const Position& p) {
    if (w.hero.getPosition() == p && !w.hero.isDead()) return 'H';
    if (is_enemy_at(w, p)) return 'E';
    if (is_spawner_at(w, p)) return 'S';
    return w.field.at(p.x, p.y).getSymbol();
}

// рисуем одну клетку
static void draw_cell(const World& w, const Position& p) {
    int sx = ORIGIN_X + p.x * 2;
    int sy = ORIGIN_Y + p.y;
    cursor_move(sy, sx);
    std::cout << char_for_cell(w, p) << ' ';
}

// панель справа
static void draw_panel(const World& w, int start_col) {
    int y = 1, x = start_col;
    cursor_move(y, x);
    std::cout << "hp: " << w.hero.getHp() << "          ";
    cursor_move(y+1, x);
    std::cout << "режим: " << (w.hero.getMode()==CombatMode::Melee? "melee":"ranged") << "          ";
    cursor_move(y+2, x);
    int alive = (int)std::count_if(w.enemies.begin(), w.enemies.end(), [](const Enemy& e){return !e.isDead();});
    std::cout << "враги: " << alive << "          ";
    cursor_move(y+3, x);
    std::cout << "убито: " << w.kills << "          ";
    cursor_move(y+4, x);
    std::cout << "ходьба: wasd, e: атака          ";
    cursor_move(y+5, x);
    std::cout << "t: режим, q: выход           ";
}

// первичная отрисовка
static void draw_world_initial(const World& w) {
    const int W = w.field.getWidth(), H = w.field.getHeight();
    std::cout << "\x1b[2J\x1b[H";
    for (int x=0;x<=W*2+1;++x) { cursor_move(ORIGIN_Y-1, ORIGIN_X-1 + x); std::cout << (x==0||x==W*2+1?'+':'-'); }
    for (int y=0;y<H;++y) {
        cursor_move(ORIGIN_Y + y, ORIGIN_X-1); std::cout << '|';
        cursor_move(ORIGIN_Y + y, ORIGIN_X + W*2); std::cout << '|';
    }
    for (int x=0;x<=W*2+1;++x) { cursor_move(ORIGIN_Y + H, ORIGIN_X-1 + x); std::cout << (x==0||x==W*2+1?'+':'-'); }

    for (int y=0;y<H;++y)
        for (int x=0;x<W;++x)
            draw_cell(w, Position{x,y});

    int panel_col = ORIGIN_X + W*2 + 4;
    draw_panel(w, panel_col);

    std::cout.flush();
}

// валидаторы клеток
static bool cell_free_for_enemy(const World& w, const Position& p) {
    if (!w.field.inBounds(p.x,p.y)) return false;
    if (!w.field.passable(p.x,p.y)) return false;
    if (w.hero.getPosition() == p) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    return true;
}
static bool cell_free_for_hero(const World& w, const Position& p) {
    if (!w.field.inBounds(p.x,p.y)) return false;
    if (!w.field.passable(p.x,p.y)) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    return true;
}

// атака ближним боем (по соседям)
static bool melee_attack(World& w, std::vector<Position>& to_redraw) {
    static const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
    for (const auto& d : dirs) {
        Position p = w.hero.getPosition() + d;
        int idx = -1;
        if (is_enemy_at(w, p, &idx)) {
            w.enemies[idx].takeDamage(w.hero.setDamage());
            if (w.enemies[idx].isDead()) {
                w.kills += 1;
                to_redraw.push_back(p);
            }
            return true;
        }
    }
    return false;
}

// атака дальним боем (до 3 клеток)
static bool ranged_attack(World& w, std::vector<Position>& to_redraw) {
    static const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
    for (const auto& d : dirs) {
        Position p = w.hero.getPosition();
        for (int step=0; step<3; ++step) {
            p = p + d;
            if (!w.field.inBounds(p.x,p.y) || !w.field.passable(p.x,p.y)) break;
            int idx=-1;
            if (is_enemy_at(w, p, &idx)) {
                w.enemies[idx].takeDamage(w.hero.setDamage());
                if (w.enemies[idx].isDead()) {
                    w.kills += 1;
                    to_redraw.push_back(p);
                }
                return true;
            }
        }
    }
    return false;
}

// ход врагов
static void enemies_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& e : w.enemies) {
        if (e.isDead()) continue;
        Position from = e.getPosition();
        Position step = Enemy::chooseStep(e.getPosition(), w.hero.getPosition());
        Position dst{from.x + step.x, from.y + step.y};
        if (!w.field.inBounds(dst.x,dst.y)) continue;
        if (dst == w.hero.getPosition()) {
            w.hero.takeDamage(e.getDmg());
            continue;
        }
        if (!cell_free_for_enemy(w, dst)) continue;
        e.setPos(dst);
        to_redraw.push_back(from);
        to_redraw.push_back(dst);
    }
}

// ход спавнеров
static void spawners_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& s : w.spawners) {
        auto spawn_pos = s.onTurn([&](const Position& p){ return cell_free_for_enemy(w,p); });
        if (spawn_pos) {
            w.enemies.emplace_back(Enemy{6,2,*spawn_pos});
            to_redraw.push_back(*spawn_pos);
        }
    }
}

int main() {
    //  параметры и стройка мира до игры
    GameParams params = prompt_game_params();

    Field fld{std::max(10, std::min(params.width, 25)),
              std::max(10, std::min(params.height, 25))};
    Hero hero{std::max(1, params.heroHp), std::max(1, params.heroDmg), Position{0,0}};
    std::vector<Spawner> spawners;

    if (!build_world_from_params(params, fld, hero, spawners)) {
        std::cout << "не удалось построить мир (проверьте ввод)\n";
        return 0;
    }

    std::vector<Enemy> enemies;
    World world{std::move(fld), std::move(hero), std::move(enemies), std::move(spawners)};

    // включаем управление и рисуем стартовое состояние
    TerminalRaw term;
    term.enable();
    draw_world_initial(world);

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

                if (k == 't') {
                    if (world.hero.switchMode()) {
                        acted = true;
                        draw_panel(world, panel_col);
                        std::cout.flush();
                    }
                } else if (k == 'e') {
                    bool ok = (world.hero.getMode()==CombatMode::Melee)
                              ? melee_attack(world, to_redraw)
                              : ranged_attack(world, to_redraw);
                    if (ok) { world.hero.consume(); acted = true; }
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
                        if (world.field.at(dst.x,dst.y).getType() == CellType::Slow) {
                            world.hero.markEnteredSlow();
                        }
                        world.hero.consume();
                        acted = true;
                        to_redraw.push_back(from);
                        to_redraw.push_back(dst);
                    }
                }

                for (const auto& p : to_redraw) draw_cell(world, p);
                std::cout.flush();
            }
            if (!quit) world.hero.endTurn();
        }
        if (quit) break;

        std::vector<Position> to_redraw;
        enemies_turn(world, to_redraw);
        spawners_turn(world, to_redraw);

        world.enemies.erase(std::remove_if(world.enemies.begin(), world.enemies.end(),
            [](const Enemy& e){ return e.isDead(); }), world.enemies.end());

        for (const auto& p : to_redraw) draw_cell(world, p);
        std::cout.flush();
    }

    cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
    if (world.hero.isDead())
        std::cout << "игра окончена, нажмите любую клавишу..." << std::flush;
    else
        std::cout << "вы вышли из игры, нажмите любую клавишу..." << std::flush;

    read_key_immediate();
    return 0;
}
