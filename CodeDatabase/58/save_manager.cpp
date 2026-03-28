#include "save_manager.hpp"
#include "game.hpp"

std::filesystem::path SaveManager::get_save_dir() {return save_dir;};

int SaveManager::save_game(bool is_active, bool is_player_turn, bool friendly_fire, int cooldown_build, int cooldown_tower, World* world, int level){
    json j;
    // Состояние игры
    j["game_state"]["is_active"] = is_active;
    j["game_state"]["is_player_turn"] = is_player_turn;
    j["game_state"]["friendly_fire"] = friendly_fire;
    j["game_state"]["cooldown_build"] = cooldown_build;
    j["game_state"]["cooldown_tower"] = cooldown_tower;
    j["game_state"]["level"] = level;

    // Размерность поля
    int size_x, size_y;
    world->get_sizes(size_x, size_y);
    j["world"]["size_x"] = size_x;
    j["world"]["size_y"] = size_y;

    // Клетки поля
    json cell_array = json::array();
    for(int y = 0; y < size_y; y++){
        for(int x = 0; x < size_x; x++){
            json cell_json;
            Cell& cell = world->get_cell(x, y);
            Entity* entity = cell.get_entity();

            cell_json["x"] = x;
            cell_json["y"] = y;
            cell_json["type_cell"] = cell.get_type_cell();
            if (!entity || entity->get_fraction() == Fraction::BLUE){
                cell_json["entity"] = nullptr;
            } else{
                json entity_json;
                if (dynamic_cast<Enemy*>(entity)){
                    entity_json["type"] = "enemy";
                } else if(dynamic_cast<EnemyBuild*>(entity)){
                    entity_json["type"] = "enemy_build";
                    entity_json["required_quantity"] = dynamic_cast<EnemyBuild*>(entity)->get_required_quantity();  
                } else if(dynamic_cast<EnemyTower*>(entity)){
                    entity_json["type"] = "enemy_tower";
                    entity_json["elapsed_since_shot"] = dynamic_cast<EnemyTower*>(entity)->get_elapsed_since_shot();
                }
                entity_json["hp"] = entity->get_hp();
                entity_json["damage"] = entity->get_damage();
                cell_json["entity"] = entity_json;
            }
            cell_array.push_back(cell_json);
        }
    }
    j["world"]["cells"] = cell_array;

    // Игрок
    j["world"]["player"]["points"] = world->get_player().get_points();
    j["world"]["player"]["is_slowed"] = world->get_player().has_slowed();
    j["world"]["player"]["type_attack"] = world->get_player().get_type_attack();
    j["world"]["player"]["x"] = world->get_cell(&world->get_player())->get_x();
    j["world"]["player"]["y"] = world->get_cell(&world->get_player())->get_y();
    j["world"]["player"]["hp"] = world->get_player().get_hp();
    j["world"]["player"]["damage"] = world->get_player().get_damage();
    j["world"]["player"]["max_hp"] = world->get_player().get_max_hp();

    // Рука игрока
    PlayerHand hand = world->get_player().get_hand();
    int size_hand = hand.get_max_size_hand();
    j["world"]["player"]["hand"]["size_hand"] = size_hand;

    json cards_array = json::array();
    for(auto card: hand.get_cards()){
        json card_json;

        if(auto waterball = dynamic_cast<WaterBall*>(card)){
            card_json["name"] = waterball->get_name();
            card_json["coef"] = waterball->get_coef();
            card_json["range"] = waterball->get_range();
        } else if(auto banana_trap = dynamic_cast<BananaTrap*>(card)){
            card_json["name"] = banana_trap->get_name();
        } else if(auto poison_cloud = dynamic_cast<PoisonCloud*>(card)){
            card_json["name"] = poison_cloud->get_name();
            card_json["coef"] = poison_cloud->get_coef();
            card_json["area"] = poison_cloud->get_area();
        }

        cards_array.push_back(card_json);
    }
    j["world"]["player"]["hand"]["cards"] = cards_array;


    if (!std::filesystem::exists(save_dir)) {
        std::filesystem::create_directory(save_dir);
    }

    std::string filename = "tmp";

    try{
        std::cout << "enter filename: ";
        if(!(std::cin >> filename) || filename.empty()){
            std::cerr << "error: not found file" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return 1;
        };
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::filesystem::path file_path = save_dir / (filename + ".json");

        if(std::filesystem::exists(file_path)){
            std::string overwrite;
            std::cout << "file: " << file_path << " is exists. overwrite? (y / N): ";
            std::cin >> overwrite;
            std::transform(overwrite.begin(), overwrite.end(), overwrite.begin(), ::toupper);

            if(overwrite != "Y" && overwrite != "YES"){
                std::cout << "save cancelled" << std::endl;
                return 1;
            }
        }

        std::ofstream file(file_path);
        if (!file.is_open()){
            std::cerr << "error: cannot open file: " << file_path << " for writing" << std::endl;
            return 1; 
        }

        file << j.dump(4);

        if(!file.good()){
            std::cerr << "error: failed write to file " << file_path << std::endl;
            return 1;
        }

        file.close();
        std::cout << "game saved" << std::endl;
    } catch (std::exception& e){
        std::cerr << "error saving game: " << e.what() << std::endl;
        return 1;
    }

    return 1;
}

