#include "Game.hpp"

#include <cctype>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "spell/SpellArea.hpp"
#include "spell/SpellDirect.hpp"

// ==========================================================
// GameIO.cpp — сохранение и загрузка игры (save/load)
// Здесь нет "геймплея": только запись/чтение состояния в файл.
// ==========================================================

namespace {

// ----------------------------
// 1) "Магическая строка" формата сохранения
// ----------------------------
// Нужна, чтобы при загрузке понять: это правильный файл и правильная версия.
const char* kSaveMagic = "OOP_LAB3_SAVE_v1";

// ----------------------------
// 2) Вспомогательная функция: перевод строки в нижний регистр
// ----------------------------
// Нужна, чтобы тип заклинания можно было читать без учёта регистра.
std::string lower_copy(std::string s) {
    for (char& ch : s) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return s;
}

// ----------------------------
// 3) Универсальный "читай или бросай исключение"
// ----------------------------
// Если в файле нет нужного значения — сразу делаем LoadError с понятным сообщением.
template <typename T>
void read_or_throw(std::istream& in, T& value, const std::string& field, const std::string& path) {
    if (!(in >> value)) {
        throw LoadError("Failed to read " + field + " from '" + path + "'");
    }
}

// ----------------------------
// 4) Кодирование клетки в символ при сохранении
// ----------------------------
// '.' - пусто, '#' - стена, '~' - замедляющая клетка
char cell_to_char(Cell c) {
    switch (c) {
        case Cell::Empty: return '.';
        case Cell::Wall:  return '#';
        case Cell::Slow:  return '~';
    }
    return '?';
}

// ----------------------------
// 5) Декодирование символа в Cell при загрузке
// ----------------------------
// Если символ неизвестен — ошибка формата сохранения.
Cell cell_from_char(char c, const std::string& path, int x, int y) {
    switch (c) {
        case '.': return Cell::Empty;
        case '#': return Cell::Wall;
        case '~': return Cell::Slow;
        default:
            throw LoadError("Unknown cell symbol at (" + std::to_string(x) + "," + std::to_string(y) + ") in '" + path + "'");
    }
}

// ----------------------------
// 6) Фабрика заклинаний при загрузке
// ----------------------------
// По строковому типу (direct/area2x2) создаёт нужный объект SpellDirect/SpellArea.
std::unique_ptr<ISpell> make_spell_from_data(const std::string& typeRaw, int dmg, int radius, int cost, const std::string& path) {
    const std::string type = lower_copy(typeRaw);

    if (type == "direct") {
        return std::make_unique<SpellDirect>(dmg, radius, cost);
    }
    if (type == "area2x2") {
        return std::make_unique<SpellArea>(dmg, radius, cost);
    }
    throw LoadError("Unknown spell type '" + typeRaw + "' in '" + path + "'");
}

} // namespace

