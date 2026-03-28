#include "Game.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <optional>
#include <sstream>
#include <string>

// ==========================================================
// Game.cpp — логика игры: обработка ввода и основной цикл хода
// ==========================================================

// ----------------------------
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ----------------------------
// Они спрятаны в anonymous namespace, чтобы были видны только в этом .cpp.
namespace {

// Возвращает знак числа: -1, 0 или 1.
// Используется, чтобы враг делал шаг в сторону игрока.
int sign(int v) {
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}

// Преобразует клавишу атаки в направление (dx, dy)
// i = вверх, k = вниз, j = влево, l = вправо
bool attack_dir_from_key(char k, int& dx, int& dy) {
    switch (k) {
        case 'i': dx =  0; dy = -1; return true;
        case 'k': dx =  0; dy =  1; return true;
        case 'j': dx = -1; dy =  0; return true;
        case 'l': dx =  1; dy =  0; return true;
        default:  return false;
    }
}

// Выполняет атаку игрока по направлению (dx,dy).
// В melee — бьём соседнюю клетку.
// В ranged — "луч" летит, пока не встретит стену (непроходимую клетку).
bool do_attack(Field& field, Player& pl, int dx, int dy) {
    if (dx == 0 && dy == 0) return false;

    const int damage = pl.damage();          // сколько урона наносит игрок
    int x = pl.pos().x + dx;                 // стартовая клетка атаки (рядом с игроком)
    int y = pl.pos().y + dy;

    // --------- ближний бой ----------
    if (pl.mode() == Player::Mode::Melee) {
        // пробуем ударить врага или гнездо в соседней клетке
        if (field.damage_enemy_at(x, y, damage)) return true;
        if (field.damage_nest_at(x, y, damage))  return true;

        // даже если никого нет, ход всё равно считается потраченным
        return true;
    }

    // --------- дальний бой ----------
    // "пуля/луч" летит, пока клетка проходимая
    while (field.passable(x, y)) {
        // если попали во врага/гнездо — наносим урон и прекращаем
        if (field.damage_enemy_at(x, y, damage) || field.damage_nest_at(x, y, damage)) {
            break;
        }
        x += dx;
        y += dy;
    }
    return true; // ход потрачен
}

} // namespace

// ----------------------------
// ЗАКЛИНАНИЯ: "мостик" между вводом и Hand/ISpell
// ----------------------------
// slot — номер слота в руке (0 = первый слот)
// target — цель (координаты), если пользователь их ввёл
bool Game::cast_spell_(std::size_t slot, std::optional<Pos> target) {
    Player& player = field_.player();

    // берём заклинание из руки по номеру слота
    ISpell* spell = player.hand().get(slot);
    if (!spell) return false; // в слоте пусто

    // проверяем, можно ли кастовать (например, хватает маны и т.п.)
    if (!spell->canCast(field_, player)) return false;

    // применяем эффект заклинания (иногда нужна цель, иногда нет)
    return spell->cast(field_, player, target);
}

// ----------------------------
// КОНСТРУКТОР ИГРЫ
// ----------------------------
Game::Game(int w, int h, const GameParams& params)
    : field_(w, h, params) {}

// ----------------------------
// ПРОВЕРКА: уровень завершён?
// ----------------------------
// Уровень считается завершённым, если игрок жив, а врагов и гнёзд не осталось.
bool Game::level_finished() const {
    const auto enemies_alive = std::any_of(
        field_.enemies().begin(), field_.enemies().end(),
        [](const Enemy& e) { return e.alive(); });

    const auto nests_alive = std::any_of(
        field_.nests().begin(), field_.nests().end(),
        [](const EnemyNest& n) { return n.alive(); });

    return field_.player().alive() && !enemies_alive && !nests_alive;
}

// ----------------------------
// СНИМОК СОСТОЯНИЯ ИГРЫ (для сохранений)
// ----------------------------
// snapshot() — собирает GameState (поле + флаг пропуска хода на slow-клетках)
GameState Game::snapshot() const {
    GameState state;
    state.field = field_.snapshot();
    state.playerSkipMove = playerSkipMove_;
    return state;
}

// apply_state() — применяет загруженное состояние
void Game::apply_state(const GameState& state) {
    field_ = Field(state.field);
    playerSkipMove_ = state.playerSkipMove;
}

