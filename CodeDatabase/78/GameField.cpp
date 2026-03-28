#include "GameField.h"
#include <iostream>
#include <cstdlib>

GameField::GameField(int width, int height)
    : width_(width), height_(height), cells_(nullptr) {
        if (!are_dimensions_valid(height, width)){
            throw std::invalid_argument("Invalid field dimensions");
        }

        initialize_cells();
    }

GameField::GameField(const GameField& other)
    : width_(other.get_width()), height_(other.get_height()), cells_(nullptr){
    
        copy_from(other);
}

GameField::GameField(GameField&& other){
    move_from(std::move(other));
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        cleanup_cells();
        
        width_ = other.width_;
        height_ = other.height_;

        copy_from(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other){
    if (this != &other) {
        cleanup_cells();
        move_from(std::move(other));
    }
    return *this;
}

GameField::~GameField(){
    cleanup_cells();
}

Cell& GameField::get_cell(int x, int y){
    if(!is_coordinates_valid(x, y)){
        throw std::out_of_range("Position is out of field bounds");
    }

    return cells_[y][x];
}

bool GameField::is_coordinates_valid(int x, int y) const {
    return x >= 0 && x <= width_ - 1 && y >= 0 && y <= height_ - 1;
}

bool GameField::is_cell_occupied(int x, int y) const{
    if(!is_coordinates_valid(x, y)){
        return false;
    }
    return cells_[y][x].is_occupied();
}

bool GameField::is_cell_passable(int x, int y) const {
    if (!is_coordinates_valid(x, y)) return false;
    return cells_[y][x].is_passable();
}

bool GameField::is_cell_empty(int x, int y) const{
    if(!is_coordinates_valid(x, y)){
        return false;
    }
    return cells_[y][x].is_empty();
}

std::unique_ptr<Player> GameField::extract_player() {
    if (player_) {
        cells_[player_->get_y()][player_->get_x()].clear_entity();
    }
    return std::move(player_);
}

bool GameField::place_entity(std::unique_ptr<Entity> entity, int x, int y) {
    if (!is_cell_passable(x, y)) {
        return false;
    }

    Entity* entity_ptr = entity.get();
    
    if (dynamic_cast<Player*>(entity_ptr)) {
        player_ = std::unique_ptr<Player>(static_cast<Player*>(entity.release()));
    } else if (dynamic_cast<Enemy*>(entity_ptr)) {
        enemies_.push_back(std::unique_ptr<Enemy>(static_cast<Enemy*>(entity.release())));
    } else if (dynamic_cast<Tower*>(entity_ptr)) {
        towers_.push_back(std::unique_ptr<Tower>(static_cast<Tower*>(entity.release())));
    } else if (dynamic_cast<Ally*>(entity_ptr)) {
        allies_.push_back(std::unique_ptr<Ally>(static_cast<Ally*>(entity.release())));
    }else {
        return false;
    }
    
    cells_[y][x].set_entity(entity_ptr);
    return true;
}

bool GameField::move_entity(int from_x, int from_y, int to_x, int to_y) {
    if (!is_coordinates_valid(from_x, from_y) || !is_coordinates_valid(to_x, to_y)) {
        return false;
    }
    
    Cell& from_cell = get_cell(from_x, from_y);
    Cell& to_cell = get_cell(to_x, to_y);
    
    if (!from_cell.is_occupied() || to_cell.is_occupied()) {
        return false;
    }
    
    Entity* entity = from_cell.get_entity();
    to_cell.set_entity(entity);
    from_cell.clear_entity();
    
    return true;
}

void GameField::spawn_random_towers(int count, int default_health, int default_r_s, int default_m_c, int health_spawned_enemy, float multiplier) {
    int spawned = 0;
    int attempts = 0;
    const int MAX_ATTEMPTS = count * 10;
    
    while (spawned < count && attempts < MAX_ATTEMPTS) {
        attempts++;
        
        int x = rand() % width_;
        int y = rand() % height_;
        
        if (is_cell_passable(x, y)) {
            auto tower = std::make_unique<Tower>(default_health, x, y, default_r_s, default_m_c, 5, health_spawned_enemy, multiplier);
            if (place_entity(std::move(tower), x, y)) {
                spawned++;
            }
        }
    }    
}

void GameField::spawn_random_enemies(int count, int default_health) {
    int spawned = 0;
    int attempts = 0;
    const int MAX_ATTEMPTS = count * 10;
    
    while (spawned < count && attempts < MAX_ATTEMPTS) {
        attempts++;
        
        int x = rand() % width_;
        int y = rand() % height_;
    
        if (is_cell_passable(x, y)) {
            auto enemy = std::make_unique<Enemy>(default_health, x, y);
            if (place_entity(std::move(enemy), x, y)) {
                spawned++;
            }
        }
    }
}

void GameField::spawn_random_cell_type(int count, CellType cell_type){
    int spawned = 0;
    int attempts = 0;
    const int MAX_ATTEMPTS = count * 10;
    
    while (spawned < count && attempts < MAX_ATTEMPTS) {
        attempts++;
        
        int x = rand() % width_;
        int y = rand() % height_;
        
        if (is_cell_empty(x, y) && !is_cell_occupied(x, y)) {
            cells_[y][x].set_type(cell_type);
            spawned++;
        }
    }
}

void GameField::update(){
    if (player_) {
        player_->update(*this);
    }
    
    update_allies();

    int total_gold_reward = 0;
    int total_score_reward = 0;
    update_enemies(total_gold_reward, total_score_reward);
    update_towers(total_gold_reward, total_score_reward);

    if (player_) {
        player_->add_score(total_score_reward);
        player_->add_gold(total_gold_reward);
    }
}

void GameField::update_enemies(int& total_gold_reward, int& total_score_reward) {
    auto enemy_it = enemies_.begin();
    while (enemy_it != enemies_.end()) {
        if (!(*enemy_it)->is_alive()) {
            int x = (*enemy_it)->get_x();
            int y = (*enemy_it)->get_y();
            cells_[y][x].clear_entity();
            
            total_gold_reward += (*enemy_it)->get_max_health() / 5;
            total_score_reward += (*enemy_it)->get_max_health() / 2;
            
            enemy_it = enemies_.erase(enemy_it);
        } else {
            (*enemy_it)->update(*this);
            ++enemy_it;
        }
    }
}

void GameField::update_towers(int& total_gold_reward, int& total_score_reward) {
    auto tower_it = towers_.begin();
    while (tower_it != towers_.end()) {
        if (!(*tower_it)->is_alive()) {
            int x = (*tower_it)->get_x();
            int y = (*tower_it)->get_y();
            cells_[y][x].clear_entity();
            
            total_gold_reward += (*tower_it)->get_max_health() / 4;
            total_score_reward += (*tower_it)->get_max_health() / 2;
            
            tower_it = towers_.erase(tower_it);
        } else {
            (*tower_it)->update(*this);
            ++tower_it;
        }
    }
}

void GameField::update_allies() {
    auto ally_it = allies_.begin();
    while (ally_it != allies_.end()) {
        if (!(*ally_it)->is_alive()) {
            int x = (*ally_it)->get_x();
            int y = (*ally_it)->get_y();
            cells_[y][x].clear_entity();
            ally_it = allies_.erase(ally_it);
        } else {
            (*ally_it)->update(*this);
            ++ally_it;
        }
    }
}

bool GameField::are_dimensions_valid(int width, int height){
    const int MIN_SIZE = 10;
    const int MAX_SIZE = 25;
    return width >= MIN_SIZE && width <= MAX_SIZE\
            && height >= MIN_SIZE && height <= MAX_SIZE;
}

void GameField::set_dimensions(int width, int height){
    if (!are_dimensions_valid(height, width)){
        throw std::invalid_argument("Invalid field dimensions");
    }
    width_ = width;
    height_ = height;
}

void GameField::initialize_cells(){
    cells_ = new Cell*[height_];   
    for(int i = 0; i < height_; i++){
        cells_[i] = new Cell[width_];

        /* for(int j = 0; j < width_; j++){
            if(i == 0 || i == height_ - 1 || j == 0 || j == width_ - 1){
                cells_[i][j].set_type(CellType::WALL);
            }
        } */
    }
}

void GameField::cleanup_cells(){
    if(cells_){
        for(int i = 0; i < height_; i++){
            delete[] cells_[i];
       }
       delete[] cells_;
       cells_ = nullptr;
    }
}

void GameField::copy_from(const GameField& other){
    initialize_cells();
    for(int i = 0; i < height_; i++){
        for(int j = 0; j < width_; j++){
            cells_[i][j] = other.cells_[i][j];
        }
    }

    if(other.get_player()){
        Player* other_player = other.get_player();
        player_ = std::make_unique<Player>(
            other_player->get_max_health(), 
            other_player->get_x(), 
            other_player->get_y(),
            other_player->get_max_mana(),
            5
        );
        player_->set_health(other_player->get_health());
        player_->set_position(other_player->get_x(), other_player->get_y());
        player_->add_score(other_player->get_score());

        for(int i = 0; i < other_player->get_level(); i++){
            player_->add_score(other_player->get_score());
        }
        player_->add_gold(other_player->get_gold());
        player_->add_mana(other_player->get_mana() - player_->get_mana());
        player_->switch_weapon(other_player->get_weapon().get_type());
    }

    for(const auto& enemy: other.enemies_){
        enemies_.push_back(std::unique_ptr<Enemy>(new Enemy(*enemy)));
    }
    
    for(const auto& tower: other.towers_){
        towers_.push_back(std::unique_ptr<Tower>(new Tower(*tower)));
    }
}

void GameField::move_from(GameField&& other){
    set_dimensions(other.get_width(), other.get_height());
    cells_ = other.cells_;
    player_ = std::move(other.player_);
    enemies_ = std::move(other.enemies_);
    towers_ = std::move(other.towers_);

    other.set_dimensions(0, 0);
    other.cells_ = nullptr;
}

void GameField::show_enemy_stats() const {
    std::cout << "=== ENEMY STATS ===" << std::endl;
    
    for (const auto& enemy : enemies_) {
        if (enemy->is_alive()) {
            std::cout << "Enemy (" << enemy->get_x() << ", " << enemy->get_y() 
                      << ") HP: " << enemy->get_health() << "/" << enemy->get_max_health() << std::endl;
        }
    }
    for (const auto& tower : towers_) {
        if (tower->is_alive()) {
            std::cout << "Tower (" << tower->get_x() << ", " << tower->get_y() 
                      << ") HP: " << tower->get_health() << "/" << tower->get_max_health() << std::endl;
        }
    }
    if (enemies_.empty() && towers_.empty()) {
        std::cout << "No enemies or towers on the field" << std::endl;
    }
}

std::string GameField::get_cell_symbol(int x, int y) const {
    std::string symbol;
    const Cell& cell = cells_[y][x];
    Entity* entity = cell.get_entity();
    
    if (entity) {
        if (dynamic_cast<Player*>(entity)) {
            symbol = "P";
        } else if (dynamic_cast<Enemy*>(entity)) {
            symbol = "E";
        } else if (dynamic_cast<Tower*>(entity)) {
            symbol = "T";
        } else if (dynamic_cast<Ally*>(entity)) {
            symbol = "A";
        } else {
            symbol = "O";
        }
    } else {
        switch (cell.get_type()) {
            case CellType::WALL: symbol = "#"; break;
            case CellType::SLOW_ZONE: symbol = "~"; break;
            case CellType::TRAP: symbol = "t"; break;
            default: symbol = "."; break;
        }
    }

    return symbol;
}

void GameField::draw_field() const {
    std::cout << "   ";
    for (int x = 0; x < width_; x++) {
        std::cout << x << (x < 10 ? "  " : " ");
    }
    std::cout << "     === GAME INFO ===" << '\n';
    
    for (int y = 0; y < height_; y++) {
        std::cout << y << (y < 10 ? "  " : " ");
        
        for (int x = 0; x < width_; x++) {
            std::cout << get_cell_symbol(x, y);
            std::cout << "  ";
        }
        
        if (y == 0 && player_) {
            std::cout << "   Player: HP " << player_->get_health() 
                      << "/" << player_->get_max_health()
                      << " | Lvl " << player_->get_level()
                      << "(" << std::to_string(player_->get_score()) 
                      << "/" << std::to_string(player_->get_score_for_next_lvl()) << ")";
        }
        else if (y == 1 && player_) {
            std::cout << "   Weapon: " << player_->get_weapon().get_name()
                      << " | Dmg " << player_->get_weapon().get_damage();
        }
        else if (y == 2 && player_) {
            std::cout << "   Mana: " << player_->get_mana() << "/" << player_->get_max_mana()
                      << " | Gold: " << player_->get_gold();
        }
        else if (y == 3 && player_) {
            auto& spell_hand = player_->get_spell_hand();
            std::cout << "   Spells: ";
            int shown_spells = 0;
            for (int i = 0; i < spell_hand.get_size() && shown_spells < 3; i++) {
                if (spell_hand.get_spell(i)) {
                    if (shown_spells > 0) std::cout << ", ";
                    std::cout << spell_hand.get_spell(i)->get_name();
                    shown_spells++;
                }
            }
            if (shown_spells == 0) {
                std::cout << "None";
            }
        }
        else if (y == 4) {
            std::cout << "   Enemies: " << enemies_.size() << " alive";
        }
        else if (y == 5 && !enemies_.empty()) {
            std::cout << "   Enemy1: (" << enemies_[0]->get_x() 
                      << "," << enemies_[0]->get_y() << ") HP " << enemies_[0]->get_health();
        }
        else if (y == 6) {
            std::cout << "   Towers: " << towers_.size() << " active";
        }
        else if (y == 7 && !towers_.empty()) {
            std::cout << "   Tower1: (" << towers_[0]->get_x() 
                      << "," << towers_[0]->get_y() << ") HP " << towers_[0]->get_health();
        }
        else if (y == 8 && player_) {
            auto& spell_hand = player_->get_spell_hand();
            int total_spells = 0;
            for (int i = 0; i < spell_hand.get_size(); i++) {
                if (spell_hand.get_spell(i)) total_spells++;
            }
            if (total_spells > 3) {
                std::cout << "   +" << (total_spells - 3) << " more spells";
            }
        }
        
        std::cout << "\n";
    }
}

std::string GameField::serialize() const{
    std::string data;

    data += "FIELD\n";
    data += "SIZE " + std::to_string(width_) + " " + std::to_string(height_) + "\n";
    std::string symbol;
    for(int y = 0; y < height_; y++){
        for(int x = 0; x < width_; x++){
            symbol = get_cell_symbol(x, y);
            data += symbol;
        }
        data += '\n';
    }
    data += "\n";

    for(int i = 0; i < enemies_.size(); i++){
        data += "ENEMY\n";
        data += enemies_[i]->serialize();
        data += "\n";
    }

    for(int i = 0; i < towers_.size(); i++){
        data += "TOWER\n";
        data += towers_[i]->serialize();
        data += "\n";
    }

    for(int i = 0; i < allies_.size(); i++){
        data += "ALLY\n";
        data += allies_[i]->serialize();
        data += "\n";
    }

    return data;
}