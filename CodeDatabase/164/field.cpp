#include "field.h"

Field::Field(unsigned int h = 10, unsigned int w = 10, unsigned int game_lvl = 1, unsigned int hard_lvl = 1) {
    if(h < 10) h = 10;
    if(h > 25) h = 25;
    if(w < 10) w = 10;
    if(w > 25) w = 25;

    height = h;
    width = w;

    unsigned int hero_count = 0;
    unsigned int enemy_count = 0;
    unsigned int base_count = 0;
    unsigned int tower_count = 0;
    unsigned int delay_cell_count = 0;
    unsigned int impassable_cell_count = 0;

    Randomiser& randomiser = Randomiser::get_randomiser();

    cells.resize(h);
    for(unsigned int y = 0; y < h; y++){
        cells[y].resize(w);
        for(unsigned int x = 0; x < w; x++){
            Character* character = nullptr;
            int random = randomiser.get_random(0, 99);
            if(random < 5){             // delay      5% vipalo  5% of all
                if((100 * delay_cell_count) / (h * w) < 5){
                    cells[y][x] = Cell(cell_type::delay, nullptr);
                    delay_cell_count++;
                }else{
                    cells[y][x] = Cell(cell_type::usual, nullptr);
                } 
            }else if(random < 10){      // impassable 5% vipalo  5% of all
                if((100 * impassable_cell_count) / (h * w) < 5){
                    cells[y][x] = Cell(cell_type::impassable, nullptr);
                    impassable_cell_count++;
                }else{
                    cells[y][x] = Cell(cell_type::usual, nullptr);
                }                    
            }else if(random < 13){      // enemy      3% vipalo  3% of all
                if((100 * enemy_count) / (h * w) < 3){
                    character = new Enemy(game_lvl, hard_lvl);
                    cells[y][x] = Cell(cell_type::usual, character);
                    enemy_count++;
                    character_coord.push_back({x, y});
                }else{
                    cells[y][x] = Cell(cell_type::usual, nullptr);
                }
            }else if(random < 15){      // tower      2% vipalo  1% of all
                if((100 * tower_count) / (h * w) < 1){
                    character = new Tower(game_lvl, hard_lvl);
                    cells[y][x] = Cell(cell_type::usual, character);
                    tower_count++;
                    character_coord.push_back({x, y});
                }else{
                    cells[y][x] = Cell(cell_type::usual, nullptr);
                }
                    
            }else if(random < 17){      // hero       2% vipalo  1
                if(hero_count == 0){
                    character = new Hero(game_lvl, hard_lvl);
                    cells[y][x] = Cell(cell_type::usual, character);
                    hero_count++;
                    hero_coord = {x, y};
                }else{
                    cells[y][x] = Cell(cell_type::usual, nullptr);
                }
            }else if(random < 19){      // base       2% vipalo  1% of all
                if((100 * base_count) / (h * w) < 1){
                    character = new Base(game_lvl, hard_lvl);
                    cells[y][x] = Cell(cell_type::usual, character);
                    base_count++;
                    character_coord.push_back({x, y});
                }else{
                    cells[y][x] = Cell(cell_type::usual, nullptr);
                }
            }else{                      // usual     81% vipalo  all else
                cells[y][x] = Cell(cell_type::usual, nullptr);
            }
        }
    }

    while(hero_count == 0 || enemy_count == 0 || base_count == 0 || tower_count == 0){
        unsigned int xr = (unsigned int)randomiser.get_random(0, width - 1);
        unsigned int yr = (unsigned int)randomiser.get_random(0, height - 1);
        if(cells[yr][xr].type == cell_type::usual && !(cells[yr][xr].occupied)){
            Character* character = nullptr;
            if(hero_count == 0){
                character = new Hero(game_lvl, hard_lvl);
                cells[yr][xr] = Cell(cell_type::usual, character);
                hero_count++;
                hero_coord = {xr, yr};
            }else if(enemy_count == 0){
                character = new Enemy(game_lvl, hard_lvl);
                cells[yr][xr] = Cell(cell_type::usual, character);
                enemy_count++;
                character_coord.push_back({xr, yr});
            }else if(base_count == 0){
                character = new Base(game_lvl, hard_lvl);
                cells[yr][xr] = Cell(cell_type::usual, character);
                base_count++;
                character_coord.push_back({xr, yr});
            }else if(tower_count == 0){
                character = new Tower(game_lvl, hard_lvl);
                cells[yr][xr] = Cell(cell_type::usual, character);
                tower_count++;
                character_coord.push_back({xr, yr});
            }
        }
    }
};

