#include "field.hpp"
#include <assert.h>

Field::Field(const int rows, const int cols) {
    // TODO(?) выбросить ошибку
    h = std::max(10, std::min(25, rows));
    w = std::max(10, std::min(25, cols));

    field = std::vector<std::vector<GameObject*>>(h, std::vector<GameObject*>(w, nullptr));
}

// TODO:
// Field::~Field() {
//     for (int i = 0; i < h; i++) {
//         for (int j = 0; j < w; j++) {
//             delete field[i][j];
//             field[i][j] = nullptr;
//         }
//     }
// }

Field::Field(const Field &to_copy): Field(to_copy.h, to_copy.w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            GameObject* cell = to_copy.field[i][j];

            if (cell != nullptr) {
                this->field[i][j] = to_copy.field[i][j]->clone();
            }
        }
    }
}

Field &Field::operator=(const Field &other) {
    if (this != &other) {
        Field *copy = new Field{other};

        std::swap(field, copy->field);
        std::swap(h, copy->h);
        std::swap(w, copy->w);

        // new (this) Field(other); - плохо
    }

    return *this;
}

Field &Field::operator=(Field &&other) {
    if (this != &other) {
        std::swap(field, other.field);
        std::swap(h, other.h);
        std::swap(w, other.w);
    }

    return *this;
}

Field::Field(Field &&to_move) {
    *this = std::move(to_move);
}

GameObject*& Field::operator [](const Position &pos) {
    return field[pos.x][pos.y];
}

GameObject* Field::operator [](const Position &pos) const {
    return field[pos.x][pos.y];
}

int Field::height() const {
    return h;
}

int Field::width() const {
    return w;
}

bool Field::in_bounds(const Position &pos) const {
    return (pos.x >= 0) && (pos.y >= 0) && (pos.x < h) && (pos.y < w);
}

std::vector<Position> Field::filter(const Position &top_left, const Position &bottom_right, const std::set<char> &to_include) const {
    bool allow_empty = to_include.find('.') != to_include.end();

    std::vector<Position> res;
    
    for (int i = top_left.x; i <= bottom_right.x; i++) {
        for (int j = top_left.y; j <= bottom_right.y; j++) {
            if (!in_bounds({ i, j })) continue;

            if (field[i][j] != nullptr) {
                char s = field[i][j]->get_screen_symbol();
                if (to_include.find(s) != to_include.end()) {
                    res.push_back({ i, j });
                }
            }
            else {
                if (allow_empty) {
                    res.push_back({ i, j });
                }
            }
        }
    }

    return res;
}

std::vector<Position> Field::filter(const std::set<char> &to_include) const {
    return filter({0, 0}, {h - 1, w - 1}, to_include);
}

std::vector<Position> Field::free_cells(const Position &top_left, const Position &bottom_right) const {
    return filter(top_left, bottom_right, {'.'});
}

std::vector<Position> Field::free_cells() const {
    return filter({'.'});
}