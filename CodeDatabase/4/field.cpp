#include "field.hpp"
#include "entitymanager.hpp"
#include "hero.hpp"
#include "attacktower.hpp"
#include <algorithm>

Field::Field(const Field& other)
    : engine(other.engine)
    , dice(other.dice)
    , width(other.width)
    , height(other.height)
    , generatorX(other.generatorX)
    , generatorY(other.generatorY)
    , prob(other.prob)
    , box(other.box)
    , x(other.x)
    , y(other.y)
{
}

Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    engine = other.engine;
    dice = other.dice;
    width = other.width;
    height = other.height;
    generatorX = other.generatorX;
    generatorY = other.generatorY;
    prob = other.prob;
    box = other.box;
    x = other.x;
    y = other.y;
    return *this;
}

Field::Field(Field&& other) noexcept
    : engine(std::move(other.engine))
    , dice(std::move(other.dice))
    , width(other.width)
    , height(other.height)
    , generatorX(std::move(other.generatorX))
    , generatorY(std::move(other.generatorY))
    , prob(std::move(other.prob))
    , box(std::move(other.box))
    , x(other.x)
    , y(other.y)
{
    other.width = 0;
    other.height = 0;
    other.x = 0;
    other.y = 0;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) return *this;
    engine = std::move(other.engine);
    dice = std::move(other.dice);
    width = other.width;
    height = other.height;
    generatorX = std::move(other.generatorX);
    generatorY = std::move(other.generatorY);
    prob = std::move(other.prob);
    box = std::move(other.box);
    x = other.x;
    y = other.y;
    other.width = 0;
    other.height = 0;
    other.x = 0;
    other.y = 0;
    return *this;
}

Field::Field(EntityManager& roster, int w, int h)
    : engine(std::random_device{}())
    , dice(10, 25)
    , width(w == 0 ? dice(engine) : w)
    , height(h == 0 ? dice(engine) : h)
    , generatorX(0, width - 1)
    , generatorY(0, height - 1)
    , x(generatorX(engine))
    , y(generatorY(engine))
    , prob(0.0f, 1.0f)
    , box(height, std::vector<FieldCell>(width, FieldCell(EMPTY))) {
    if (isValidPosition(x, y)) {
        box[y][x] = FieldCell(EMPTY, HERO, 0);
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == y && j == x) continue;
            float p = prob(engine);
            if (p < 0.05f) {
                box[i][j] = FieldCell(HARD);
            } else if (p < 0.15f) {
                box[i][j] = FieldCell(SLOW);
            }
        }
    }

    std::cout << "Field constructor: roster.towers.size() = " << roster.towers.size() << std::endl;
    std::cout << "Field constructor: roster.enemies.size() = " << roster.enemies.size() << std::endl;
    std::cout << "Field constructor: roster.attack_towers.size() = " << roster.attack_towers.size() << std::endl;

    for (size_t i = 0; i < roster.towers.size(); ++i) {
        auto& tower_pair = roster.towers[i];
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;
        int tx, ty;
        do {
            tx = generatorX(engine);
            ty = generatorY(engine);
            attempts++;
        } while (attempts < MAX_ATTEMPTS && (!isValidPosition(tx, ty) || !canMoveTo(tx, ty)));
        if (attempts < MAX_ATTEMPTS) {
            if (!isValidPosition(tx, ty)) {
                std::cerr << "Error: Generated invalid position (" << tx << ", " << ty << ") for tower!" << std::endl;
                continue;
            }
            if (!canMoveTo(tx, ty)) {
                std::cerr << "Error: Generated position (" << tx << ", " << ty << ") is not movable!" << std::endl;
                continue;
            }
            tower_pair.second = std::make_pair(tx, ty);
            box[ty][tx] = FieldCell(box[ty][tx].get_type(), TOWER, tower_pair.first.id);
        }
    }

    for (size_t i = 0; i < roster.enemies.size(); ++i) {
        auto& mob_pair = roster.enemies[i];
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;
        int mx, my;
        do {
            mx = generatorX(engine);
            my = generatorY(engine);
            attempts++;
        } while (attempts < MAX_ATTEMPTS && (!isValidPosition(mx, my) || !canMoveTo(mx, my)));
        if (attempts < MAX_ATTEMPTS) {
            if (!isValidPosition(mx, my)) {
                std::cerr << "Error: Generated invalid position (" << mx << ", " << my << ") for enemy!" << std::endl;
                continue;
            }
            if (!canMoveTo(mx, my)) {
                std::cerr << "Error: Generated position (" << mx << ", " << my << ") is not movable!" << std::endl;
                continue;
            }
            mob_pair.second = std::make_pair(mx, my);
            box[my][mx] = FieldCell(box[my][mx].get_type(), MOB, mob_pair.first.id);
        }
    }

    for (size_t i = 0; i < roster.attack_towers.size(); ++i) {
        auto& at_pair = roster.attack_towers[i];
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;
        int mx, my;
        do {
            mx = generatorX(engine);
            my = generatorY(engine);
            attempts++;
        } while (attempts < MAX_ATTEMPTS && (!isValidPosition(mx, my) || !canMoveTo(mx, my)));
        if (attempts < MAX_ATTEMPTS) {
            if (!isValidPosition(mx, my)) {
                std::cerr << "Error: Generated invalid position (" << mx << ", " << my << ") for attack tower!" << std::endl;
                continue;
            }
            if (!canMoveTo(mx, my)) {
                std::cerr << "Error: Generated position (" << mx << ", " << my << ") is not movable!" << std::endl;
                continue;
            }
            at_pair.second = std::make_pair(mx, my);
            box[my][mx] = FieldCell(box[my][mx].get_type(), ATTACK_TOWER, at_pair.first.id);
        }
    }
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Field::canMoveTo(int x, int y) const {
    return isValidPosition(x, y) && box[y][x].get_entity() == ZERO && box[y][x].get_type() != HARD;
}