// ==========================================================
// ОСНОВНОЙ ШАГ ИГРЫ: обработка команды + один полный "тик"
// ==========================================================
// Возвращает false, если игра завершена (выход или смерть игрока).
bool Game::step(const std::string& input) {
    Player& player = field_.player();

    // 1) Если уровень уже пройден — просто ничего не делаем (игра продолжается)
    if (level_finished()) {
        return true;
    }

    // 2) Если игрок мёртв — игра закончена
    if (!player.alive()) {
        return false;
    }

    // 3) Если пустая строка — пропускаем (не тратим ход)
    if (input.empty()) {
        return true;
    }

    // ----------------------------------------
    // 4) Парсинг ввода пользователя
    // ----------------------------------------
    std::istringstream line(input);
    char keyRaw = 0;
    line >> keyRaw;

    // приводим к нижнему регистру
    char key = static_cast<char>(std::tolower(static_cast<unsigned char>(keyRaw)));

    // q — выход из игры
    if (key == 'q') {
        return false;
    }

    bool turn_consumed = false; // потрачен ли ход игрока

    // ----------------------------------------
    // 5) Каст заклинания: клавиша '1'..'9'
    // Формат: "1" или "1 x y"
    // ----------------------------------------
    if (std::isdigit(static_cast<unsigned char>(key))) {
        std::size_t slotIndex = static_cast<std::size_t>(key - '1'); // '1' -> 0
        std::optional<Pos> target;
        int tx = 0, ty = 0;

        // если координаты в строке есть — считаем целью
        if (line >> tx >> ty) {
            target = Pos{tx, ty};
        }

        // если каст успешен — ход потрачен
        turn_consumed = cast_spell_(slotIndex, target);
    }

    // ----------------------------------------
    // 6) Переключение режима melee/ranged: 't'
    // ----------------------------------------
    if (!turn_consumed && key == 't') {
        player.toggle_mode();
        turn_consumed = true;
    }

    // ----------------------------------------
    // 7) Атака по направлению: i/j/k/l
    // ----------------------------------------
    int adx = 0;
    int ady = 0;
    if (!turn_consumed && attack_dir_from_key(key, adx, ady)) {
        turn_consumed = do_attack(field_, player, adx, ady);
    }

    // ----------------------------------------
    // 8) Движение: w/a/s/d
    // + логика "замедляющих" клеток (skip move)
    // ----------------------------------------
    const bool is_move_key = (key == 'w' || key == 'a' || key == 's' || key == 'd');
    if (!turn_consumed && is_move_key) {
        // если игрок должен пропустить ход из-за slow-клетки
        if (playerSkipMove_) {
            playerSkipMove_ = false;
            turn_consumed = true;
        } else {
            Pos new_pos = player.pos();
            if (key == 'w') new_pos.y -= 1;
            if (key == 's') new_pos.y += 1;
            if (key == 'a') new_pos.x -= 1;
            if (key == 'd') new_pos.x += 1;

            // проверка, что клетка свободна:
            // - проходимая
            // - на ней нет врага
            // - на ней нет гнезда
            const bool free_tile =
                field_.passable(new_pos.x, new_pos.y) &&
                !field_.enemy_at(new_pos.x, new_pos.y) &&
                !field_.nest_at(new_pos.x, new_pos.y);

            // если клетка свободна — переносим игрока
            if (free_tile) {
                player.set_pos(new_pos);

                // если клетка slow — на следующий ход игрок пропускает движение
                if (field_.is_slow(new_pos.x, new_pos.y)) {
                    playerSkipMove_ = true;
                }
            }
            turn_consumed = true;
        }
    }

    // ----------------------------------------
    // 9) Если команда не распознана — просто ничего не делаем
    // ----------------------------------------
    if (!turn_consumed) {
        return true;
    }

    // если игрок умер уже в ходе своего действия
    if (!player.alive()) {
        return false;
    }

    // ==========================================================
    // 10) ХОД ВРАГОВ: движение к игроку или атака в упор
    // ==========================================================
    for (std::size_t i = 0; i < field_.enemies().size(); ++i) {
        Enemy& e = field_.enemies()[i];
        if (!e.alive()) {
            continue;
        }

        Pos enemy_pos = e.pos();
        Pos player_pos = player.pos();

        // враг двигается на 1 клетку к игроку (по x, иначе по y)
        int ddx = sign(player_pos.x - enemy_pos.x);
        int ddy = 0;
        if (ddx == 0) {
            ddy = sign(player_pos.y - enemy_pos.y);
        }

        int next_x = enemy_pos.x + ddx;
        int next_y = enemy_pos.y + ddy;

        // если следующий шаг — клетка игрока, то враг атакует
        if (next_x == player_pos.x && next_y == player_pos.y) {
            player.take_damage(e.atk());
            continue;
        }

        // иначе — двигаемся, если клетка проходимая и не занята гнездом/игроком
        if (field_.passable(next_x, next_y) &&
            !field_.nest_at(next_x, next_y) &&
            !(next_x == player_pos.x && next_y == player_pos.y)) {
            e.set_pos(Pos{ next_x, next_y });
        }
    }

    // ==========================================================
    // 11) ХОД ГНЁЗД: тик/спавн врагов
    // ==========================================================
    field_.tick_nests_and_spawn();

    // ==========================================================
    // 12) Возврат результата: жив ли игрок после полного тика
    // ==========================================================
    return player.alive();
}

// ----------------------------
// ПЕЧАТЬ ПОЛЯ (рендер в ASCII)
// ----------------------------
void Game::print() const {
    std::printf("%s", field_.render().c_str());
}