// ==========================================================
// 7) СОХРАНЕНИЕ В ФАЙЛ
// ==========================================================
void Game::save_to_file(const std::string& path) const {
    // Открываем файл на запись (trunc = перезаписать).
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        throw SaveError("Cannot open save file '" + path + "' for writing");
    }

    // Включаем исключения потока, чтобы ловить ошибки записи.
    out.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    // Берём "снимок" состояния игры.
    const GameState state = snapshot();
    const FieldState& f = state.field;

    try {
        // -------- заголовок/формат --------
        out << kSaveMagic << "\n";

        // -------- размеры поля --------
        out << "DIMS " << f.w << " " << f.h << "\n";

        // -------- параметры врага (базовые) + счётчик убийств --------
        out << "ENEMY " << f.enemy_hp << " " << f.enemy_atk << " " << f.kill_count << "\n";

        // -------- игрок: hp atk pos mode bonuses mana + флаг skipMove --------
        out << "PLAYER "
            << f.player.hp() << " " << f.player.atk() << " "
            << f.player.pos().x << " " << f.player.pos().y << " "
            << (f.player.mode() == Player::Mode::Ranged ? 1 : 0) << " "
            << f.player.melee_bonus() << " " << f.player.ranged_bonus() << " "
            << f.player.mana() << " "
            << (state.playerSkipMove ? 1 : 0) << "\n";

        // -------- рука заклинаний --------
        // Сохраняем capacity и size, затем каждое заклинание.
        const SpellHand& hand = f.player.hand();
        out << "HAND " << hand.capacity() << " " << hand.size() << "\n";

        for (const auto& spellPtr : hand.slots()) {
            const ISpell* spell = spellPtr.get();
            if (!spell) continue;

            // Определяем реальный тип заклинания через dynamic_cast
            if (const auto* direct = dynamic_cast<const SpellDirect*>(spell)) {
                out << "SPELL direct " << direct->damage() << " " << direct->radius() << " " << direct->cost() << "\n";
            } else if (const auto* area = dynamic_cast<const SpellArea*>(spell)) {
                out << "SPELL area2x2 " << area->damage() << " " << area->radius() << " " << area->cost() << "\n";
            } else {
                // Если появится новое заклинание и оно не учтено — честная ошибка.
                throw SaveError("Unsupported spell type while saving to '" + path + "'");
            }
        }

        // -------- список врагов --------
        out << "ENEMIES " << f.enemies.size() << "\n";
        for (const auto& e : f.enemies) {
            out << e.hp() << " " << e.atk() << " " << e.pos().x << " " << e.pos().y << "\n";
        }

        // -------- список гнёзд --------
        out << "NESTS " << f.nests.size() << "\n";
        for (const auto& n : f.nests) {
            out << n.pos().x << " " << n.pos().y << " " << n.period() << " " << n.counter() << " " << n.hp() << "\n";
        }

        // -------- карта клеток --------
        out << "CELLS\n";
        for (int y = 0; y < f.h; ++y) {
            for (int x = 0; x < f.w; ++x) {
                out << cell_to_char(f.cells[static_cast<std::size_t>(y * f.w + x)]);
            }
            out << "\n";
        }

        // -------- маркер конца --------
        out << "END\n";

    } catch (const std::ios_base::failure& e) {
        // Ошибка записи в файл
        throw SaveError("Failed to write save '" + path + "': " + std::string(e.what()));
    }
}

