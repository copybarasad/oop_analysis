#include "../include/core/game_board.h"

#include <algorithm>

int CheckInLimit(int value) {
	if (value < 10) { return 10; }
	if (value > 25) { return 25; }

	return value;
}

GameBoard::GameBoard(int rows, int cols) : 
    rows_count_(CheckInLimit(rows)), 
    cols_count_(CheckInLimit(cols)), 
    cells_(rows_count_ * cols_count_, Cell(CellType::Empty)), 
    player_(), 
    enemies_(), 
    friends_(),
    enemy_towers_(),
    player_position_(-1, -1),
    enemy_positions_(),
    friend_positions_(),
    tower_positions_(),
    current_level_(1) {}

GameBoard::~GameBoard() = default;

GameBoard::GameBoard(GameBoard &other) : 
rows_count_(other.rows_count_), 
cols_count_(other.cols_count_), 
cells_(other.cells_), 
player_(other.player_), 
enemies_(other.enemies_), 
enemy_towers_(other.enemy_towers_),
player_position_(other.player_position_),
enemy_positions_(other.enemy_positions_),
tower_positions_(other.tower_positions_) {}

GameBoard &GameBoard::operator=(GameBoard &other) {
	if (this == &other) { return *this; }

	this->rows_count_ = other.rows_count_;
	this->cols_count_ = other.cols_count_;
	this->cells_ = other.cells_;
	this->player_ = other.player_;
	this->enemies_ = other.enemies_;
	this->enemy_towers_ = other.enemy_towers_;
    this->player_position_ = other.player_position_;
    this->enemy_positions_ = other.enemy_positions_;
    this->tower_positions_ = other.tower_positions_;
 
	return *this;
}

GameBoard::GameBoard(GameBoard &&other) 
    : rows_count_(other.rows_count_), 
      cols_count_(other.cols_count_), 
      cells_(std::move(other.cells_)), 
      player_(std::move(other.player_)), 
      enemies_(std::move(other.enemies_)),
      enemy_towers_(std::move(other.enemy_towers_)),
      player_position_(other.player_position_),
      enemy_positions_(std::move(other.enemy_positions_)),
      tower_positions_(std::move(other.tower_positions_)) {
    other.rows_count_ = 0;
    other.cols_count_ = 0;
    other.player_position_ = Position(-1, -1);
}

GameBoard &GameBoard::operator=(GameBoard &&other) {
	if (this == &other) { return *this; }
	
    this->rows_count_ = other.rows_count_;
	this->cols_count_ = other.cols_count_;
	this->cells_ = std::move(other.cells_);
	this->player_ = std::move(other.player_);
	this->enemies_ = std::move(other.enemies_);
	this->enemy_towers_ = std::move(other.enemy_towers_);
	this->player_position_ = other.player_position_;
    this->enemy_positions_ = std::move(other.enemy_positions_);
    this->tower_positions_ = std::move(other.tower_positions_);
	
	other.rows_count_ = 0;
	other.cols_count_ = 0;
	other.player_position_ = Position(-1, -1);

    return *this;
}

int GameBoard::rows() const { return this->rows_count_; }
int GameBoard::cols() const { return this->cols_count_; }

const Player &GameBoard::player() const { return this->player_; }
const std::vector<Enemy> &GameBoard::enemies() const { return this->enemies_; }
const std::vector<EnemyTower> &GameBoard::enemy_towers() const { return this->enemy_towers_; }

int GameBoard::Index(int row, int col) const {
	return row * this->cols_count_ + col;
}

CellType GameBoard::cell_type(int row, int col) const {
	return this->cells_[Index(row, col)].type();
}

void GameBoard::SetCell(const Position &p, CellType ct) {
	this->cells_[Index(p.row(), p.col())].set_type(ct);
}

void GameBoard::ClearCell(const Position& p) {
	this->cells_[Index(p.row(), p.col())].set_type(CellType::Empty);
}