Field::Field(const Field& field): height(field.height), width(field.width), hero_coord(field.hero_coord),
        character_coord(field.character_coord) { 
    cells.resize(height);
    for(unsigned int y = 0; y < height; y++){
        cells[y].resize(width);
        for(unsigned int x = 0; x < width; x++){
            cells[y][x] = field.cells[y][x];
        }
    }
};

Field& Field::operator=(const Field& field){
    if(this == &field) return *this;

    height = field.height;
    width = field.width;
    cells.resize(height);
    for(unsigned int y = 0; y < height; y++){
        cells[y].resize(width);
        for(unsigned int x = 0; x < width; x++){
            cells[y][x] = field.cells[y][x];
        }
    }

    hero_coord = field.hero_coord;
    character_coord = field.character_coord;

    return *this;
};

Field::Field(Field&& field) noexcept: height(field.height), width(field.width), cells(std::move(field.cells)), 
        hero_coord(std::move(field.hero_coord)), character_coord(std::move(field.character_coord)) { // peremeschtschenie
    field.height = 0;
    field.width = 0;
};

Field& Field::operator=(Field&& field) noexcept {
    if(this == &field) return *this;

    height = field.height;
    width = field.width;
    cells = std::move(field.cells);
    hero_coord = std::move(field.hero_coord);
    character_coord = std::move(field.character_coord);

    field.height = 0;
    field.width = 0;
    
    return *this;
};

Field::~Field(){
    for(unsigned int y = 0; y < height; y++) {
        for(unsigned int x = 0; x < width; x++) {
            if(cells[y][x].occupied != nullptr) {
                delete cells[y][x].occupied; 
            }
        }
    }
};

bool Field::damage_cell(unsigned int x, unsigned int y, unsigned int damage){
    if(!(0 <= y && y < height && 0 <= x && x < width)) return false;
    if(!(cells[y][x].occupied)) return false;

    bool living = cells[y][x].occupied->get_damage(damage);
    bool give_spell = false;
    if(!living){
        if(cells[y][x].occupied->get_type() != character_type::hero){
            Hero* hero = static_cast<Hero*>(cells[hero_coord[1]][hero_coord[0]].occupied);
            hero->get_money(cells[y][x].occupied->give_points());

            std::array<unsigned int, 2> coords = {x, y};
            auto it = std::find(character_coord.begin(), character_coord.end(), coords);
            if(it != character_coord.end()){
                character_coord.erase(it);
            }
        }

        if(cells[y][x].occupied->get_type() == character_type::enemy){
            give_spell = true;
        }

        delete cells[y][x].occupied;
        cells[y][x].occupied = nullptr;
    }
    return give_spell;
};