Game SaveManager::load_game(Game copy_game){
    std::string filename;
    std::filesystem::path file_path;
    bool load_success = false;

    while(!load_success){
        try{
            if(!std::filesystem::exists(save_dir)){
                std::cout << "error: not found save dir 'saves'" << std::endl;
                std::cout << "load game cancelled" << std::endl;
                return copy_game;
            }

            std::cout << "enter filename: ";
            if(!(std::cin >> filename) || filename.empty()){
                std::cin.clear();
                std::cout << "error: save not found" << std::endl;
                return copy_game;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

            if(filename == "cancel" || filename == "exit"){
                std::cout << "load canceled" << std::endl;
                return copy_game;
            }

            file_path = save_dir / (filename + ".json");
            if(!std::filesystem::exists(file_path)){
                std::cout << "error: file not exists" << std::endl;
                std::cout << "try again... or write cancel" << std::endl;
                continue;
            }
        } catch (std::exception& e){
            std::cerr << "unexpected error: " << e.what() << std::endl;
            std::cout << "try again... or write cancel" << std::endl;
            continue;
        }
        load_success = !load_success;
    }

    std::ifstream file(file_path);
    json load_json;
    try{
        load_json = json::parse(file);
    } catch (json::parse_error& e){
        std::cout << "error: invalid file *_*) " << std::endl;
        return copy_game;
    }
    
    if(!valid_check_json(load_json)){
        return copy_game;
    }

    // game state
    int cooldown_build = load_json["game_state"]["cooldown_build"];
    int cooldown_tower = load_json["game_state"]["cooldown_tower"];
    bool friendly_fire = load_json["game_state"]["friendly_fire"];
    bool is_active = load_json["game_state"]["is_active"];
    bool is_player_turn = load_json["game_state"]["is_player_turn"];
    int level = load_json["game_state"]["level"];

    // world
    int size_x = load_json["world"]["size_x"];
    int size_y = load_json["world"]["size_y"];

    // player
    bool is_slowed = load_json["world"]["player"]["is_slowed"];
    int points = load_json["world"]["player"]["points"];
    TypeAttack type_attack = load_json["world"]["player"]["type_attack"];
    int player_x = load_json["world"]["player"]["x"];
    int player_y = load_json["world"]["player"]["y"];
    int player_hp = load_json["world"]["player"]["hp"];
    int player_damage = load_json["world"]["player"]["damage"];
    int player_max_hp = load_json["world"]["player"]["max_hp"];

    // player hand
    int size_hand = load_json["world"]["player"]["hand"]["size_hand"];
    std::vector<ISpellCard*> cards;
    for(auto card_json : load_json["world"]["player"]["hand"]["cards"]){
        std::string card_name = card_json["name"];

        if(card_name == "WaterBall"){
            int range = card_json["range"];
            int coef = card_json["coef"];
            cards.push_back(new WaterBall(range, coef));
        } else if (card_name == "PoisonCloud"){
            cards.push_back(new PoisonCloud());
        } else if (card_name == "BananaTrap"){
            cards.push_back(new BananaTrap());
        }
    }

    // cells
    std::vector<std::vector<Cell>> cells;
    std::vector<Enemy*> enemies;
    std::vector<EnemyBuild*> enemies_build;
    std::vector<EnemyTower*> enemies_tower;
    cells.resize(size_y);
    for(int i = 0; i < size_y; i++){
        cells[i].resize(size_x);
        for(int j = 0; j < size_x; j++){
            cells[i][j] = Cell(j, i);
        }
    }
    for(auto cell_json : load_json["world"]["cells"]){
        int x = cell_json["x"];
        int y = cell_json["y"];
        TypeCell type_cell = cell_json["type_cell"];
        cells[y][x].set_type_cell(type_cell);

        if(!cell_json["entity"].is_null()){
            json entity_json = cell_json["entity"];
            std::string type = entity_json["type"];

            int enemy_hp = entity_json["hp"];
            int enemy_damage = entity_json["damage"];

            if(type == "enemy_build"){
                int required_quantity = entity_json["required_quantity"];
                EnemyBuild* eb = new EnemyBuild(required_quantity, enemy_hp, enemy_damage);
                enemies_build.push_back(eb);
                cells[y][x].set_entity(eb);
            } else if(type == "enemy_tower"){
                int elapsed_since_shot = entity_json["elapsed_since_shot"];
                EnemyTower* et = new EnemyTower(enemy_hp, enemy_damage, elapsed_since_shot);
                enemies_tower.push_back(et);
                cells[y][x].set_entity(et);
            } else if(type == "enemy"){
                Enemy* e = new Enemy(enemy_hp, enemy_damage);
                enemies.push_back(e);
                cells[y][x].set_entity(e);
            }
        }
    }

    Player* player_ptr = new Player(player_hp, player_damage, type_attack, size_hand);
    player_ptr->set_points(points);
    if(is_slowed){
        player_ptr->switch_slowed();
    }
    player_ptr->set_max_hp(player_max_hp);
    
    PlayerHand& hand = player_ptr->get_hand();
    hand.set_cards(cards);

    cells[player_y][player_x].set_entity(player_ptr);

    World* world_ptr = new World(size_x, size_y, player_ptr);
    world_ptr->set_cells(cells);
    world_ptr->set_enemies(enemies);
    world_ptr->set_enemies_build(enemies_build);
    world_ptr->set_enemies_tower(enemies_tower);

    Game new_game(world_ptr, is_active, is_player_turn, friendly_fire, cooldown_build, cooldown_tower);
    new_game.set_level(level);

    return new_game;
}


bool SaveManager::valid_check_json(json load_json){
        // Проверяем обязательные поля и их типы
    if (!load_json.contains("game_state") || !load_json.contains("world")) {
        std::cout << "error: invalid save file structure" << std::endl;
        return false;
    }

    // Проверяем game_state
    auto& game_state = load_json["game_state"];
    if (!game_state.contains("cooldown_build") || !game_state["cooldown_build"].is_number() ||
        !game_state.contains("cooldown_tower") || !game_state["cooldown_tower"].is_number() ||
        !game_state.contains("friendly_fire") || !game_state["friendly_fire"].is_boolean() ||
        !game_state.contains("is_active") || !game_state["is_active"].is_boolean() ||
        !game_state.contains("is_player_turn") || !game_state["is_player_turn"].is_boolean() ||
        !game_state.contains("level") || !game_state["level"].is_number()) {
        std::cout << "error: invalid game state data" << std::endl;
        return false;
    }

    // Проверяем world
    auto& world = load_json["world"];
    if (!world.contains("size_x") || !world["size_x"].is_number() ||
        !world.contains("size_y") || !world["size_y"].is_number() ||
        !world.contains("player") || !world.contains("cells")) {
        std::cout << "error: invalid world data" << std::endl;
        return false;
    }

    // Проверяем player
    auto& player = world["player"];
    if (!player.contains("is_slowed") || !player["is_slowed"].is_boolean() ||
        !player.contains("points") || !player["points"].is_number() ||
        !player.contains("type_attack") || !player["type_attack"].is_number() ||
        !player.contains("x") || !player["x"].is_number() ||
        !player.contains("y") || !player["y"].is_number() ||
        !player.contains("hp") || !player["hp"].is_number() ||
        !player.contains("damage") || !player["damage"].is_number() ||
        !player.contains("hand") || !player["max_hp"].is_number()) {
        std::cout << "error: invalid player data" << std::endl;
        return false;
    }

    // Проверяем hand
    auto& hand_json = player["hand"];
    if (!hand_json.contains("size_hand") || !hand_json["size_hand"].is_number() ||
        !hand_json.contains("cards") || !hand_json["cards"].is_array()) {
        std::cout << "error: invalid hand data" << std::endl;
        return false;
    }

    // Проверяем cells
    auto& cells_json = world["cells"];
    if (!cells_json.is_array()) {
        std::cout << "error: invalid cells data" << std::endl;
        return false;
    }

    // player hand
    for(auto card_json : hand_json["cards"]){
        if (!card_json.contains("name") || !card_json["name"].is_string()) {
            std::cout << "error: invalid card data" << std::endl;
            return false;
        }

        std::string card_name = card_json["name"];

        if(card_name == "WaterBall"){
            if (!card_json.contains("range") || !card_json["range"].is_number() ||
                !card_json.contains("coef") || !card_json["coef"].is_number()) {
                std::cout << "error: invalid WaterBall card data" << std::endl;
                return false;
            }
        } else if (card_name == "PoisonCloud"){
        } else if (card_name == "BananaTrap"){
        } else {
            std::cout << "error: unknown card type: " << card_name << std::endl;
            return false;
        }
    }


    // cells
    for(auto cell_json : cells_json){
        if (!cell_json.contains("x") || !cell_json["x"].is_number() ||
            !cell_json.contains("y") || !cell_json["y"].is_number() ||
            !cell_json.contains("type_cell") || !cell_json["type_cell"].is_number()) {
            std::cout << "error: invalid cell data" << std::endl;
            // Очищаем ресурсы перед возвратом
            return false;
        }

        if(!cell_json["entity"].is_null()){
            json entity_json = cell_json["entity"];
            if (!entity_json.contains("type") || !entity_json["type"].is_string() ||
                !entity_json.contains("hp") || !entity_json["hp"].is_number() ||
                !entity_json.contains("damage") || !entity_json["damage"].is_number()) {
                std::cout << "error: invalid entity data" << std::endl;
                return false;
            }
            
            std::string type = entity_json["type"];

            if(type == "enemy_build"){
                if (!entity_json.contains("required_quantity") || !entity_json["required_quantity"].is_number()) {
                    std::cout << "error: invalid enemy_build data" << std::endl;
                    return false;
                }
            } else if(type == "enemy_tower"){
                if (!entity_json.contains("elapsed_since_shot") || !entity_json["elapsed_since_shot"].is_number()) {
                    std::cout << "error: invalid enemy_tower data" << std::endl;
                    return false;
                }
            } else if(type == "enemy"){
            } else {
                std::cout << "error: unknown entity type: " << type << std::endl;
                return false;
            }
        }
    }
    return true;
}