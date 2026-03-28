#include "world.hpp"

#define MAX_ATTEMPT 1000

World::World(int size_x, int size_y, Player* player_ptr){

        size_x = validate_coord(size_x);
        size_y = validate_coord(size_y);
        
        this->size_x = size_x;
        this->size_y = size_y;

        cells.resize(size_y);
        for(int y = 0; y < size_y; y++){
            cells[y].resize(size_x);
            for(int x = 0; x < size_x; x++){
                cells[y][x] = Cell(x, y);
                if (x > 1 && y > 1){
                    cells[y][x].set_type_cell(gen_state_cell());
                }
            }
        }

        if(!player_ptr) {player_ptr = new Player(20, 5);};
        this->player = player_ptr;

        cells[0][0].set_entity(player_ptr);
    }

World::World(const World& other)
    : size_x(other.size_x), size_y(other.size_y), player(nullptr){
        cells.resize(size_y);
        for(int y = 0; y < size_y; y++){
            cells[y].resize(size_x);
            for(int x = 0; x < size_x; x++){
                cells[y][x] = Cell(x, y, other.cells[y][x].get_type_cell());
                Entity* orig_entity = other.cells[y][x].get_entity();

                if (orig_entity){
                    if (dynamic_cast<Player*>(orig_entity)){
                        if(!player){
                            player = new Player(*static_cast<Player*>(orig_entity));
                        }
                        cells[y][x].set_entity(player);
                    }
                    else if (dynamic_cast<Enemy*>(orig_entity)){
                        Enemy* new_enemy = new Enemy(*static_cast<Enemy*>(orig_entity));
                        cells[y][x].set_entity(new_enemy);
                        enemies.push_back(new_enemy);
                    }
                    else if(dynamic_cast<EnemyBuild*>(orig_entity)){
                        EnemyBuild* new_enemy_build = new EnemyBuild(*static_cast<EnemyBuild*>(orig_entity));
                        cells[y][x].set_entity(new_enemy_build);
                        enemies_build.push_back(new_enemy_build);
                    } else if(dynamic_cast<EnemyTower*>(orig_entity)){
                        EnemyTower* et = new EnemyTower(*static_cast<EnemyTower*>(orig_entity));
                        cells[y][x].set_entity(et);
                        enemies_tower.push_back(et);
                    }
                }
            }
        }
    }

World& World::operator=(const World& other){
    if(this == &other) return *this;

    for (auto enemy : enemies){
        delete enemy;
    }
    enemies.clear();

    for(auto enemy_b : enemies_build){
        delete enemy_b;
    }
    enemies_build.clear();

    if (player) {
        delete player;
        player = nullptr;
    }

    size_x = other.size_x;
    size_y = other.size_y;

    cells.resize(size_y);
    for(int y = 0; y < size_y; y++) {
        cells[y].resize(size_x);
        for(int x = 0; x < size_x; x++) {
            cells[y][x] = Cell(x, y, other.cells[y][x].get_type_cell());
            Entity* orig_entity = other.cells[y][x].get_entity();

            if (orig_entity) {
                if (dynamic_cast<Player*>(orig_entity)) {
                    if(!player) {
                        player = new Player(*static_cast<Player*>(orig_entity));
                    }
                    cells[y][x].set_entity(player);
                }
                else if (dynamic_cast<Enemy*>(orig_entity)) {
                    Enemy* new_enemy = new Enemy(*static_cast<Enemy*>(orig_entity));
                    cells[y][x].set_entity(new_enemy);
                    enemies.push_back(new_enemy);
                } else if(dynamic_cast<EnemyBuild*>(orig_entity)){
                    EnemyBuild* eb = new EnemyBuild(*static_cast<EnemyBuild*>(orig_entity));
                    cells[y][x].set_entity(eb);
                    enemies_build.push_back(eb);
                } else if(dynamic_cast<EnemyTower*>(orig_entity)){
                    EnemyTower* et = new EnemyTower(*static_cast<EnemyTower*>(orig_entity));
                    cells[y][x].set_entity(et);
                    enemies_tower.push_back(et);
                }
            }
        }
    }

    return *this;
}
World::World(World&& other)
    :   size_x(other.size_x), size_y(other.size_y),
        cells(std::move(other.cells)),
        player(other.player),
        enemies(std::move(other.enemies)),
        enemies_build(std::move(other.enemies_build)),
        enemies_tower(std::move(other.enemies_tower)){
    other.player = nullptr;
}
World& World::operator=(World&& other){
    if (this == &other) return *this;

    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    if (player) {
        delete player;
    }

    size_x = other.size_x;
    size_y = other.size_y;
    cells = std::move(other.cells);
    player = other.player;
    enemies = std::move(other.enemies);
    enemies_build = std::move(other.enemies_build);
    enemies_tower = std::move(other.enemies_tower);
    
    other.player = nullptr;
    other.size_x = 0;
    other.size_y = 0;
    
    return *this;
}