void Field::move_character(unsigned int old_x, unsigned int old_y, unsigned int target_x, unsigned int target_y){
    if(!cells[old_y][old_x].occupied->can_move()){
        cells[old_y][old_x].occupied->change_move();
        return;
    }

    if(target_x < 0) target_x = 0;
    if(target_y < 0) target_y = 0;
    if(target_x > width) target_x = width - 1;
    if(target_y > height) target_y = height - 1;

    if(old_x == target_x && old_y == target_y) return;
        
    int dx = target_x - old_x;
    int dy = target_y - old_y;
    int x = 0;
    int y = 0;

    if(dx != 0){
        x = abs(dx) / dx; // 1 or -1
    }
    if(dy != 0){
        y = abs(dy) / dy; // 1 or -1
    }
    dx = abs(dx);
    dy = abs(dy);

    if((cells[old_y][old_x + x].type == cell_type::impassable || cells[old_y][old_x + x].occupied) && 
            (cells[old_y + y][old_x].type == cell_type::impassable || cells[old_y + y][old_x].occupied)){
        return;
    }

    unsigned int new_x = old_x;
    unsigned int new_y = old_y;

    if(dx >= dy && !(cells[old_y][old_x + x].type == cell_type::impassable || cells[old_y][old_x + x].occupied)){
        new_x += x;
    }else if(dx <= dy && !(cells[old_y + y][old_x].type == cell_type::impassable || cells[old_y + y][old_x].occupied)){
        new_y += y;
    }else if(!(cells[old_y][old_x + x].type == cell_type::impassable || cells[old_y][old_x + x].occupied)){
        new_x += x;
    }else if(!(cells[old_y + y][old_x].type == cell_type::impassable || cells[old_y + y][old_x].occupied)){
        new_y += y;
    }

    if(old_x == new_x && old_y == new_y) return;
        
    cells[new_y][new_x].occupied = cells[old_y][old_x].occupied;
    cells[old_y][old_x].occupied = nullptr;
    if(cells[new_y][new_x].type == cell_type::delay){
        cells[new_y][new_x].occupied->change_move();
    }

    std::array<unsigned int, 2> coords = {new_x, new_y};
    if(cells[new_y][new_x].occupied->get_type() == character_type::hero){
        hero_coord = coords;
    }else{
        coords = {old_x, old_y};
        auto it = std::find(character_coord.begin(), character_coord.end(), coords);
        if(it != character_coord.end()){
            coords = {new_x, new_y};
            *it = coords;
        }
    }

    if(cells[new_y][new_x].trap){
        this->damage_cell(x, y, cells[new_y][new_x].use_trap());
    }
};

void Field::spawn_character(unsigned int x, unsigned int y, Character* character){
    unsigned int dx = 0;
    unsigned int dy = 0;
    if(0 <= y - 1 && y - 1 < height && 
            !(cells[y - 1][x].type == cell_type::impassable || cells[y - 1][x].occupied)){   
        dy = -1;
    }else if(0 <= x + 1 && x + 1 < width && 
            !(cells[y][x + 1].type == cell_type::impassable || cells[y][x + 1].occupied)){   
        dx = 1;
    }else if(0 <= y + 1 && y + 1 < height && 
            !(cells[y + 1][x].type == cell_type::impassable || cells[y + 1][x].occupied)){ 
        dy = 1;
    }else if(0 <= x - 1 && x - 1 < width && 
            !(cells[y][x - 1].type == cell_type::impassable || cells[y][x - 1].occupied)){   
        dx = -1;
    }

    if(dx !=0 || dy != 0){
        cells[y + dy][x + dx].occupied = character;
        character_coord.push_back({x + dx, y + dy});

        if(cells[y + dy][x + dx].trap){
            this->damage_cell(x + dx, y + dy, cells[y + dy][x + dx].damage);
        }
    }else{
        delete character;
        character = nullptr;
    }
};

void make_enemy(unsigned int x, unsigned int y, unsigned int game_lvl, unsigned int hard_lvl){
    if(cells[y][x].occupied->get_type() != character_type::base) return;
    Base* base = static_cast<Base*>(cells[y][x].occupied);
    bool make = base->make_enemy();
    if(!make) return;

    Character* character = new Enemy(game_lvl, hard_lvl);
    this->spawn_character(x, y, character);
};

void Field::call_ally(unsigned int x, unsigned int y, unsigned int game_lvl, unsigned int hard_lvl){
    Character* character = new Ally(game_lvl, hard_lvl);
    this->spawn_character(x, y, character);
};

void Field::set_trap(unsigned int x, unsigned int y, unsigned int damage){
    cells[y][x].set_trap(damage);
};

