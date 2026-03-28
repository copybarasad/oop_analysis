#include "game.hpp"

Game::Game(int size_x, int size_y, Player* player_ptr, int cooldown_tower,
     bool friendly_fire) : friendly_fire(friendly_fire), cooldown_tower(cooldown_tower){
    world = new World(size_x, size_y, player_ptr);
}
Game::Game(World* world_ptr, bool active, bool player_turn, bool friendly_fire, 
         int build_cooldown, int tower_cooldown) 
        : is_active(active), is_player_turn(player_turn),
          friendly_fire(friendly_fire), cooldown_build(build_cooldown),
          cooldown_tower(tower_cooldown), world(world_ptr) {};
Game::~Game(){
    delete world;
}
Game::Game(const Game& other) 
        : is_active(other.is_active),
          is_player_turn(other.is_player_turn),
          friendly_fire(other.friendly_fire),
          cooldown_build(other.cooldown_build),
          cooldown_tower(other.cooldown_tower),
          world(new World(*other.world)),
          level(other.level) {}
Game& Game::operator=(const Game& other){
    if (this != &other) {
        is_active = other.is_active;
        is_player_turn = other.is_player_turn;
        friendly_fire = other.friendly_fire;
        cooldown_build = other.cooldown_build;
        cooldown_tower = other.cooldown_tower;
        level = other.level;
        
        if (world) delete world;
        world = new World(*other.world);
    }
    return *this;
}

Game& Game::operator=(Game&& other) noexcept{
    if (this != &other) {
        is_active = other.is_active;
        is_player_turn = other.is_player_turn;
        friendly_fire = other.friendly_fire;
        cooldown_build = other.cooldown_build;
        cooldown_tower = other.cooldown_tower;
        level = other.level;
        
        if (world) delete world;
        world = other.world;
        other.world = nullptr;
    }
    return *this;
}

void Game::process_enemy(){
    // Процесс хода enemy
    Cell* player_cell = world->get_cell(&world->get_player());
    for (auto& enemy_ptr : world -> get_enemies()){
        if (!enemy_ptr || !enemy_ptr->is_alive()) continue;
        
        Cell* enemy_cell = world->get_cell(enemy_ptr);

        if (!enemy_cell || !player_cell) continue;

        int directions[4][2] = {
            {0, 1},  // w
            {-1, 0}, // a
            {0, -1}, // s
            {1, 0},  // d
        };
        int priority[4] = {0, 1, 2, 3};

        for(int i = 0; i < 4; i++){
            int new_x = enemy_cell->get_x() + directions[i][0];
            int new_y = enemy_cell->get_y() + directions[i][1];

            if (world->is_valid_coord(new_x, new_y)){
                int dist_to_player = abs(player_cell->get_x() - new_x)
                    + abs(player_cell->get_y() - new_y);
                
                priority[i] = -dist_to_player;
            } else{
                priority[i] = -100000;
            }
        }

        bool moved = false;
        for(int i = 0; i < 4 && !moved; i++){
            int best_index = 0;
            for(int j = 0; j < 4; j++){
                if (priority[j] > priority[best_index]){
                    best_index = j;
                }
            }
            priority[best_index] = -1000;

            if(!world->move_enemy(directions[best_index][0], directions[best_index][1],
                enemy_ptr)){
                moved = true;
            }
        }
    }

    // Процесс хода вражеских зданий
    if (cooldown_build >= world->get_cur_cooldown_e_build()){
        for(auto eb : world->get_enemies_build()){

            if (!eb || !eb->is_alive()) continue;

            Cell* build_cell = world->get_cell(eb);

            if(!build_cell) continue;

            int attempt = 0;
            bool is_spawn = false;
            float coef = 1 + (level / 3);

            Enemy* e = new Enemy(BASE_HP * coef, BASE_DAMAGE * coef);
            
            while (attempt < 3 && !is_spawn){
                int new_x = build_cell->get_x() + world->random(3) - 1;
                int new_y = build_cell->get_y() + world->random(3) - 1;

                if ((new_x != build_cell->get_x() || new_y != build_cell->get_y()) && world->is_valid_coord(new_x, new_y)){
                    Cell& target_cell = world->get_cell(new_x, new_y);

                    if(!target_cell.set_entity(e)){
                        is_spawn = true;
                    }
                }
                attempt++;
            }
            if (!is_spawn){
                for(int y = -1; y < 2 && !is_spawn; y++){
                    for(int x = -1; x < 2 && !is_spawn; x++){
                        if (y == 0 && x == 0) continue;
                        int new_x = build_cell->get_x() + x;
                        int new_y = build_cell->get_y() + y;

                        if (!world->is_valid_coord(new_x, new_y)) continue;
                        Cell& target_cell = world->get_cell(new_x, new_y);

                        if(!target_cell.set_entity(e)){
                            is_spawn = true;
                            break;
                        }
                    }
                }
            }

            if (is_spawn){
                world->push_enemy_to_vector(e);
            } else{
                delete e;
            }
        }
        cooldown_build = 0;
    } else{
        cooldown_build++;
    }

    // процесс хода вражеских башен
    for (auto et : world->get_enemies_tower()){
        if (!et->is_alive()) continue;

        if (et->get_elapsed_since_shot() < cooldown_tower){
            et->add_elapsed_since_shot();
            continue;
        }

        Player& player = world->get_player();
        Cell* cell_player = world->get_cell(&player);
        Cell* cell_et = world->get_cell(et);

        if (!cell_player || !cell_player->get_entity()) continue;

        int x_p, y_p, x_et, y_et;
        int range = et->get_range();
        cell_player->get_coord(x_p, y_p);
        cell_et->get_coord(x_et, y_et);


        if ((x_et - range) < x_p && x_p < (x_et + range)
        && (y_et - range) < y_p && y_p < (y_et + range)){
            world->magic_damage(et->get_waterball().get_damage(*et), cell_player);
            et->set_elapsed_since_shot(0);
        } else {
            et->add_elapsed_since_shot();
        }
    }
}

