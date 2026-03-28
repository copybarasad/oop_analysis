#include "level_up_menu.hpp"

#include <iostream>

#include "rendering.hpp"
#include "terminal.hpp"

void level_up_menu(World& w) {
    int base_y = ORIGIN_Y + w.field.getHeight() + 2;

    auto msg = [&](int dy, const std::string& s) {
        cursor_move(base_y + dy, ORIGIN_X);
        std::cout << std::string(80, ' ') << std::flush;
        cursor_move(base_y + dy, ORIGIN_X);
        std::cout << s << std::flush;
    };

    msg(0, "=== Прокачка ===");
    msg(1, "1) +2 урона героя");
    msg(2, "2) +5 к базовому HP героя (восстановление на новый максимум при переходе)");
    msg(3, "3) Direct: +1 к радиусу (boosts.directRadiusPlus)");
    msg(4, "4) AoE: +1 к стороне квадрата (boosts.aoeSizePlus)");
    msg(5, "5) Ловушки: +2 к урону (boosts.trapDamagePlus)");
    msg(6, "6) Призыв: +1 к количеству союзников (boosts.summonCountPlus)");
    msg(7, "Выберите 1..6 (или c=пропустить): ");

    for (;;) {
        int k = read_key_immediate();
        if (!k) continue;

        if (k == '1') {
            Hero h = w.hero;
            h = Hero(h.getHp(), h.getDmg() + 2, h.getPosition());
            w.hero = std::move(h);
            msg(8, "Готово: урон героя увеличен на +2.");
            break;
        } else if (k == '2') {
            w.baseHeroHp = (w.baseHeroHp > 0 ? w.baseHeroHp : w.hero.getHp());
            w.baseHeroHp += 5;
            msg(8, "Готово: базовое HP увеличено на +5.");
            break;
        } else if (k == '3') {
            w.boosts.directRadiusPlus += 1;
            msg(8, "Готово: Direct-радиус +1.");
            break;
        } else if (k == '4') {
            w.boosts.aoeSizePlus += 1;
            msg(8, "Готово: AoE-площадь +1 к стороне.");
            break;
        } else if (k == '5') {
            w.boosts.trapDamagePlus += 2;
            msg(8, "Готово: урон ловушек +2.");
            break;
        } else if (k == '6') {
            w.boosts.summonCountPlus += 1;
            msg(8, "Готово: призыв +1 союзник.");
            break;
        } else if (k == 'c' || k == 'C' || k == 'q' || k == 'Q' || k == 27) {
            msg(8, "Прокачка пропущена.");
            break;
        }
    }

    msg(9, "Нажмите любую клавишу...");
    (void)read_key_immediate();

    for (int dy = 0; dy <= 9; ++dy) {
        cursor_move(base_y + dy, ORIGIN_X);
        std::cout << std::string(100, ' ');
    }
    std::cout.flush();
}

