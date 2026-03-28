#include "setup.hpp"
#include <iostream>
#include <sstream>
#include <set>
#include <limits>
#include <algorithm>
#include <cctype>

// убираем пробелы по краям строки
static std::string trim_copy(std::string s) {
    auto isw = [](unsigned char c){ return std::isspace(c); };
    while (!s.empty() && isw(s.front())) s.erase(s.begin());
    while (!s.empty() && isw(s.back()))  s.pop_back();
    return s;
}

// читаем целое с ограничениями и значением по умолчанию
static int read_int(const std::string& prompt, int lo, int hi, int defVal) {
    for (;;) {
        std::cout << prompt << " [" << lo << ".." << hi << "] (default " << defVal << "): " << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) return defVal;   // eof -> берём дефолт
        auto t = trim_copy(line);
        if (t.empty()) return defVal;
        std::istringstream is(t);
        int v; if (is >> v && v >= lo && v <= hi) return v;
        std::cout << "неверное значение, попробуйте снова\n";
    }
}

// читаем одну позицию формата "x y" с проверкой границ
static Position read_pos_single(const std::string& prompt, int w, int h) {
    for (;;) {
        std::cout << prompt << " (x y): ";
        std::string line; if (!std::getline(std::cin, line)) return Position{0,0};
        auto t = trim_copy(line);
        std::istringstream is(t);
        int x, y;
        if (is >> x >> y) {
            if (0 <= x && x < w && 0 <= y && y < h) return Position{x,y};
            std::cout << "вне поля, допустимо: 0.." << w-1 << " 0.." << h-1 << "\n";
        } else {
            std::cout << "некорректный формат, пример: 3 5\n";
        }
    }
}

// читаем список позиций, заканчиваем пустой строкой или done
static std::vector<Position> read_positions_list(const std::string& title, int w, int h) {
    std::cout << title << "\n";
    std::cout << "введите пары координат (x y), пустая строка / 'done' — конец\n";
    std::vector<Position> out;
    std::set<Position> seen;

    for (;;) {
        std::cout << "> " << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) break;
        auto t = trim_copy(line);
        if (t.empty() || t == "done" || t == "DONE") break;

        std::istringstream is(t);
        int x, y;
        if (!(is >> x >> y)) {
            std::cout << "пример: 6 2\n";
            continue;
        }
        if (x < 0 || x >= w || y < 0 || y >= h) {
            std::cout << "вне поля, допустимо: 0.." << w-1 << " 0.." << h-1 << "\n";
            continue;
        }
        Position p{x,y};
        if (!seen.insert(p).second) {
            std::cout << "эта клетка уже добавлена, пропускаю\n";
            continue;
        }
        out.push_back(p);
    }
    return out;
}

// спрашиваем все параметры игры
GameParams prompt_game_params() {
    GameParams p{};

    p.width  = read_int("ширина поля", 10, 25, p.width);
    p.height = read_int("высота поля", 10, 25, p.height);

    p.heroHp  = read_int("hp героя", 1, 999, p.heroHp);
    p.heroDmg = read_int("урон героя", 1, 99, p.heroDmg);
    p.heroStart = read_pos_single("координаты героя", p.width, p.height);
    p.spawnPeriod = read_int("период спавна (ходов)", 1, 20, p.spawnPeriod);

    p.spawners = read_positions_list("координаты спавнеров", p.width, p.height);
    p.walls    = read_positions_list("координаты стен (непроходимые клетки)", p.width, p.height);
    p.slows    = read_positions_list("координаты slow-клеток (пропуск следующего хода)", p.width, p.height);

    return p;
}

// базовая валидация, чтобы не было конфликтов
static bool validate_no_conflicts(const GameParams& p) {
    auto in_bounds = [&](const Position& q){
        return (0 <= q.x && q.x < p.width && 0 <= q.y && q.y < p.height);
    };
    auto has = [](const std::vector<Position>& v, const Position& t){
        return std::find(v.begin(), v.end(), t) != v.end();
    };
    if (!in_bounds(p.heroStart)) {
        std::cout << "ошибка: герой вне поля\n"; return false;
    }
    if (has(p.walls, p.heroStart)) {
        std::cout << "ошибка: герой на стене\n"; return false;
    }
    if (has(p.spawners, p.heroStart)) {
        std::cout << "ошибка: герой на спавнере\n"; return false;
    }

    for (auto s : p.spawners) {
        if (!in_bounds(s)) { std::cout << "спавнер вне поля\n"; return false; }
        if (has(p.walls, s)) { std::cout << "спавнер на стене\n"; return false; }
    }
    for (auto w : p.walls) if (!in_bounds(w)) { std::cout << "стена вне поля\n"; return false; }
    for (auto s : p.slows) if (!in_bounds(s)) { std::cout << "slow вне поля\n"; return false; }

    return true;
}

// строим объекты по параметрам
bool build_world_from_params(const GameParams& p,
                             Field& outField,
                             Hero& outHero,
                             std::vector<Spawner>& outSpawners)
{
    if (!validate_no_conflicts(p)) return false;

    outField = Field{p.width, p.height};

    for (auto w : p.walls) outField.at(w.x, w.y).setType(CellType::Wall);
    for (auto s : p.slows) outField.at(s.x, s.y).setType(CellType::Slow);

    outHero = Hero{p.heroHp, p.heroDmg, p.heroStart};

    outSpawners.clear();
    outSpawners.reserve(p.spawners.size());
    for (auto s : p.spawners) outSpawners.emplace_back(Spawner{s, p.spawnPeriod});

    return true;
}