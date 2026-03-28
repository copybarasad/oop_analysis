#include "game_controller.hpp"
#include "game_io.hpp"
#include <iostream>
#include <cmath>
#include "constants.hpp"

void GameController::Run() {

    while (true) {
        std::cout << "==============================\n";
        std::cout << "1 — новая игра\n";
        std::cout << "2 — загрузить игру\n";
        std::cout << "3 — выход\n";
        std::cout << "==============================\n> ";

        char menu_choice;
        if (!(std::cin >> menu_choice))
            return;

        if (menu_choice == '3')
            return;

        if (menu_choice == '1') {
            game_ = Game();
        }

        else if (menu_choice == '2') {
            std::string filename;
            std::cout << "Имя файла сохранения: ";
            std::cin >> filename;

            try {
                GameLoad(game_, filename);
                std::cout << "✔ Сохранение загружено.\n";
            }
            catch (const std::exception& ex) {
                std::cout << "❌ Ошибка загрузки: " << ex.what() << "\n";
                std::cout << "Возврат в меню...\n";
                continue;
            }
        }

        else {
            std::cout << "Неизвестный пункт меню.\n";
            continue;
        }

        std::cout << "Игра началась. Уровень " << game_.GetLevel() << "\n";
        game_.PrintState();

        bool exit_to_menu = false;

        while (!exit_to_menu) {

            if (game_.IsLevelCompleted()) {

                std::cout << "\nУровень " << game_.GetLevel() << " пройден.\n";
                std::cout << "N — следующий уровень, Q — в меню\n> ";

                char lvl_choice;
                std::cin >> lvl_choice;

                if (lvl_choice == 'N' || lvl_choice == 'n') {
                    game_.NextLevel();
                    ShowUpgradeMenu();
                    std::cout << "Уровень " << game_.GetLevel() << " начался.\n";
                    game_.PrintState();
                    continue;
                }

                exit_to_menu = true;
                break;
            }

            std::cout << "\n==============================\n";
            std::cout << "Уровень: " << game_.GetLevel() << "\n";
            std::cout << "Ход игрока:\n";
            std::cout << "W/A/S/D — движение\n";
            std::cout << "F — атака | U — заклинание | M — смена режима боя\n";
            std::cout << "I — инвентарь | T — статус | H — рука\n";
            std::cout << "Z — сохранить | L — загрузить | Q — в меню\n";
            std::cout << "==============================\n> ";

            char move;
            std::cin >> move;

            auto endTurn = [&]() {

                game_.MoveAllies();
                game_.MoveEnemies();

                for (auto& b : game_.GetBuildings())
                    if (!b.IsDestroyed())
                        b.Tick(game_.GetEnemies(), game_.GetField());

                game_.PrintState();

                if (!game_.GetPlayer().IsAlive()) {
                    std::cout << "\nИгрок погиб.\n";
                    std::cout << "1 — начать заново\n";
                    std::cout << "2 — выйти из программы\n> ";
                    char d;
                    std::cin >> d;

                    if (d == '1') {
                        game_ = Game();
                        std::cout << "Новая игра началась.\n";
                        game_.PrintState();
                    } else {
                        std::exit(0);
                    }
                }
            };

            if (move == ACTION_QUIT || move == std::tolower(ACTION_QUIT)) {
                std::cout << "Выход в меню.\n";
                exit_to_menu = true;
                break;
            }

            if (move == ACTION_SAVE || move == std::tolower(ACTION_SAVE)) {
                std::string filename;
                std::cout << "Имя файла для сохранения: ";
                std::cin >> filename;

                try {
                    GameSave(game_, filename);
                    std::cout << "Игра сохранена.\n";
                }
                catch (const std::exception& ex) {
                    std::cout << "Ошибка сохранения: " << ex.what() << "\n";
                }

                game_.PrintState();
                continue;
            }

            if (move == ACTION_LOAD || move == std::tolower(ACTION_LOAD)) {

                std::string filename;
                std::cout << "Имя файла сохранения: ";
                std::cin >> filename;

                bool success = true;

                try {
                    GameLoad(game_, filename);
                    std::cout << "✔ Сохранение загружено.\n";
                }
                catch (const std::exception& ex) {
                    std::cout << "❌ Ошибка загрузки: " << ex.what() << "\n";
                    std::cout << "Возврат в меню...\n";
                    success = false;
                }

                if (!success) {
                    exit_to_menu = true;
                    break;
                }

                game_.PrintState();
                continue;
            }

            if (move == ACTION_INVENTORY || move == std::tolower(ACTION_INVENTORY)) {
                game_.GetPlayer().PrintInventory();
                std::cout << "\nВведите номер зелья или 0 для выхода: ";
                int choice;
                std::cin >> choice;

                if (choice != 0)
                    game_.GetPlayer().UsePotion(choice - 1);

                game_.PrintState();
                continue;
            }

            if (move == ACTION_STATUS || move == std::tolower(ACTION_STATUS)) {
                game_.GetPlayer().PrintStatus();
                game_.PrintState();
                continue;
            }

            if (move == ACTION_HAND || move == std::tolower(ACTION_HAND)) {
                game_.GetPlayer().GetHand().PrintHand();
                continue;
            }

            if (move == ACTION_MODE || move == std::tolower(ACTION_MODE)) {
                game_.GetPlayer().ToggleMode();
                game_.PrintState();
                continue;
            }

            if (move == ACTION_ATTACK || move == std::tolower(ACTION_ATTACK)) {
                bool attacked = false;

                for (auto& e : game_.GetEnemies()) {
                    if (!e.IsAlive())
                        continue;

                    int dx = std::abs(e.GetX() - game_.GetPlayer().GetX());
                    int dy = std::abs(e.GetY() - game_.GetPlayer().GetY());
                    int dist = dx + dy;

                    bool can_attack_ranged =
                        game_.GetPlayer().IsRanged() &&
                        ((dx == 0 && dy <= RANGE_ATTACK_DISTANCE) ||
                         (dy == 0 && dx <= RANGE_ATTACK_DISTANCE));

                    bool can_attack_melee =
                        !game_.GetPlayer().IsRanged() &&
                        dist == MELEE_ATTACK_DISTANCE;

                    if (can_attack_ranged || can_attack_melee) {
                        game_.GetPlayer().Attack(e);
                        attacked = true;

                        if (!e.IsAlive())
                            std::cout << "Враг " << e.GetName() << " повержен.\n";

                        break;
                    }
                }

                if (!attacked)
                    std::cout << "Нет врагов в радиусе атаки.\n";

                endTurn();
                continue;
            }

            if (move == ACTION_USE_SPELL || move == std::tolower(ACTION_USE_SPELL)) {

                game_.GetPlayer().GetHand().PrintHand();
                std::cout << "Введите номер заклинания: ";
                size_t choice;
                std::cin >> choice;

                if (choice == 0 ||
                    choice > game_.GetPlayer().GetHand().Size()) {
                    std::cout << "Неверный выбор.\n";
                    continue;
                }

                game_.GetPlayer().GetHand().UseSpell(choice - 1, game_);
                endTurn();
                continue;
            }

            int dx = 0, dy = 0;

            if (move == STEP_UP    || move == std::tolower(STEP_UP))    dy = -1;
            else if (move == STEP_DOWN  || move == std::tolower(STEP_DOWN))  dy = 1;
            else if (move == STEP_LEFT  || move == std::tolower(STEP_LEFT))  dx = -1;
            else if (move == STEP_RIGHT || move == std::tolower(STEP_RIGHT)) dx = 1;
            else {
                std::cout << "Неизвестная команда.\n";
                game_.PrintState();
                continue;
            }

            if (game_.GetPlayer().ShouldSkipTurn()) {
                std::cout << "🧊 Вы заморожены! Ход пропущен.\n";

                game_.GetPlayer().SetSkipTurn(false);

                game_.MoveAllies();
                game_.MoveEnemies();
                for (auto& b : game_.GetBuildings())
                    if (!b.IsDestroyed())
                        b.Tick(game_.GetEnemies(), game_.GetField());

                game_.PrintState();
                continue;
            }

            game_.GetPlayer().Move(dx, dy, game_.GetField(), game_.GetEnemies());
            endTurn();
        }
    }
}

