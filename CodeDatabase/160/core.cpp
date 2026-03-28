#include "../include/core/core.h"
#include <iostream>
#include <cstdlib>

Core::Core() : move_ticker_(0) {}

void Core::InitializeBoard(GameBoard& board) {
    board.AddPlayer(Position(0, 0));
    this->GenerateEnemies(board);
    this->GenerateTowers(board);
    this->GenerateWalls(board, 10);
    this->GenerateFreezeTraps(board, 5);
}

void Core::UpdateGame(GameBoard& board) {
    this->move_ticker_++;

    this->UpdateEnemies(board);
    this->UpdateFriends(board);
    board.UpdateEnemyTowers();
    board.UpdateTowerAttacks();
    board.AddPlayerScore(1);
}

void Core::UpdateEnemies(GameBoard& board) {
    Position player_pos = board.PlayerPosition();
    
    for (size_t i = 0; i < board.enemies().size(); ++i) {
        if (!board.enemies()[i].is_alive()) { 
            board.enemies().erase(board.enemies().begin() + i);
            continue; 
        }

        if (move_ticker_ % (2 + i % 2) != 0) { continue; }

        Position enemy_pos = board.EnemyPosition(i);
        Direction d = Direction::Right;

        int chance = rand() % 100 + 1;
        if (chance < 70) {
            if (enemy_pos.row() < player_pos.row()) d = Direction::Down;
            else if (enemy_pos.row() > player_pos.row()) d = Direction::Up;
            else if (enemy_pos.col() < player_pos.col()) d = Direction::Right;
            else if (enemy_pos.col() > player_pos.col()) d = Direction::Left;
        } else {
            int rand_move = chance % 4;
            switch (rand_move) {
                case 0: d = Direction::Up; break;
                case 1: d = Direction::Down; break;
                case 2: d = Direction::Left; break;
                case 3: d = Direction::Right; break;
            }
        }
        board.MoveEnemy(i, d);
    }
}

void Core::GenerateEnemies(GameBoard& board) {
    board.AddEnemy(Position(0, 7));
    // board.AddEnemy(Position(6, 0));
}

void Core::UpdateFriends(GameBoard& board) {
    for (size_t i = 0; i < board.friends().size(); ++i) {
        if (!board.friends()[i].is_alive()) {
            continue;
            board.friends().erase(board.friends().begin() + i);
        }

        Direction dir = static_cast<Direction>(rand() % 4);
        board.MoveFriend(i, dir);
    }
}

void Core::GenerateTowers(GameBoard& board) {
    // board.AddEnemyTower(Position(20, 5), 150, 15);
}

void Core::GenerateWalls(GameBoard& board, int wall_percentage) {
    if (wall_percentage < 0 || wall_percentage > 50) {
        std::cout << "Warning: wall-процент должен быть в рамках 0-50%. Использую 15%." << std::endl;
        wall_percentage = 15;
    }

    int total_cells = board.rows() * board.cols();
    int walls_to_place = (total_cells * wall_percentage) / 100;
    int walls_placed = 0;
    
    while (walls_placed < walls_to_place) {
        int row = rand() % board.rows();
        int col = rand() % board.cols();

        if ((row != 0 || col != 0) && board.cell_type(row, col) == CellType::Empty) {
            board.SetCell(Position(row, col), CellType::Wall);
            walls_placed++;
        }
    }
}

void Core::GenerateFreezeTraps(GameBoard& board, int freeze_percentage) {
    if (freeze_percentage < 0 || freeze_percentage > 20) {
        std::cout << "Warning: freeze-процент должен быть в рамках 0-15%. Использую 15%." << std::endl;
        freeze_percentage = 5;
    }
    
    int total_cells = board.rows() * board.cols();
    int freeze_traps_to_place = (total_cells * freeze_percentage) / 100;
    int freeze_traps_placed = 0;
    
    while (freeze_traps_placed < freeze_traps_to_place) {
        int row = rand() % board.rows();
        int col = rand() % board.cols();

        if ((row != 0 || col != 0) && board.cell_type(row, col) == CellType::Empty) {
            board.SetCell(Position(row, col), CellType::Freeze);
            freeze_traps_placed++;
        }
    }
}

void Core::InitializeBoardForLevel(GameBoard& board, int level) {
    board.set_level(level);
    
    for (int r = 0; r < board.rows(); ++r) {
        for (int c = 0; c < board.cols(); ++c) {
            board.SetCell(Position(r, c), CellType::Empty);
        }
    }
    
    board.enemies().clear();
    board.friends().clear();
    board.enemy_towers().clear();
    
    GenerateWalls(board, 10);
    
    int freeze_percentage = std::min(5 + level, 15);
    GenerateFreezeTraps(board, freeze_percentage);
    
    Position center(board.rows() / 2, board.cols() / 2);
    while (board.cell_type(center.row(), center.col()) != CellType::Empty) {
        center = Position((center.row() + 1) % board.rows(), (center.col() + 1) % board.cols());
    }
    board.AddPlayer(center);
    
    GenerateEnemiesForLevel(board, level);
    
    GenerateTowersForLevel(board, level);
    
    std::cout << "\n=== УРОВЕНЬ " << level << " ===" << std::endl;
    std::cout << "Враги стали сильнее! Будьте осторожны!" << std::endl;
}

void Core::GenerateEnemiesForLevel(GameBoard& board, int level) {
    int base_enemy_count = 3;
    int enemy_count = base_enemy_count + (level - 1) * 2;
    
    std::vector<Position> empty_positions;
    for (int r = 0; r < board.rows(); ++r) {
        for (int c = 0; c < board.cols(); ++c) {
            if (board.cell_type(r, c) == CellType::Empty) {
                empty_positions.push_back(Position(r, c));
            }
        }
    }

    for (int i = 0; i < enemy_count && !empty_positions.empty(); ++i) {
        int random_idx = rand() % empty_positions.size();
        Position pos = empty_positions[random_idx];
        
        board.AddEnemy(pos);
        
        if (level > 1 && !board.enemies().empty()) {
            int bonus_health = (level - 1) * 20;
            board.enemies().back().set_health(board.enemies().back().max_health() + bonus_health);
            board.enemies().back().set_max_health(board.enemies().back().max_health() + bonus_health);
        }
        
        empty_positions.erase(empty_positions.begin() + random_idx);
    }
}

void Core::GenerateTowersForLevel(GameBoard& board, int level) {
    int tower_count = std::min(1 + (level - 1) / 2, 4);
    
    std::vector<Position> empty_positions;
    for (int r = 0; r < board.rows(); ++r) {
        for (int c = 0; c < board.cols(); ++c) {
            if (board.cell_type(r, c) == CellType::Empty) {
                empty_positions.push_back(Position(r, c));
            }
        }
    }

    for (int i = 0; i < tower_count && !empty_positions.empty(); ++i) {
        int random_idx = rand() % empty_positions.size();
        Position pos = empty_positions[random_idx];
        
        int tower_health = 50 + level * 10;
        int spawn_interval = std::max(5 - level / 2, 2);
        
        board.AddEnemyTower(pos, tower_health, spawn_interval);
        empty_positions.erase(empty_positions.begin() + random_idx);
    }
}