World::World(int size_x, int size_y, Player* player_ptr,
    std::vector<Enemy*> enemies, std::vector<EnemyBuild*> enemies_build, std::vector<EnemyTower*> enemies_tower)
    : World(size_x, size_y, player_ptr){
    this->enemies = std::move(enemies);
    this->enemies_build = std::move(enemies_build);
    this->enemies_tower = std::move(enemies_tower);
}

World::~World(){
    for(int y = 0; y < size_y; y++) {
        for(int x = 0; x < size_x; x++) {
            cells[y][x].clear_entity();
        }
    }

    for(auto enemy : enemies) {
        if(enemy) {
            delete enemy;
        }
    }
    enemies.clear();

    for(auto enemy_b: enemies_build){
        if(enemy_b){
            delete enemy_b;
        }
    }
    enemies_build.clear();

    for(auto enemy_t : enemies_tower){
        if(enemy_t){
            delete enemy_t;
        }
    }
    enemies_tower.clear();

    if(player) {
        delete player;
        player = nullptr;
    }
}

int World::validate_coord(int coord){
    if (coord < MIN_SIZE || coord > MAX_SIZE){
        coord = MIN_SIZE;
    }
    return coord;
}

bool World::is_valid_coord(int x, int y){
    if (x >= 0 && x < size_x && y >= 0 && y < size_y){
        return true;
    }
    return false;
}

int World::random(int max_size){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, max_size - 1);

    return distrib(gen);
}

int World::get_size_x() {return size_x;};
int World::get_size_y() {return size_y;};
void World::get_sizes(int& size_x, int& size_y) {size_x = this->size_x, size_y = this->size_y;};

Cell& World::get_cell(int x, int y) {return cells[y][x];};
Cell* World::get_cell(Entity* entity) {
    for(int y = 0; y < size_y; y++){
        for(int x = 0; x < size_x; x++){
            if(cells[y][x].get_entity() == entity){
                return &cells[y][x];
            }
        }
    }

    return nullptr;
}

int World::add_enemy(int x, int y, Enemy* enemy){
    if (x < 0 || x >= size_x || y < 0 || y >= size_y){
        std::cout << "x or y out of range" << std::endl;
        return 1;
    }
    cells[y][x].set_entity(enemy);
    return 0;
}

TypeCell World::gen_state_cell(){
    int c = random(100);

    if (c < 55) return TypeCell::NORMAL;
    else if (c < 80) return TypeCell::PASSABLE;
    else if (c < 95) return TypeCell::SLOW;
    else if (c < 100) return TypeCell::MAGIC;
    return TypeCell::SLOW;
}

void World::set_cells(std::vector<std::vector<Cell>> cells) {this->cells = cells;};
void World::set_enemies(std::vector<Enemy*> enemies) {
    for(auto e : this->enemies){
        delete e;
    }
    this->enemies.clear();

    for(auto e : enemies){
        this->enemies.push_back(e);
    }
}
void World::set_enemies_build(std::vector<EnemyBuild*> enemies_build){
    for(auto e : this->enemies_build){
        delete e;
    }
    this->enemies_build.clear();

    for(auto e : enemies_build){
        this->enemies_build.push_back(e);
    }
}
void World::set_enemies_tower(std::vector<EnemyTower*> enemies_tower){
    for(auto e : this->enemies_tower){
        delete e;
    }
    this->enemies_tower.clear();

    for(auto e : enemies_tower){
        this->enemies_tower.push_back(e);
    }
}

void World::set_player(Player* player_ptr){
    if(player){
        delete player;
    }
    player = player_ptr;
}

Player& World::get_player(){return *player;};

int World::move_player(int x, int y){
    Cell* cell = get_cell(player);
    if (!cell){
        std::cout << "player not found" << std::endl;
        return 1;
    }

    int corrections_coord = 1;
    if(player->get_type_attack() == TypeAttack::RANGED){
        corrections_coord = 2;
    }
    
    int new_x = cell->get_x() + (x * corrections_coord);
    int new_y = cell->get_y() + (y * corrections_coord);
    if (!is_valid_coord(new_x, new_y)){
        std::cout << "x or y out of range" << std::endl;
        return 1;
    }

    Cell& target_cell = get_cell(new_x, new_y);

    switch (target_cell.get_type_cell()){
        case TypeCell::PASSABLE:
            std::cout << "cell is not passible" << std::endl;
            return 1;
        case TypeCell::SLOW:
            std::cout << "player is slowed" << std::endl;
            player->switch_slowed();
            break;
        case TypeCell::MAGIC:
            if (player->get_hand().get_cur_size_hand() + 1 <= player->get_hand().get_max_size_hand()){
                target_cell.set_type_cell(TypeCell::NORMAL);
                player->get_hand().add_random_card();
            }
            break;
        default:
            break;
    }

    cell->move_entity(player, target_cell);
    return 0;
}

