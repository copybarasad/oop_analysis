#include "hero_phase.hpp"

#include <cmath>
#include <iostream>
#include <set>
#include <vector>

#include "rendering.hpp"
#include "spell_callbacks.hpp"
#include "terminal.hpp"
#include "turns.hpp"
#include "world.hpp"

namespace {
int manh(const Position& a, const Position& b) {
    return std::abs(a.getX()-b.getX()) + std::abs(a.getY()-b.getY());
}
}

bool handle_hero_phase(World& world, Aim& aim, bool& quit) {
    const int panel_col = ORIGIN_X + world.field.getWidth()*2 + 4;

    bool acted = false;
    while (!acted) {
        int k = read_key_immediate();
        if (!k) continue;

        if (k == 'o' || k == 'O') {
            try {
                save_world_to(world, "save.txt");
                cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
                std::cout << "сохранено в save.txt" << std::flush;
            } catch (const std::exception& e) {
                cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
                std::cout << "ошибка сохранения: " << e.what() << std::flush;
            }
            continue;
        }
        if (k == 'l' || k == 'L') {
            try {
                World nw = load_world_from("save.txt");
                world = std::move(nw);
                draw_world_initial(world);
                int pc = ORIGIN_X + world.field.getWidth()*2 + 4;
                draw_panel(world, pc);
                cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
                std::cout << "загружено из save.txt" << std::flush;
            } catch (const std::exception& e) {
                cursor_move(ORIGIN_Y + world.field.getHeight() + 2, ORIGIN_X);
                std::cout << "ошибка загрузки: " << e.what() << std::flush;
            }
            continue;
        }

        if (k == 'q') { quit = true; break; }

        std::vector<Position> to_redraw;

        if (aim.active) {
            Position prev = aim.cur;

            if (aim.mode == TargetMode::Adjacent) {
                const Position h = world.hero.getPosition();
                Position cand = aim.cur;
                if (k=='w') cand = Position(h.getX(),   h.getY()-1);
                if (k=='s') cand = Position(h.getX(),   h.getY()+1);
                if (k=='a') cand = Position(h.getX()-1, h.getY());
                if (k=='d') cand = Position(h.getX()+1, h.getY());
                bool ok = world.field.inBounds(cand.getX(),cand.getY()) && world.field.passable(cand.getX(),cand.getY())
                       && !is_enemy_at(world, cand) && !is_spawner_at(world, cand)
                       && trap_index_at(world, cand) < 0 && tower_index_at(world, cand) < 0
                       && !is_ally_at(world, cand);
                if (ok) aim.cur = cand;
            } else if (aim.mode == TargetMode::Cell) {
                if (k=='w') aim.cur.setY(aim.cur.getY()-1);
                if (k=='s') aim.cur.setY(aim.cur.getY()+1);
                if (k=='a') aim.cur.setX(aim.cur.getX()-1);
                if (k=='d') aim.cur.setX(aim.cur.getX()+1);
                if (!world.field.inBounds(aim.cur.getX(),aim.cur.getY()) || manh(aim.cur, aim.origin) > aim.radius)
                    aim.cur = prev;
            } else if (aim.mode == TargetMode::Area2x2) {
                if (k=='w') aim.cur.setY(aim.cur.getY()-1);
                if (k=='s') aim.cur.setY(aim.cur.getY()+1);
                if (k=='a') aim.cur.setX(aim.cur.getX()-1);
                if (k=='d') aim.cur.setX(aim.cur.getX()+1);
                if (!inBoundsSquare(world.field, aim.cur, aim.areaSide) || manh(aim.cur, aim.origin) > aim.radius)
                    aim.cur = prev;
            }

            if (k == 'c') {
                if (aim.mode == TargetMode::Area2x2) {
                    if (inBoundsSquare(world.field, prev, aim.areaSide)) {
                        for (int dy=0; dy<aim.areaSide; ++dy)
                            for (int dx=0; dx<aim.areaSide; ++dx)
                                draw_cell(world, Position(prev.getX()+dx, prev.getY()+dy));
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
                if (!world.hand.has((std::size_t)aim.cardIndex)) { aim = {}; continue; }
                Spell* s = world.hand.get((std::size_t)aim.cardIndex);
                bool ok = s->cast(world.hero, world.field, world.enemies, world.spawners, world.boosts, aim.cur);
                if (ok) {
                    world.hand.remove((std::size_t)aim.cardIndex);
                    world.hero.consume();
                    acted = true;
                    flush_created_entities(world);

                    if (aim.mode == TargetMode::Area2x2) {
                        if (inBoundsSquare(world.field, aim.cur, aim.areaSide)) {
                            for (int dy=0; dy<aim.areaSide; ++dy)
                                for (int dx=0; dx<aim.areaSide; ++dx)
                                    draw_cell(world, Position(aim.cur.getX()+dx, aim.cur.getY()+dy));
                        } else {
                            redraw_all_cells(world);
                        }
                    } else {
                        draw_cell(world, aim.cur);
                    }
                    maybe_award_card(world);
                }
                aim = {};
                draw_panel(world, panel_col);
                std::cout.flush();
                continue;
            }

            if (!(prev.getX()==aim.cur.getX() && prev.getY()==aim.cur.getY())) {
                if (aim.mode == TargetMode::Area2x2) {
                    if (inBoundsSquare(world.field, prev, aim.areaSide)) {
                        for (int dy=0; dy<aim.areaSide; ++dy)
                            for (int dx=0; dx<aim.areaSide; ++dx)
                                draw_cell(world, Position(prev.getX()+dx, prev.getY()+dy));
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
                int rad = s->radius() + world.boosts.directRadiusPlus;

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
                    static const Position dirs[4]{{0,-1},{1,0},{-1,0},{0,1}};
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
                        if (aim.cur.getX() > 0 && inBoundsSquare(world.field, Position(aim.cur.getX()-1, aim.cur.getY()), aim.areaSide)) aim.cur.setX(aim.cur.getX()-1);
                        if (!inBoundsSquare(world.field, aim.cur, aim.areaSide) && aim.cur.getY() > 0 &&
                            inBoundsSquare(world.field, Position(aim.cur.getX(), aim.cur.getY()-1), aim.areaSide)) aim.cur.setY(aim.cur.getY()-1);
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

        std::set<Position> uniq;
        for (const auto& p : to_redraw) if (uniq.insert(p).second) draw_cell(world, p);
        std::cout.flush();
    }

    return !quit;
}