bool GameBoard::InBoard(const Position &p) const {
    return p.row() >= 0 && p.row() < this->rows_count_ && 
           p.col() >= 0 && p.col() < this->cols_count_;
}

bool GameBoard::AddPlayer(const Position &pos) {
    if (!this->InBoard(pos)) { return false; }
    if (this->cell_type(pos.row(), pos.col()) != CellType::Empty) { return false; }

    this->player_position_ = pos;
    this->SetCell(pos, CellType::Player);

    return true;
}

bool GameBoard::MovePlayer(Direction dir) {
	if (!this->player_.is_alive()) { return false; };
    if (this->player_.is_frozen()) {
        this->player_.set_frozen(false);
        return false;
    }

    Position next = GetNextPosition(this->player_position_, dir);
    if (!this->InBoard(next)) { return false; };

    CellType ct = cell_type(next.row(), next.col());
	if (ct == CellType::Wall || ct == CellType::Enemy || ct == CellType::EnemyTower) {
		return false;
	}

    Position old_pos = this->player_position_;
        
    if (this->player_.on_freeze_cell()) {
        this->player_.set_on_freeze(false);
        this->SetCell(this->player_position_, CellType::Freeze);
    } else if (this->player_.has_trap()) {
        this->SetCell(old_pos, CellType::Trap);
        this->player_.set_trap(false);
	} else {
        this->ClearCell(this->player_position_);
    }

    if (ct == CellType::Freeze) {
        this->player_.set_frozen(true);
        this->player_.set_on_freeze(true);
    }
    if (ct == CellType::Trap) {
        this->player_.get_damage(40);
    }


	this->player_position_ = next;
	this->SetCell(next, CellType::Player);
	return true;
}

void GameBoard::DamagePlayer(int damage) {
    this->player_.get_damage(damage);
}

void GameBoard::AddPlayerScore(int points) {
    this->player_.add_score(points);
}

void GameBoard::ChangePlayerWeapon() {
    this->player_.change_weapon();
}

void GameBoard::SetPlayerFrozen(bool frozen) {
    this->player_.set_frozen(frozen);
}

void GameBoard::SetPlayerOnFreeze(bool on_freeze) {
    this->player_.set_on_freeze(on_freeze);
}

bool GameBoard::AddEnemy(const Position &pos) {
	if (!this->InBoard(pos)) { return false; }
	if (this->cell_type(pos.row(), pos.col()) != CellType::Empty) { return false; }
	
    Enemy e;
    this->enemies_.push_back(e);
    this->enemy_positions_.push_back(pos); 
	SetCell(pos, CellType::Enemy);

	return true;
}

bool GameBoard::MoveEnemy(int enemy_index, Direction dir) {
    if (enemy_index < 0 || enemy_index >= static_cast<int>(this->enemies_.size())) { 
        return false; 
    }

	Enemy& enemy = this->enemies_[enemy_index];
	if (!enemy.is_alive()) { return false; }
    if (enemy.is_frozen()) {
        enemy.set_frozen(false);
        return false;
    }


    Position pos = this->enemy_positions_[enemy_index];
    Position next = GetNextPosition(pos, dir);

	if (!InBoard(next)) return false;

	CellType ct = cell_type(next.row(), next.col());
	if (ct == CellType::Wall || ct == CellType::Enemy || ct == CellType::EnemyTower) { return false; };
	
    if (ct == CellType::Player) {
		this->player_.get_damage(enemy.damage());
		return false;
	}

    if (enemy.on_freeze_cell()) {
        enemy.set_on_freeze(false);
        this->SetCell(pos, CellType::Freeze);
    } else {
		this->ClearCell(pos);
	}

    if (ct == CellType::Trap) {
        enemy.get_damage(50);
        this->ClearCell(next);
    
        if (!enemy.is_alive()) {
            this->ClearCell(pos);
            this->player_.add_score(50);
            return true;
        }
    }

    if (ct == CellType::Freeze) {
        enemy.set_frozen(true);
        enemy.set_on_freeze(true);
    }

    this->enemy_positions_[enemy_index] = next;
    this->SetCell(next, CellType::Enemy);
	return true;
}