int World::move_enemy(int x, int y, Enemy* enemy){
    Cell* cell = get_cell(enemy);
    int new_x = cell->get_x() + x;
    int new_y = cell->get_y() + y;
    if (!is_valid_coord(new_x, new_y)){
        return 1;
    }

    Cell& target_cell = get_cell(new_x, new_y);
    if (target_cell.get_type_cell() == TypeCell::PASSABLE){
        return 1;
    }

    cell->move_entity(enemy, target_cell);

    if (target_cell.get_type_cell() == TypeCell::TRAP){
        target_cell.set_type_cell(TypeCell::NORMAL);
        magic_damage(player->get_base_damage(), &target_cell);
    }

    return 0;
}

void World::gen_eneimes(int cnt_enemies, float coef){
    std::vector<std::pair<int, int>> avaible_pos;

    for(int y = 0; y < size_y; y++){
        for(int x = 0; x < size_x; x++){
            if (!cells[y][x].get_entity() && cells[y][x].get_type_cell() != TypeCell::PASSABLE){
                avaible_pos.push_back({x, y});
            }
        }
    }

    if (cnt_enemies > (int)avaible_pos.size()){
        cnt_enemies = 1;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(avaible_pos.begin(), avaible_pos.end(), g);

    for (int i = 0; i < cnt_enemies; i++) {
        int x = avaible_pos[i].first;
        int y = avaible_pos[i].second;
        
        Enemy* enemy = new Enemy(BASE_HP * coef, BASE_DAMAGE * coef);
        
        if (cells[y][x].set_entity(enemy) == 0) {
            enemies.push_back(enemy);
        } else {
            delete enemy;
        }
    }
}

void World::gen_enemies_build(int cnt_enemies_build, int required_quantity, float coef){
    int max_tower = (size_x - 5 + size_y - 5) * (size_x - 5 + size_y - 5);
    if (cnt_enemies_build < 1 || cnt_enemies_build > max_tower){
        cnt_enemies_build = 1;
    }

    if (required_quantity < 1 || required_quantity > 20){
        required_quantity = BASE_REQUIRED_QUANTITY;
    }

    for(int i = 0; i < cnt_enemies_build; i++){
        EnemyBuild* e_ptr = new EnemyBuild(required_quantity, BASE_HP_BUILD * coef);
        int x = random(size_x);
        int y = random(size_y);

        int res = cells[y][x].set_entity(e_ptr);
        while (res){
            res = cells[random(size_y)][random(size_x)].set_entity(e_ptr);
        }
        enemies_build.push_back(e_ptr);
    }
}

void World::gen_enemies_tower(int cnt_enemies_tower, float coef){
    int max_tower = (size_x - 6 + size_y - 6) * (size_x - 6 + size_y - 6);
    if (cnt_enemies_tower < 0 || cnt_enemies_tower > max_tower){
        cnt_enemies_tower = 1;
    }

    for(int i = 0; i < cnt_enemies_tower; i++){
        EnemyTower* et = new EnemyTower(BASE_HP_ENEMY_TOWER * coef, BASE_DAMAGE_ENEMY_TOWER * coef);
        int x = random(size_x);
        int y = random(size_y);

        int res = cells[y][x].set_entity(et);
        while (res){
            res = cells[random(size_y)][random(size_x)].set_entity(et);
        }
        enemies_tower.push_back(et);
    }
}

std::vector<Enemy*> World::get_enemies() {return enemies;};
std::vector<EnemyBuild*> World::get_enemies_build() {return enemies_build;};
std::vector<EnemyTower*> World::get_enemies_tower() {return enemies_tower;};

void World::push_enemy_to_vector(Enemy* enemy) {enemies.push_back(enemy);};

int World::get_cur_cooldown_e_build(){
    int current_cooldown = 0;
    for(auto eb: enemies_build){
        if (eb->is_alive()){
            current_cooldown = eb->get_required_quantity();
            break;
        }
    }
    return current_cooldown;
}

bool World::magic_damage(int damage, Cell* target_cell){
    Entity* target = target_cell->get_entity();
    if (!target) return true;

    int old_hp = target->get_hp();
    int new_hp = old_hp - damage;

    if (new_hp <= 0){
        new_hp = 0;
        target->set_hp(0);
        target_cell->set_entity(nullptr);
    } else {
        target->set_hp(new_hp);
        std::cout << "old hp: " << old_hp << "\tnew hp: " << new_hp << std::endl;
    }
    
    return true;
}

bool World::has_eny_enemies(){
    for(auto& enemy_ptr : enemies){
        if (enemy_ptr->is_alive()){
            return true;
        }
    }
    return false;
}