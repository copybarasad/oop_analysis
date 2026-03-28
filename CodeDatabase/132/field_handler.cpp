#include "field_handler.hpp"

FieldHandler::FieldHandler(int h, int w):
    landscape(h, w),
    field(h, w)
{}

FieldHandler::FieldHandler(FieldImage& img): 
    FieldHandler(img.height, img.width) 
{
    for (const Position& pos: img.stones) {
        GameObject *obj = new Stone();
        place_landscape_obj(pos, obj);
    }

    for (const Position& pos: img.swamps) {
        GameObject *obj = new Swamp();
        place_landscape_obj(pos, obj);
    }

    for (const auto& el: img.allies) {
        place_img(el.first, el.second, 'A');
    }

    for (const auto& el: img.players) {
        place_img(el.first, el.second);
    }

    for (const auto& el: img.enemies) {
        place_img(el.first, el.second, 'E');
    }

    for (const auto& el: img.enemy_towers) {
        place_img(el.first, el.second, 'T');
    }

    for (const auto& el: img.enemy_spawners) {
        place_img(el.first, el.second, 'B');
    }

    for (const auto& el: img.traps) {
        place_img(el.first, el.second);
    }
}

FieldImage FieldHandler::get_data_image() {
    FieldImage img;

    img.width = width();
    img.height = height();

    for (const Position p: pos_container['#']) {
        img.stones.push_back(p);
    }

    for (const Position p: pos_container['~']) {
        img.swamps.push_back(p);
    }

    for (const Position p: pos_container['P']) {
        if (auto obj = dynamic_cast<Player*>(field[p])) {
            img.players.insert({ p, obj->get_data_image()});
        }
    }

    for (const Position p: pos_container['A']) {
        if (auto obj = dynamic_cast<Ally*>(field[p])) {
            img.allies.insert({ p, obj->get_data_image()});
        }
    }

    for (const Position p: pos_container['E']) {
        if (auto obj = dynamic_cast<Enemy*>(field[p])) {
            img.enemies.insert({ p, obj->get_data_image()});
        }
    }

    for (const Position p: pos_container['B']) {
        if (auto obj = dynamic_cast<EnemySpawner*>(field[p])) {
            img.enemy_spawners.insert({ p, obj->get_data_image()});
        }
    }

    for (const Position p: pos_container['T']) {
        if (auto obj = dynamic_cast<EnemyTower*>(field[p])) {
            img.enemy_towers.insert({ p, obj->get_data_image()});
        }
    }

    for (const Position p: pos_container['_']) {
        if (auto obj = dynamic_cast<Trap*>(field[p])) {
            img.traps.insert({ p, obj->get_data_image()});
        }
    }

    return img;
}

void FieldHandler::place_img(const Position &pos, const PlayerImage &img) {
    GameObject *obj = new Player(img);
    place_obj(pos, obj);
}

void FieldHandler::place_img(const Position &pos, const MobImage &img, char type) {
    GameObject *obj;

    if (type == 'A') {
        obj = new Ally(img);
    }
    else if (type == 'E') {
        obj = new Enemy(img);
    }

    place_obj(pos, obj);
}

void FieldHandler::place_img(const Position &pos, const BuildingImage &img, char type) {
    GameObject *obj;

    if (type == 'T') {
        obj = new EnemyTower(img);
    }
    else if (type == 'B') {
        obj = new EnemySpawner(img);
    }

    place_obj(pos, obj);
}

void FieldHandler::place_img(const Position &pos, const TrapImage &img) {
    GameObject *obj = new Trap(img);
    place_obj(pos, obj);
}

int FieldHandler::width() const {
    return landscape.width();
}

int FieldHandler::height() const {
    return landscape.height();
}

void FieldHandler::place_landscape_obj(const Position &pos, GameObject *obj) {
    if (landscape[pos] == nullptr) {
        char screen_symbol = obj->get_screen_symbol();
        pos_container[screen_symbol].insert(pos);

        landscape[pos] = obj;

        if (field[pos] == nullptr) {
            field[pos] = obj;
        }
    }
    else {
        throw std::invalid_argument("Попытка перезаписать элемент ландшафта");
    }
}

void FieldHandler::place_obj(const Position &pos, GameObject *obj) {
    if (obj == nullptr) {
        throw std::invalid_argument("Потытка вставить недопустимый элемент поля");
    }

    if ((field[pos] == nullptr) || (landscape[pos] != nullptr && landscape[pos]->get_screen_symbol() == field[pos]->get_screen_symbol())) {
        char screen_symbol = obj->get_screen_symbol();
        pos_container[screen_symbol].insert(pos);
        field[pos] = obj;
    }
    else {
        throw std::invalid_argument("Попытка перезаписать элемент поля");
    }
}

GameObject* FieldHandler::remove_obj(const Position &pos) {
    GameObject *obj = field[pos];
    char screen_symbol = obj->get_screen_symbol();

    if (pos_container.find(screen_symbol) != pos_container.end()) {
        pos_container[screen_symbol].erase(pos);
    }

    field[pos] = landscape[pos];

    return obj;
}

void FieldHandler::move_obj(const Position &cur, const Position &new_pos) {
    if (cur == new_pos) return;

    GameObject* obj = remove_obj(cur);
    place_obj(new_pos, obj);
}


std::set<Position> FieldHandler::get_positions(char type) const {
    if (pos_container.find(type) != pos_container.end()) {
        return pos_container.at(type);
    }
    else {
        return {};
    }
}

int FieldHandler::count_positions(char type) const {
    return get_positions(type).size();
}

GameObject *FieldHandler::get_obj(const Position &pos) {
    return field[pos];
}

Player *FieldHandler::get_player() {
    const Position player_pos = *get_positions('P').begin();
    GameObject* obj = get_obj(player_pos);

    if (auto player = dynamic_cast<Player*>(obj)) {
        return player;
    }
    else {
        return nullptr;
    }
}

const Field& FieldHandler::get_field() const {
    return field;
}

std::vector<Position> FieldHandler::free_cells(const Position &top_left, const Position &bottom_right) const {
    return field.free_cells(top_left, bottom_right);
}

Position FieldHandler::get_nearest(const Position&to, std::set<char> search_cells) const {
    Position nearest = NO_POSITION;
    int min_dist = INF;

    std::set<Position> search_pos;
    for (char to_search: search_cells) {
        if (pos_container.find(to_search) == pos_container.end()) continue;
        for (auto &x: pos_container.at(to_search)) {
            search_pos.insert(x);
        }
    }

    for (const Position &pos: search_pos) {
        int cur_dist = (pos.x - to.x) * (pos.x - to.x) + (pos.y - to.y) * (pos.y - to.y);

        if (cur_dist < min_dist) {
            min_dist = cur_dist;
            nearest = pos;
        }
    }
    
    return nearest;
}