void Game::init_world(int cnt_enemy, int cnt_enemy_build, int cnt_enemy_tower, int required_quantity, float coef){
    world->gen_eneimes(cnt_enemy, coef);
    world->gen_enemies_build(cnt_enemy_build, required_quantity, coef);
    world->gen_enemies_tower(cnt_enemy_tower, coef);
}

bool Game::is_friendly_fire() {return friendly_fire == true;};


void Game::gen_new_level(World* snapshot_world, int cnt_enemy, int cnt_enemy_build, int cnt_enemy_tower, int required_quantity){
    Player& snapshot_player = snapshot_world->get_player();
    Player& player = world->get_player();

    player.set_hp(snapshot_player.get_max_hp());
    player.get_hand().random_remove_card();

    float coef = 1 + (level / 4);

    int new_size_x = snapshot_world->get_size_x() + 2;
    int new_size_y = snapshot_world->get_size_y() + 2;

    if(new_size_x >= MAX_SIZE) new_size_x = MAX_SIZE;
    if(new_size_y >= MAX_SIZE) new_size_y = MAX_SIZE;

    Player* player_ptr = new Player(player);
    player_ptr -> update_player();

    delete world;
    world = new World(new_size_x, new_size_y, player_ptr);
    init_world(cnt_enemy, cnt_enemy_build, cnt_enemy_tower, required_quantity, coef);
}

void Game::set_level(int nl) {nl = (nl >= 25 || nl < 1) ? 1 : nl; level = nl;};

int Game::get_level() {return level;};

World* Game::get_world() {return world;};

int Game::handle_command(CommandManager& cmd){
    Game load_game;
    switch (cmd.get_type()) {
        case CommandType::MOVE_UP:
            return world->move_player(0, -1);
        case CommandType::MOVE_LEFT:
            return world->move_player(-1, 0);
        case CommandType::MOVE_DOWN:
            return world->move_player(0, 1);
        case CommandType::MOVE_RIGHT:
            return world->move_player(1, 0);
        case CommandType::CHANGE_ATTACK:
            world->get_player().switch_attack();
            world->get_player().switch_slowed();
            return 0;
        case CommandType::USE_CARD:
            return world->get_player().get_hand().use_card(world);
        case CommandType::END_GAME:
            is_active = false;
            return 0;
        case CommandType::SAVE:
            sm.save_game(is_active, is_player_turn, friendly_fire, cooldown_build,
                cooldown_tower, world, level);
            return 1;
        case CommandType::LOAD:
            load_game = sm.load_game(*this);
            *this = std::move(load_game);
            return 1;
        case CommandType::UNKNOWN:
        default:
            std::cout << "unknown command" << std::endl;
            std::cout << "type 'help' for avaible commands" << std::endl;
            return 1;
    }
}