bool GameBoard::AddEnemyTower(const Position &pos, int health, int spawn_interval) {
    if (!this->InBoard(pos)) { return false; }
    if (this->cell_type(pos.row(), pos.col()) != CellType::Empty) { return false; }
    
    EnemyTower tower(health, spawn_interval);

    this->enemy_towers_.push_back(tower);
    this->tower_positions_.push_back(pos);
    this->SetCell(pos, CellType::EnemyTower);

    return true;
}

Position GameBoard::FindEmptySpace(const Position& center) const {
    int directions[][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    
    for (int i = 0; i < 8; i++) {
        Position candidate(center.row() + directions[i][0], center.col() + directions[i][1]);
        if (this->InBoard(candidate) && this->cell_type(candidate.row(), candidate.col()) == CellType::Empty) {
            return candidate;
        }
    }
    
    return Position(-1, -1);
}

void GameBoard::UpdateEnemyTowers() {
    for (int i = 0; i < this->enemy_towers_.size(); ++i) {
        EnemyTower& tower = this->enemy_towers_[i];
        if (!tower.is_alive()) {
            tower_positions_.erase(tower_positions_.begin() + i);
            continue;
        }

        if (tower.tick()) {
            Position spawn_pos = this->FindEmptySpace(this->tower_positions_[i]);
            if (spawn_pos.row() != -1) {
                this->AddEnemy(spawn_pos);
            }
        }
    }
}

void GameBoard::UpdateTowerAttacks() {
    Position player_pos = this->PlayerPosition();
    
    for (size_t i = 0; i < this->enemy_towers_.size(); i++) {
        EnemyTower& tower = this->enemy_towers_[i];
        if (!tower.is_alive()) continue;
        if (!tower.tick_attack()) continue;

        Position tower_pos = this->tower_positions_[i];
    
        int distance = std::abs(player_pos.row() - tower_pos.row()) + 
                      std::abs(player_pos.col() - tower_pos.col());
        
        if (distance <= tower.attack_range()) {
            this->player_.get_damage(tower.damage());
        }
    }
}

Position GameBoard::GetNextPosition(const Position& current, Direction dir) const {
    int row = current.row();
    int col = current.col();
    
    switch (dir) {
        case Direction::Up:    if (row > 0) --row; break;
        case Direction::Down:  ++row; break;
        case Direction::Left:  if (col > 0) --col; break;
        case Direction::Right: ++col; break;
    }
    
    return Position(row, col);
}

Position GameBoard::PlayerPosition() const { 
    return this->player_position_; 
}

Position GameBoard::EnemyPosition(int index) const {
    if (index < 0 || index >= static_cast<int>(enemy_positions_.size())) {
        return Position(-1, -1);
    }
    return this->enemy_positions_[index];
}

Position GameBoard::TowerPosition(int index) const {
    if (index < 0 || index >= static_cast<int>(tower_positions_.size())) {
        return Position(-1, -1);
    }
    return this->tower_positions_[index];
}

void GameBoard::HealPlayer(int amount) {
    this->player_.heal(amount);
}

void GameBoard::DamageEnemy(int enemy_index, int damage) {
    if (enemy_index < 0 || enemy_index >= static_cast<int>(this->enemies_.size())) {
        return;
    }
    
    this->enemies_[enemy_index].get_damage(damage);
    
    if (!this->enemies_[enemy_index].is_alive()) {
        Position enemy_pos = this->enemy_positions_[enemy_index];
        this->ClearCell(enemy_pos);
        this->player_.add_score(50);
    }
}

void GameBoard::DamageTower(int tower_index, int damage) {
    if (tower_index < 0 || tower_index >= static_cast<int>(this->enemy_towers_.size())) {
        return;
    }
    
    this->enemy_towers_[tower_index].get_damage(damage);
    
    if (!this->enemy_towers_[tower_index].is_alive()) {
        Position tower_pos = this->tower_positions_[tower_index];
        this->ClearCell(tower_pos);
        this->player_.add_score(100);
    }
}

Player &GameBoard::player() {
    return this->player_;
}

void GameBoard::SetPlayerHasTrap(bool has_trap) {
    this->player_.set_trap(has_trap);
}

const std::vector<FriendEnemy> &GameBoard::friends() const {
    return this->friends_;
}

Position GameBoard::FriendPosition(int index) const {
    if (index < 0 || index >= static_cast<int>(this->friend_positions_.size())) {
        return Position(-1, -1);
    }
    return this->friend_positions_[index];
}

bool GameBoard::AddFriend(const Position &pos) {
    if (!this->InBoard(pos)) { return false; }
    if (this->cell_type(pos.row(), pos.col()) != CellType::Empty) { return false; }

    FriendEnemy friendly(50);
    this->friends_.push_back(friendly);
    this->friend_positions_.push_back(pos);
    this->SetCell(pos, CellType::FriendEnemy);

    return true;
}

bool GameBoard::MoveFriend(int friend_index, Direction dir) {
    if (friend_index < 0 || friend_index >= static_cast<int>(this->friends_.size())) { 
        return false; 
    }

    FriendEnemy& friendly = this->friends_[friend_index];
    if (!friendly.is_alive()) { return false; }

    Position pos = this->friend_positions_[friend_index];
    Position next = GetNextPosition(pos, dir);

    if (!InBoard(next)) return false;

    CellType ct = cell_type(next.row(), next.col());
    if (ct == CellType::Wall || ct == CellType::Player || ct == CellType::FriendEnemy || 
        ct == CellType::EnemyTower) { 
        return false; 
    }
    
    if (ct == CellType::Enemy) {
        for (size_t i = 0; i < this->enemies_.size(); ++i) {
            if (this->enemy_positions_[i].row() == next.row() && 
                this->enemy_positions_[i].col() == next.col()) {
                this->DamageEnemy(i, 10);
                return false;
            }
        }
    }

    this->ClearCell(pos);

    if (ct == CellType::Trap) {
        friendly.get_damage(50);
        this->ClearCell(next);
        
        if (!friendly.is_alive()) {
            return true;
        }
    }

    this->friend_positions_[friend_index] = next;
    this->SetCell(next, CellType::FriendEnemy);
    return true;
}

void GameBoard::DamageFriend(int friend_index, int damage) {
    if (friend_index < 0 || friend_index >= static_cast<int>(this->friends_.size())) {
        return;
    }
    
    this->friends_[friend_index].get_damage(damage);
    
    if (!this->friends_[friend_index].is_alive()) {
        Position friend_pos = this->friend_positions_[friend_index];
        this->ClearCell(friend_pos);
    }
}

void GameBoard::SummonFriends(int count) {
    Position player_pos = this->PlayerPosition();
    
    std::vector<Position> possible_positions = {
        Position(player_pos.row() - 1, player_pos.col() - 1),
        Position(player_pos.row() - 1, player_pos.col()),
        Position(player_pos.row() - 1, player_pos.col() + 1),
        Position(player_pos.row(), player_pos.col() - 1),
        Position(player_pos.row(), player_pos.col() + 1),
        Position(player_pos.row() + 1, player_pos.col() - 1),
        Position(player_pos.row() + 1, player_pos.col()),
        Position(player_pos.row() + 1, player_pos.col() + 1)
    };
    
    int summoned = 0;
    for (const Position& pos : possible_positions) {
        if (summoned >= count) break;
        
        if (this->InBoard(pos) && this->cell_type(pos.row(), pos.col()) == CellType::Empty) {
            if (this->AddFriend(pos)) {
                summoned++;
            }
        }
    }
}