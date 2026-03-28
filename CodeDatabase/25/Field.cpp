#include "Field.hpp"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <random>
#include <sstream>

// ==========================================================
// Field.cpp — игровое поле (мир):
// 1) генерация карты (стены/slow)
// 2) создание игрока, врагов, гнёзд
// 3) проверка занятости клеток (enemy_at / nest_at)
// 4) нанесение урона (damage_enemy_at / damage_nest_at)
// 5) ход гнёзд и спавн врагов (tick_nests_and_spawn)
// 6) выдача заклинаний и маны
// 7) отрисовка поля + HUD (render)
// 8) создание снимка состояния для сохранения (snapshot)
// ==========================================================

#include "spell/SpellDirect.hpp"
#include "spell/SpellArea.hpp"

namespace {


// 1) ГЕНЕРАТОР СЛУЧАЙНЫХ ЧИСЕЛ

std::mt19937& rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}


// 2) СЛУЧАЙНОЕ ЗАКЛИНАНИЕ

// Возвращает либо SpellDirect, либо SpellArea.

std::unique_ptr<ISpell> make_random_spell() {
    std::uniform_int_distribution<int> dist(0, 1);
    if (dist(rng()) == 0) {
        return std::make_unique<SpellDirect>(1000, 4, 1); // параметры
    }
    return std::make_unique<SpellArea>(1000, 3, 2);       

}

// 3) Манхэттенское расстояние
// Нужно для поиска цели в радиусе 
int manhattan(Pos a, Pos b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

} 


// 4) КОНСТРУКТОР ПОЛЯ: генерация карты + размещение сущностей

Field::Field(int w, int h, const GameParams& params)
    : w_(w),
      h_(h),
      cells_(w*h, Cell::Empty),                 
      enemy_hp_(std::max(0, params.enemy_hp)),  
      enemy_atk_(std::max(0, params.enemy_atk))
{
    
    if (w_ <= 2 || h_ <= 2) {
        throw std::runtime_error("Field: ширина и высота должны быть больше 2");
    }

    // --- внешняя рамка стен ---
    for (int x = 0; x < w_; ++x) {
        set(x, 0,      Cell::Wall);
        set(x, h_-1,   Cell::Wall);
    }
    for (int y = 0; y < h_; ++y) {
        set(0,     y, Cell::Wall);
        set(w_-1,  y, Cell::Wall);
    }

    // полоска на карте
    for (int y = 2; y < h_-2; ++y) {
        set(w_/3, y, Cell::Wall);
    }

    //  замедляющие клетки 
    if (w_ > 5 && h_ > 5) {
        set(2, 2, Cell::Slow);
        set(w_-3, h_-3, Cell::Slow);
    }


    const int melee_dmg    = std::max(0, params.hero_melee_dmg);
    const int ranged_dmg   = std::max(0, params.hero_ranged_dmg);
    const int base_atk     = std::min(melee_dmg, ranged_dmg);
    const int melee_bonus  = melee_dmg  - base_atk;
    const int ranged_bonus = ranged_dmg - base_atk;

    // --- создание игрока ---

    player_ = Player(std::max(0, params.hero_hp), base_atk, Pos{w_/2, h_/2},
                     melee_bonus, ranged_bonus, HandConfig{3});

    // --- стартовые ресурсы игрока ---
    player_.set_mana(1);

    // выдача 1 заклинания
    give_initial_spell_();


    // Размещение врагов 
    std::vector<Pos> enemy_spots = {
        {1, 1},
        {w_-2, h_-2},
        {w_-2, 1},
        {1, h_-2},
        {w_/2, 1},
        {w_/2, h_-2},
        {1, h_/2},
        {w_-2, h_/2}
    };

    int enemies_to_place = std::max(0, params.enemy_count);
    for (const auto& spot : enemy_spots) {
        if (enemies_to_place == 0) break;

        // ставим врага только на проходимую клетку и не на клетку игрока
        if (passable(spot.x, spot.y) && !(spot == player_.pos())) {
            enemies_.emplace_back(Enemy(enemy_hp_, enemy_atk_, spot));
            --enemies_to_place;
        }
    }


    // Размещение гнёзд 

    std::vector<Pos> nest_spots = {
        {1, h_/2},
        {w_-2, 1},
        {w_-2, h_-2},
        {1, 1},
        {w_/2, 1},
        {w_/2, h_-2}
    };

    int nests_to_place = std::max(0, params.nest_count);
    for (const auto& spot : nest_spots) {
        if (nests_to_place == 0) break;

        // ставим гнездо только если клетка свободна
        if (passable(spot.x, spot.y) && !(spot == player_.pos())) {
            if (enemy_at(spot.x, spot.y)) continue; // нельзя ставить на врага
            nests_.emplace_back(EnemyNest(spot, 5)); // period = 5 (фиксированный)
            --nests_to_place;
        }
    }
}