json Field::to_json() const {
    json j;
    j["height"] = height;
    j["width"] = width;
    
    json delay_cells_array = json::array();
    json impassable_cells_array = json::array();
    json trap_array = json::array();
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            json cell_json;
            if(cells[y][x].type == cell_type::delay){
                cell_json["x"] = x;
                cell_json["y"] = y;
                delay_cells_array.push_back(cell_json);
            }else if(cells[y][x].type == cell_type::impassable){
                cell_json["x"] = x;
                cell_json["y"] = y;
                impassable_cells_array.push_back(cell_json);
            }

            if(cells[y][x].trap){
                cell_json["x"] = x;
                cell_json["y"] = y;
                cell_json["damage"] = cells[y][x].damage;
                trap_array.push_back(cell_json);
            }
        }
    }
    j["delay_cells"] = delay_cells_array;
    j["impassable_cells"] = impassable_cells_array;
    j["traps"] = trap_array;

    json hero_json;
    Hero* hero = static_cast<Hero*>(cells[hero_coord[1]][hero_coord[0]].occupied);
    hero_json["x"] = hero_coord[0];
    hero_json["y"] = hero_coord[1];
    hero_json["hp"] = hero->get_hp();
    hero_json["money"] = hero->get_money();
    hero_json["move"] = hero->can_move();
    j["hero"] = hero_json;
        
    json characters_array = json::array();
    for (const auto& coord : character_coord) {
        json char_json;
        Character* character = cells[coord[1]][coord[0]].occupied;
        char_json["x"] = coord[0];
        char_json["y"] = coord[1];
        char_json["type"] = static_cast<int>(character->get_type());
        char_json["hp"] = character->get_hp();
        char_json["move"] = character->can_move();            
        characters_array.push_back(char_json);
    }
    j["character_coords"] = characters_array;
        
    return j;
};
    
void Field::from_json(const json& j, unsigned int game_lvl, unsigned int hard_lvl) {
    height = j["height"];
    width = j["width"];
        
    cells.clear();
    character_coord.clear();
        
    int delay_cells_index = 0;
    int impassable_cells_index = 0;
    int trap_index = 0;

    const auto& delay_cells_array = j["delay_cells"];
    const auto& impassable_cells_array = j["impassable_cells"];
    const auto& trap_array = j["traps"];

    cells.resize(height);
    for (unsigned int y = 0; y < height; y++) {
        cells[y].resize(width);
        for (unsigned int x = 0; x < width; x++) {
            if(delay_cells_index < delay_cells_array.size() 
                    && delay_cells_array[delay_cells_index]["x"] == x 
                    && delay_cells_array[delay_cells_index]["y"] == y){
                cells[y][x].type = cell_type::delay;
                delay_cells_index++;
            }else if(impassable_cells_index < impassable_cells_array.size() 
                    && impassable_cells_array[impassable_cells_index]["x"] == x 
                    && impassable_cells_array[impassable_cells_index]["y"] == y){
                cells[y][x].type = cell_type::impassable;
                impassable_cells_index++;
            }else{
                cells[y][x].type = cell_type::usual;
            }

            if(trap_index < trap_array.size() && trap_array[trap_index]["x"] == x && 
                    trap_array[trap_index]["y"] == y){                cells[y][x].trap = true;
                cells[y][x].damage = trap_array[trap_index]["damage"];
                trap_index++;
            }else{
                cells[y][x].trap = false;
                cells[y][x].damage = 0;
            }
        }
    }

    hero_coord[0] = j["hero"]["x"];
    hero_coord[1] = j["hero"]["y"];
    unsigned int hp = j["hero"]["hp"];
    unsigned int money = j["hero"]["money"];
    bool move = j["hero"]["move"];
    cells[hero_coord[1]][hero_coord[0]].occupied = new Hero(game_lvl, hard_lvl, hp, money, move);

    const auto& characters_array = j["character_coords"];
    for (int index = 0; index < characters_array.size(); index++) {
        const auto& char_json = characters_array[index];
        unsigned int x = char_json["x"];
        unsigned int y = char_json["y"];
        hp = char_json["hp"];
        move = char_json["move"];
        character_type type = static_cast<character_type>(char_json["type"]);
        switch(type){
            case character_type::ally:
                cells[y][x].occupied = new Ally(game_lvl, hard_lvl, hp, move);
                break;
            case character_type::enemy:
                cells[y][x].occupied = new Enemy(game_lvl, hard_lvl, hp, move);
                break;
            case character_type::base:
                cells[y][x].occupied = new Base(game_lvl, hard_lvl, hp);
                break;
            case character_type::tower:
                cells[y][x].occupied = new Tower(game_lvl, hard_lvl, hp);
                break;
        }
        character_coord.push_back({x, y});
    }
};