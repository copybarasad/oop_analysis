#include "setup.hpp"
#include <iostream>
#include <sstream>
#include <set>
#include <limits>
#include <algorithm>
#include <cctype>

// читаем целое в диапазоне, с дефолтом
static int read_int(const std::string& prompt, int lo, int hi, int defVal) {
    for (;;) {
        std::cout << prompt << " [" << lo << ".." << hi << "] (default " << defVal << "): " << std::flush;
        std::string line; 
        if (!std::getline(std::cin, line)) return defVal;   // eof -> дефолт
        auto t = trim_copy(line);
        if (t.empty()) return defVal;
        std::istringstream is(t);
        int v; if (is >> v && v >= lo && v <= hi) return v;
        std::cout << "Неверное значение. Повторите.\n";
    }
}

// читаем одну позицию (x y)
static Position read_pos_single(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " (x y): ";
        std::string line; std::getline(std::cin, line);
        std::istringstream is(line);
        int x, y; if (is >> x >> y) return Position{x,y};
        std::cout << "Некорректный формат. Пример: 3 5\n";
    }
}

// трим пробельчиков
std::string trim_copy(std::string s) {
    auto isw = [](unsigned char c){ return std::isspace(c); };
    while (!s.empty() && isw(s.front())) s.erase(s.begin());
    while (!s.empty() && isw(s.back()))  s.pop_back();
    return s;
}

// читаем список позиций, с валидацией, без дублей
static std::vector<Position> read_positions_list(const std::string& title,
                                                 int w, int h)
{
    std::cout << title << "\n";
    std::cout << "Введите координаты по одной паре на строку (x y).\n"
                 "Пустая строка / пробелы / 'done' — закончить ввод.\n";
    std::vector<Position> out;
    std::set<Position> seen;

    for (;;) {
        std::cout << "> " << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) break;          // eof -> выходим
        auto t = trim_copy(line);
        if (t.empty() || t == "done" || t == "DONE") break; // пустая/пробелы/'done'

        std::istringstream is(t);
        int x, y;
        if (!(is >> x >> y)) {
            std::cout << "Некорректный ввод. Пример: 6 2\n";
            continue;
        }
        if (x < 0 || x >= w || y < 0 || y >= h) {
            std::cout << "Вне поля. Допустимо: 0.."<<w-1<<" 0.."<<h-1<<"\n";
            continue;
        }
        Position p{x,y};
        if (!seen.insert(p).second) {
            std::cout << "Дублирование координаты, пропущено.\n";
            continue;
        }
        out.push_back(p);
    }
    return out;
}

GameParams prompt_game_params() {
    GameParams p{};

    // поле
    p.width  = read_int("Ширина поля", 10, 25, p.width);
    p.height = read_int("Высота поля", 10, 25, p.height);

    // герой
    p.heroHp  = read_int("HP героя", 1, 999, p.heroHp);
    p.heroDmg = read_int("Урон героя", 1, 99, p.heroDmg);
    p.heroStart = read_pos_single("Координаты героя");

    // размер руки (лимит карт)
    p.handSize = read_int("Размер руки (кол-во слотов карт)", 1, 9, p.handSize);

    // спавн
    p.spawnPeriod = read_int("Период спавна (ходов)", 1, 20, p.spawnPeriod);

    // объекты
    p.spawners = read_positions_list("Координаты СПАВНЕРОВ", p.width, p.height);
    p.walls    = read_positions_list("Координаты СТЕН (непроходимые клетки)", p.width, p.height);
    p.slows    = read_positions_list("Координаты SLOW-клеток (пропуск следующего хода)", p.width, p.height);

    return p;
}

static bool validate_no_conflicts(const GameParams& p) {
    auto in_bounds = [&](const Position& q){
        return (0 <= q.getX() && q.getX() < p.width && 0 <= q.getY() && q.getY() < p.height);
    };

    if (!in_bounds(p.heroStart)) {
        std::cout << "Ошибка: герой вне поля.\n"; return false;
    }

    auto has = [](const std::vector<Position>& v, const Position& t){
        return std::find(v.begin(), v.end(), t) != v.end();
    };

    if (has(p.walls, p.heroStart)) {
        std::cout << "Ошибка: герой на стене.\n"; return false;
    }
    if (has(p.spawners, p.heroStart)) {
        std::cout << "Ошибка: герой на спавнере.\n"; return false;
    }

    for (auto s : p.spawners) {
        if (!in_bounds(s)) { std::cout << "Спавнер вне поля.\n"; return false; }
        if (has(p.walls, s)) { std::cout << "Спавнер на стене.\n"; return false; }
    }

    for (auto w : p.walls) {
        if (!in_bounds(w)) { std::cout << "Стена вне поля.\n"; return false; }
    }
    for (auto s : p.slows) {
        if (!in_bounds(s)) { std::cout << "Slow вне поля.\n"; return false; }
    }

    if (p.handSize < 1 || p.handSize > 9) {
        std::cout << "Ошибка: размер руки вне диапазона (1..9).\n"; return false;
    }

    return true;
}

bool build_world_from_params(const GameParams& p,
                             Field& outField,
                             Hero& outHero,
                             std::vector<Spawner>& outSpawners)
{
    if (!validate_no_conflicts(p)) return false;

    outField = Field{p.width, p.height};

    for (auto w : p.walls) outField.at(w.getX(), w.getY()).setType(CellType::Wall);
    for (auto s : p.slows) outField.at(s.getX(), s.getY()).setType(CellType::Slow);

    outHero = Hero{p.heroHp, p.heroDmg, p.heroStart};

    outSpawners.clear();
    outSpawners.reserve(p.spawners.size());
    for (auto s : p.spawners) outSpawners.emplace_back(Spawner{s, p.spawnPeriod});

    return true;
}