// ==========================================================
// 5) КОНСТРУКТОР ПОЛЯ ИЗ СОХРАНЕНИЯ (FieldState)
// ==========================================================
Field::Field(const FieldState& state)
    : w_(state.w),
      h_(state.h),
      cells_(state.cells),
      enemy_hp_(state.enemy_hp),
      enemy_atk_(state.enemy_atk),
      player_(state.player),
      enemies_(state.enemies),
      nests_(state.nests),
      kill_count_(state.kill_count)
{
    // проверки корректности загруженного состояния
    if (w_ <= 0 || h_ <= 0) {
        throw std::runtime_error("Field: invalid dimensions in loaded state");
    }
    if (static_cast<int>(cells_.size()) != w_ * h_) {
        throw std::runtime_error("Field: cell data size mismatch in loaded state");
    }
}

// ==========================================================
// 6) КОПИРОВАНИЕ (copy ctor / operator=)
// ==========================================================
Field::Field(const Field& other)
    : w_(other.w_), h_(other.h_),
      cells_(other.cells_),
      enemy_hp_(other.enemy_hp_),
      enemy_atk_(other.enemy_atk_),
      player_(other.player_),
      enemies_(other.enemies_),
      nests_(other.nests_),
      kill_count_(other.kill_count_)
{}

Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    w_ = other.w_;
    h_ = other.h_;
    cells_  = other.cells_;
    enemy_hp_ = other.enemy_hp_;
    enemy_atk_ = other.enemy_atk_;
    player_ = other.player_;
    enemies_ = other.enemies_;
    nests_   = other.nests_;
    kill_count_ = other.kill_count_;
    return *this;
}

// ==========================================================
// 7) ВСПОМОГАТЕЛЬНОЕ: найти свободную клетку рядом (4 стороны)
// ==========================================================
std::optional<Pos> Field::find_free_adjacent_(Pos around) const {
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        int nx = around.x + d[0];
        int ny = around.y + d[1];
        if (!inside(nx,ny)) continue;
        if (!passable(nx,ny)) continue;
        if (enemy_at(nx,ny)) continue;
        if (nest_at(nx,ny)) continue;
        if (player_.pos().x == nx && player_.pos().y == ny) continue;
        return Pos{nx,ny};
    }
    return std::nullopt;
}

// ==========================================================
// 8) ВСПОМОГАТЕЛЬНОЕ: найти цель (враг/гнездо) в радиусе
// ==========================================================
std::optional<Pos> Field::find_target_in_radius(Pos center, int radius) const {
    for (const auto& e : enemies_) {
        if (!e.alive()) continue;
        if (manhattan(center, e.pos()) <= radius) {
            return e.pos();
        }
    }
    for (const auto& n : nests_) {
        if (!n.alive()) continue;
        if (manhattan(center, n.pos()) <= radius) {
            return n.pos();
        }
    }
    return std::nullopt;
}

// ==========================================================
// 9) ВЫДАЧА ЗАКЛИНАНИЙ И СТАРТОВОЕ ЗАКЛИНАНИЕ
// ==========================================================
bool Field::give_random_spell_() {
    if (player_.hand().full()) return false;
    return player_.hand().add(make_random_spell());
}

void Field::give_initial_spell_() {
    if (player_.hand().size() == 0) {
        give_random_spell_();
    }
}

// ==========================================================
// 10) ПРОВЕРКИ: есть ли враг/гнездо в клетке
// ==========================================================
bool Field::enemy_at(int x, int y) const {
    for (const auto& e : enemies_) {
        if (!e.alive()) continue;
        if (e.pos().x == x && e.pos().y == y) return true;
    }
    return false;
}

bool Field::nest_at(int x, int y) const {
    for (const auto& n : nests_) {
        if (!n.alive()) continue;
        if (n.pos().x == x && n.pos().y == y) return true;
    }
    return false;
}

// ==========================================================
// 11) НАНЕСЕНИЕ УРОНА ВРАГУ / ГНЁЗДУ
// + выдача маны/заклинания за убийства
// ==========================================================
bool Field::damage_enemy_at(int x, int y, int dmg) {
    static constexpr int kills_for_spell = 3; // каждые 3 убийства — новое заклинание

    for (auto& e : enemies_) {
        if (!e.alive()) continue;
        if (e.pos().x == x && e.pos().y == y) {
            e.take_damage(dmg);

            // если враг умер — даём ману и считаем убийства
            if (!e.alive()) {
                player_.add_mana(1);
                ++kill_count_;
                if (kill_count_ % kills_for_spell == 0) {
                    give_random_spell_();
                }
            }
            return true;
        }
    }
    return false;
}

