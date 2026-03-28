#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "game.hpp"
#include "game_errors.hpp"
#include "hero_phase.hpp"
#include "level_up_menu.hpp"
#include "rendering.hpp"
#include "setup.hpp"
#include "spell_callbacks.hpp"
#include "spell.hpp"
#include "terminal.hpp"
#include "turns.hpp"
#include "world.hpp"

void Game::run() {
    bool reuseTemplate = false; 
    const std::string kTemplatePath="save.txt";

    for(;;) {
    try {
    bool useLoaded = false;
    World loadedWorld(Field(10,10), Hero(10,2, Position(0,0)), {}, {}, 5);
    if (reuseTemplate) {
        loadedWorld = load_world_from(kTemplatePath);
        useLoaded = true;
        reuseTemplate = false;
    } else {
        std::cout << "1) Новая игра\n2) Загрузиться из save.txt\n> " << std::flush;
        std::string _choice; std::getline(std::cin, _choice);
        if (!_choice.empty() && _choice[0] == '2') {
            try {
                loadedWorld = load_world_from("save.txt");
                useLoaded = true;
                std::cout << "Загрузка успешна.\n";
            } catch (const std::exception& e) {
                std::cout << "Не удалось загрузить: " << e.what() << "\nНачинаем новую игру.\n";
            }
        }
    }
    World world = [&]{
        if (useLoaded) return std::move(loadedWorld);

        GameParams params = prompt_game_params();
        Field fld{std::max(10, std::min(params.width, 25)),
                  std::max(10, std::min(params.height, 25))};
        Hero hero{std::max(1, params.heroHp), std::max(1, params.heroDmg), Position(0,0)};
        std::vector<Spawner> spawners;
        if (!build_world_from_params(params, fld, hero, spawners)) {
            std::cout << "не удалось построить мир (проверьте ввод)\n";
            throw ValidationError("build_world_from_params failed (conflicting positions or invalid input)");
        }
        std::vector<Enemy> enemies;
        std::size_t handCap = params.handSize > 0 ? (std::size_t)params.handSize : (std::size_t)5;
        World w{std::move(fld), std::move(hero), std::move(enemies), std::move(spawners), handCap};

        w.level      = 1;
        w.baseHeroHp = w.hero.getHp();
        w.hand.drawRandom();
        for (auto& s : w.spawners) {
            if (s.hasTower()) continue;
            static const Position order[4]{{1,0},{0,1},{-1,0},{0,-1}};
            Position sPos = s.getPos();
            for (auto d : order) {
                Position cand{sPos.getX() + d.getX(), sPos.getY() + d.getY()};
                if (!w.field.inBounds(cand.getX(),cand.getY())) continue;
                if (!w.field.passable(cand.getX(),cand.getY())) continue;
                if (w.hero.getPosition() == cand) continue;
                bool blocked = false;
                for (auto const& e: w.enemies) if (!e.isDead() && e.getPosition()==cand) { blocked=true; break; }
                if (blocked) continue;
                for (auto const& a: w.allies) if (!a.isDead() && a.getPosition()==cand) { blocked=true; break; }
                if (blocked) continue;
                for (auto const& t: w.towers) if (t.getPos()==cand) { blocked=true; break; }
                if (blocked) continue;
                for (auto const& sp: w.spawners) if (sp.getPos()==cand) { blocked=true; break; }
                if (blocked) continue;
                if (trap_index_at(w, cand) >= 0) continue;
                w.towers.push_back(Tower(cand, std::max(1, w.level), 2));
                s.markTower();
                break;
            }
        }
    
        return w;
    }();

    TerminalRaw term; term.enable();
    draw_world_initial(world);
    int panel_col0 = ORIGIN_X + world.field.getWidth()*2 + 4;
    draw_panel(world, panel_col0);

    Aim aim;
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
            if (!handle_hero_phase(world, aim, quit)) break;
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
        auto rmS = std::remove_if(world.spawners.begin(), world.spawners.end(), [](const Spawner& s){ return s.isDead(); });
        world.spawners.erase(rmS, world.spawners.end());

        std::set<Position> uniq;
        for (const auto& p : to_redraw) if (uniq.insert(p).second) draw_cell(world, p);
        std::cout.flush();

        if (!world.hero.isDead() && world.spawners.empty() && world.enemies.empty()) {
            level_up_menu(world);
            proceed_to_next_level(world);
            draw_world_initial(world);
            int pc = ORIGIN_X + world.field.getWidth()*2 + 4;
            draw_panel(world, pc);
            continue;
        }
    }

    cursor_move(ORIGIN_Y + world.field.getHeight() + 4, ORIGIN_X);
if (world.hero.isDead()) {
    std::cout << "игра окончена. (r — заново, q — выход)" << std::flush;
    for (;;) {
        int ch = read_key_immediate();
        if (ch=='r' || ch=='R') {
            
            throw RestartGame{};
        } else if (ch=='q' || ch=='Q' || ch==27) {
            return;
        }
    }
} else {
    std::cout << "вы вышли из игры, нажмите любую клавишу..." << std::flush;
    (void)read_key_immediate();
    return;
}
   
    } catch (const RestartGame&) {
            reuseTemplate = true;
            continue;     
        } catch (const FileOpenError& e) {
            std::cout << "[Файл] " << e.what() << "\nНажмите любую клавишу..." << std::flush;
            (void)read_key_immediate();
            continue;
        } catch (const ParseError& e) {
            std::cout << "[Парсинг] " << e.what() << "\nНажмите любую клавишу..." << std::flush;
            (void)read_key_immediate();
            continue;
        } catch (const ValidationError& e) {
            std::cout << "[Валидация] " << e.what() << "\nНажмите любую клавишу..." << std::flush;
            (void)read_key_immediate();
            continue;
        } catch (const GameLogicError& e) {
            std::cout << "[Логика] " << e.what() << "\nНажмите любую клавишу..." << std::flush;
            (void)read_key_immediate();
            continue;
        } catch (const std::exception& e) {
            std::cout << "[Неожиданная ошибка] " << e.what() << "\nНажмите любую клавишу..." << std::flush;
            (void)read_key_immediate();
            continue;
        }
    }
}