// ==========================================================
// 8) ЗАГРУЗКА ИЗ ФАЙЛА
// ==========================================================
void Game::load_from_file(const std::string& path) {
    // Открываем файл на чтение.
    std::ifstream in(path);
    if (!in.is_open()) {
        throw LoadError("Cannot open save file '" + path + "'");
    }

    // Включаем исключения (badbit) для серьёзных ошибок чтения.
    in.exceptions(std::ifstream::badbit);

    try {
        // -------- 1) проверка заголовка --------
        std::string magic;
        read_or_throw(in, magic, "header", path);
        if (magic != kSaveMagic) {
            throw LoadError("Unsupported save format in '" + path + "'");
        }

        // -------- 2) секция DIMS --------
        std::string tag;
        read_or_throw(in, tag, "tag DIMS", path);
        if (tag != "DIMS") {
            throw LoadError("Expected DIMS section in '" + path + "'");
        }

        FieldState fstate;
        read_or_throw(in, fstate.w, "width", path);
        read_or_throw(in, fstate.h, "height", path);
        if (fstate.w <= 0 || fstate.h <= 0) {
            throw LoadError("Invalid field size in '" + path + "'");
        }

        // -------- 3) секция ENEMY --------
        read_or_throw(in, tag, "tag ENEMY", path);
        if (tag != "ENEMY") {
            throw LoadError("Expected ENEMY section in '" + path + "'");
        }
        read_or_throw(in, fstate.enemy_hp, "enemy hp", path);
        read_or_throw(in, fstate.enemy_atk, "enemy atk", path);
        read_or_throw(in, fstate.kill_count, "kill count", path);
        if (fstate.enemy_hp < 0 || fstate.enemy_atk < 0 || fstate.kill_count < 0) {
            throw LoadError("Invalid enemy parameters in '" + path + "'");
        }

        // -------- 4) секция PLAYER --------
        read_or_throw(in, tag, "tag PLAYER", path);
        if (tag != "PLAYER") {
            throw LoadError("Expected PLAYER section in '" + path + "'");
        }

        // читаем все параметры игрока
        int player_hp = 0, player_atk = 0, px = 0, py = 0;
        int mode_raw = 0;
        int melee_bonus = 0, ranged_bonus = 0;
        int mana = 0;
        int skipFlag = 0;

        read_or_throw(in, player_hp, "player hp", path);
        read_or_throw(in, player_atk, "player atk", path);
        read_or_throw(in, px, "player x", path);
        read_or_throw(in, py, "player y", path);
        read_or_throw(in, mode_raw, "player mode", path);
        read_or_throw(in, melee_bonus, "melee bonus", path);
        read_or_throw(in, ranged_bonus, "ranged bonus", path);
        read_or_throw(in, mana, "mana", path);
        read_or_throw(in, skipFlag, "skip flag", path);

        // проверки корректности
        if (player_hp < 0 || player_atk < 0 || melee_bonus < 0 || ranged_bonus < 0 || mana < 0) {
            throw LoadError("Invalid player parameters in '" + path + "'");
        }
        if (px < 0 || px >= fstate.w || py < 0 || py >= fstate.h) {
            throw LoadError("Player position is out of bounds in '" + path + "'");
        }

        // -------- 5) секция HAND --------
        read_or_throw(in, tag, "tag HAND", path);
        if (tag != "HAND") {
            throw LoadError("Expected HAND section in '" + path + "'");
        }

        int hand_capacity = 0;
        int hand_size = 0;
        read_or_throw(in, hand_capacity, "hand capacity", path);
        read_or_throw(in, hand_size, "hand size", path);

        if (hand_size < 0 || hand_capacity < 0 || hand_size > hand_capacity) {
            throw LoadError("Invalid hand configuration in '" + path + "'");
        }

        // читаем список заклинаний (hand_size штук)
        std::vector<std::unique_ptr<ISpell>> spells;
        spells.reserve(static_cast<std::size_t>(hand_size));

        for (int i = 0; i < hand_size; ++i) {
            read_or_throw(in, tag, "spell tag", path);
            if (tag != "SPELL") {
                throw LoadError("Expected SPELL entry in '" + path + "'");
            }
            std::string type;
            int dmg = 0, radius = 0, cost = 0;
            read_or_throw(in, type, "spell type", path);
            read_or_throw(in, dmg, "spell dmg", path);
            read_or_throw(in, radius, "spell radius", path);
            read_or_throw(in, cost, "spell cost", path);

            // создаём объект нужного заклинания
            spells.push_back(make_spell_from_data(type, dmg, radius, cost, path));
        }

        // -------- 6) секция ENEMIES --------
        read_or_throw(in, tag, "tag ENEMIES", path);
        if (tag != "ENEMIES") {
            throw LoadError("Expected ENEMIES section in '" + path + "'");
        }

        int enemy_count = 0;
        read_or_throw(in, enemy_count, "enemy count", path);
        if (enemy_count < 0) {
            throw LoadError("Negative enemy count in '" + path + "'");
        }

        fstate.enemies.reserve(static_cast<std::size_t>(enemy_count));
        for (int i = 0; i < enemy_count; ++i) {
            int hp = 0, atk = 0, ex = 0, ey = 0;
            read_or_throw(in, hp, "enemy hp", path);
            read_or_throw(in, atk, "enemy atk", path);
            read_or_throw(in, ex, "enemy x", path);
            read_or_throw(in, ey, "enemy y", path);

            if (hp < 0 || atk < 0) {
                throw LoadError("Invalid enemy stats in '" + path + "'");
            }
            if (ex < 0 || ex >= fstate.w || ey < 0 || ey >= fstate.h) {
                throw LoadError("Enemy position is out of bounds in '" + path + "'");
            }

            fstate.enemies.emplace_back(Enemy(hp, atk, Pos{ex, ey}));
        }

        // -------- 7) секция NESTS --------
        read_or_throw(in, tag, "tag NESTS", path);
        if (tag != "NESTS") {
            throw LoadError("Expected NESTS section in '" + path + "'");
        }

        int nest_count = 0;
        read_or_throw(in, nest_count, "nest count", path);
        if (nest_count < 0) {
            throw LoadError("Negative nest count in '" + path + "'");
        }

        fstate.nests.reserve(static_cast<std::size_t>(nest_count));
        for (int i = 0; i < nest_count; ++i) {
            int nx = 0, ny = 0, period = 0, counter = 0, hp = 0;
            read_or_throw(in, nx, "nest x", path);
            read_or_throw(in, ny, "nest y", path);
            read_or_throw(in, period, "nest period", path);
            read_or_throw(in, counter, "nest counter", path);
            read_or_throw(in, hp, "nest hp", path);

            if (period <= 0 || counter < 0 || hp < 0) {
                throw LoadError("Invalid nest parameters in '" + path + "'");
            }
            if (nx < 0 || nx >= fstate.w || ny < 0 || ny >= fstate.h) {
                throw LoadError("Nest position is out of bounds in '" + path + "'");
            }

            // создаём гнездо и восстанавливаем его счётчик/HP
            EnemyNest nest(Pos{nx, ny}, period, hp);
            nest.set_counter(counter);
            nest.set_hp(hp);
            fstate.nests.push_back(nest);
        }

        // -------- 8) секция CELLS (карта) --------
        read_or_throw(in, tag, "tag CELLS", path);
        if (tag != "CELLS") {
            throw LoadError("Expected CELLS section in '" + path + "'");
        }

        // переходим на новую строку после тега
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        fstate.cells.clear();
        fstate.cells.reserve(static_cast<std::size_t>(fstate.w * fstate.h));

        std::string line;
        for (int y = 0; y < fstate.h; ++y) {
            if (!std::getline(in, line)) {
                throw LoadError("Unexpected end of file in CELLS section of '" + path + "'");
            }
            if (!line.empty() && line.back() == '\r') line.pop_back();

            if (static_cast<int>(line.size()) != fstate.w) {
                throw LoadError("Cell line has wrong length in '" + path + "'");
            }

            for (int x = 0; x < fstate.w; ++x) {
                fstate.cells.push_back(cell_from_char(line[static_cast<std::size_t>(x)], path, x, y));
            }
        }

        // -------- 9) END маркер --------
        read_or_throw(in, tag, "end marker", path);
        if (tag != "END") {
            throw LoadError("Missing END marker in '" + path + "'");
        }

        // ==========================================================
        // 10) СОЗДАНИЕ ОБЪЕКТОВ И ПРИМЕНЕНИЕ СОСТОЯНИЯ
        // ==========================================================
        // Собираем Player, восстанавливаем режим, ману и заклинания.
        Player player(
            player_hp, player_atk, Pos{px, py},
            melee_bonus, ranged_bonus,
            HandConfig{static_cast<std::size_t>(hand_capacity)}
        );
        player.set_mana(mana);

        // проверяем режим (0/1) и приводим к нужному
        if ((mode_raw != 0 && mode_raw != 1)) {
            throw LoadError("Invalid player mode in '" + path + "'");
        }
        if ((mode_raw == 1 && player.mode() == Player::Mode::Melee) ||
            (mode_raw == 0 && player.mode() == Player::Mode::Ranged)) {
            player.toggle_mode();
        }

        // добавляем заклинания в руку игрока
        for (auto& sp : spells) {
            if (!player.hand().add(std::move(sp))) {
                throw LoadError("Not enough spell slots when loading '" + path + "'");
            }
        }

        // кладём игрока в FieldState
        fstate.player = player;

        // собираем GameState
        GameState state;
        state.field = std::move(fstate);
        state.playerSkipMove = (skipFlag != 0);

        // применяем состояние к текущей игре
        apply_state(state);

    } catch (const std::ios_base::failure& e) {
        // системная ошибка чтения файла
        throw LoadError("IO error while reading '" + path + "': " + std::string(e.what()));
    } catch (const SaveLoadError&) {
        // наши "осмысленные" ошибки — пробрасываем дальше
        throw;
    } catch (const std::exception& e) {
        // всё остальное считаем повреждённым сохранением
        throw LoadError("Invalid save '" + path + "': " + std::string(e.what()));
    }
}