bool Field::damage_nest_at(int x, int y, int dmg) {
    static constexpr int kills_for_spell = 3;

    for (auto& n : nests_) {
        if (!n.alive()) continue;
        if (n.pos().x == x && n.pos().y == y) {
            // take_damage может вернуть true (если урон применён)
            if (n.take_damage(dmg)) {
                if (!n.alive()) {
                    ++kill_count_;
                    if (kill_count_ % kills_for_spell == 0) {
                        give_random_spell_();
                    }
                }
                return true;
            }
        }
    }
    return false;
}

// ==========================================================
// 12) ХОД ГНЁЗД: тик -> если пора, то спавним врага рядом
// ==========================================================
void Field::tick_nests_and_spawn() {
    for (auto& n : nests_) {
        if (!n.alive()) continue;

        // tick() возвращает true, если "созрел" спавн
        if (n.tick()) {
            auto spot = find_free_adjacent_(n.pos());
            if (spot) {
                enemies_.emplace_back(Enemy(enemy_hp_, enemy_atk_, *spot));
            }
            n.reset(); // сбрасываем счётчик до следующего спавна
        }
    }
}

// ==========================================================
// 13) ОТРИСОВКА ПОЛЯ + HUD (строка для печати)
// ==========================================================
std::string Field::render() const {
    std::ostringstream out;

    // --- координатная сетка сверху ---
    out << "    ";
    for (int x = 0; x < w_; ++x) out << ((x / 10) % 10);
    out << '\n';
    out << "    ";
    for (int x = 0; x < w_; ++x) out << (x % 10);
    out << '\n';

    // --- рисуем каждую клетку ---
    for (int y = 0; y < h_; ++y) {
        out << (y / 10) % 10 << (y % 10) << " ";
        for (int x = 0; x < w_; ++x) {
            char ch = '.';

            // базовый символ клетки по типу Cell
            switch (get(x,y)) {
                case Cell::Empty: ch = '.'; break;
                case Cell::Wall:  ch = '#'; break;
                case Cell::Slow:  ch = '~'; break;
            }

            // поверх клетки рисуем сущности (приоритет: Player > Enemy > Nest)
            if (player_.alive() && player_.pos().x == x && player_.pos().y == y) {
                ch = 'P';
            } else {
                bool put = false;
                for (const auto& e : enemies_) {
                    if (!e.alive()) continue;
                    if (e.pos().x == x && e.pos().y == y) { ch = 'E'; put = true; break; }
                }
                if (!put) {
                    for (const auto& nest : nests_) {
                        if (!nest.alive()) continue;
                        if (nest.pos().x == x && nest.pos().y == y) { ch = 'N'; break; }
                    }
                }
            }

            out << ch;
        }
        out << '\n';
    }

    // --- подсчёт живых врагов/гнёзд для HUD ---
    const int aliveEnemies = (int)std::count_if(
        enemies_.begin(), enemies_.end(),
        [](const Enemy& e){ return e.alive(); });
    const int aliveNests = (int)std::count_if(
        nests_.begin(), nests_.end(),
        [](const EnemyNest& n){ return n.alive(); });

    // --- HUD: hp/atk/mode/mana + счётчики ---
    out << "HP: "   << player_.hp()
        << "  ATK: " << player_.atk()
        << "  MODE: " << (player_.mode() == Player::Mode::Melee ? "Melee" : "Ranged")
        << "  MANA: " << player_.mana()
        << "  Enemies: " << aliveEnemies
        << "  Nests: "   << aliveNests
        << "\n";

    // --- список заклинаний по слотам ---
    out << "Spells: ";
    const auto& slots = player_.hand().slots();
    if (slots.empty()) {
        out << "none";
    } else {
        for (std::size_t i = 0; i < slots.size(); ++i) {
            out << (i+1) << ")" << slots[i]->name() << "[c" << slots[i]->cost() << "] ";
        }
    }
    out << "(max " << player_.hand().capacity() << ")\n";

    // --- счётчик убийств (влияет на выдачу заклинаний) ---
    out << "Kills: " << kill_count_ << "\n";

    return out.str();
}

// ==========================================================
// 14) СНИМОК СОСТОЯНИЯ ДЛЯ СОХРАНЕНИЯ (FieldState)
// ==========================================================
FieldState Field::snapshot() const {
    FieldState state;
    state.w = w_;
    state.h = h_;
    state.cells = cells_;
    state.enemy_hp = enemy_hp_;
    state.enemy_atk = enemy_atk_;
    state.player = player_;
    state.enemies = enemies_;
    state.nests = nests_;
    state.kill_count = kill_count_;
    return state;
}