void GameController::ShowUpgradeMenu() {

    std::cout << "\nВыберите улучшение:\n";
    std::cout << "1 — увеличить максимальное HP на 5\n";
    std::cout << "2 — увеличить урон игрока на 10\n";
    std::cout << "3 — улучшить заклинание из руки\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    switch (choice) {

        case 1: {
            int newMax = game_.GetPlayer().GetMaxHealth() + 5;
            game_.GetPlayer().SetMaxHealth(newMax);
            game_.GetPlayer().SetHealth(newMax);
            std::cout << "❤️ Максимальное здоровье увеличено! Теперь: " << newMax << "\n";
            break;
        }

        case 2: {
            int newDmg = game_.GetPlayer().GetDamage() + 5;
            game_.GetPlayer().SetDamage(newDmg);
            std::cout << "🗡 Урон увеличен! Теперь: " << newDmg << "\n";
            break;
        }

        case 3: {
            if (game_.GetPlayer().GetHand().Size() == 0) {
                std::cout << "❌ В руке нет заклинаний.\n";
                break;
            }

            game_.GetPlayer().GetHand().PrintHand();
            std::cout << "Введите номер заклинания для улучшения: ";
            size_t idx;
            std::cin >> idx;

            if (idx == 0 || idx > game_.GetPlayer().GetHand().Size()) {
                std::cout << "❌ Неверный выбор.\n";
                break;
            }

            game_.GetPlayer().GetHand().UpgradeSpell(idx - 1);
            break;
        }

        default:
            std::cout << "❌ Неверный пункт.\n";
            break;
    }
}