void Field::show(const Hero& player) const {
    int result = system("clear");
    std::cout << "   "; 
    for (int j = 0; j < width; ++j) {
        std::cout << std::setw(2) << j << " ";
    }
    std::cout << "\n";
    for (int i = height - 1; i >= 0; --i) {
        if (i < 10) {
            std::cout << " " << i << " ";
        } else {
            std::cout << i << " ";
        }
        for (int j = 0; j < width; ++j) {
            FieldCell cell = box[i][j];
            char symbol = '?';
            switch (cell.get_entity()) {
                case HERO:   symbol = 'H'; break;
                case MOB:    symbol = 'E'; break;
                case TOWER:  symbol = 'T'; break;
                case ALLY:   symbol = 'A'; break;
                case ATTACK_TOWER: symbol = 'B'; break;
                case ZERO:
                    switch (cell.get_type()) {
                        case EMPTY: symbol = '.'; break;
                        case SLOW:  symbol = '='; break;
                        case HARD:  symbol = '#'; break;
                        case TRAP:  symbol = 'X'; break;
                    }
                    break;
            }
            std::cout << std::setw(2) << symbol << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\nYour position: (" << x << ", " << y << ")\n";
    std::cout << "Your health: " << player.get_hp() << "\n";
    std::cout << "Your mana: " << player.get_mana() << "\n";
    std::cout << "Your weapon: " << player.get_weapon_name() << "\n";
    std::cout << "Your damage: " << player.damage << "\n";
    std::cout << "\nLegend:\n"
              << "H - Hero\n"
              << "E - Enemy\n"
              << "T - Tower\n"
              << "A - Ally\n"
              << ". - Empty\n"
              << "= - Slow terrain\n"
              << "# - Hard (impassable)\n"
              << "X - Trap\n";
}

void Field::remove_dead_entities(EntityManager& roster) {
    for (auto it = roster.enemies.begin(); it != roster.enemies.end(); ) {
        auto& e_pair = *it;
        const Enemy& e = e_pair.first;
        if (!e.alive()) {
            int x = e_pair.second.first;
            int y = e_pair.second.second;
            if (isValidPosition(x, y) && box[y][x].get_entity() == MOB) {
                box[y][x] = FieldCell(box[y][x].get_type());
            }
            it = roster.enemies.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = roster.towers.begin(); it != roster.towers.end(); ) {
        auto& t_pair = *it;
        const Tower& t = t_pair.first;
        if (!t.alive()) {
            int x = t_pair.second.first;
            int y = t_pair.second.second;
            if (isValidPosition(x, y) && box[y][x].get_entity() == TOWER) {
                box[y][x] = FieldCell(box[y][x].get_type());
            }
            it = roster.towers.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = roster.allies.begin(); it != roster.allies.end(); ) {
        auto& a_pair = *it;
        const Ally& a = a_pair.first;
        if (!a.alive()) {
            int x = a_pair.second.first;
            int y = a_pair.second.second;
            if (isValidPosition(x, y) && box[y][x].get_entity() == ALLY) {
                box[y][x] = FieldCell(box[y][x].get_type());
            }
            it = roster.allies.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = roster.attack_towers.begin(); it != roster.attack_towers.end(); ) {
        auto& at_pair = *it;
        const AttackTower& at = at_pair.first;
        if (!at.alive()) {
            int x = at_pair.second.first;
            int y = at_pair.second.second;
            if (isValidPosition(x, y) && box[y][x].get_entity() == ATTACK_TOWER) {
                box[y][x] = FieldCell(box[y][x].get_type());
            }
            it = roster.attack_towers.erase(it);
        } else {
            ++it;
        }
    }
}