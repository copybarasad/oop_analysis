#ifndef INCLUDE_GAME_BOARD
#define INCLUDE_GAME_BOARD

#include <vector>

#include "../ui/cell.h"
#include "../entity/player.h"
#include "../entity/enemy.h"
#include "../entity/friend_enemy.h"
#include "../entity/enemy_tower.h"
#include "../ui/direction.h"

class GameBoard {
private:
	int Index(int row, int col) const;
	Position GetNextPosition(const Position& current, Direction dir) const;

	std::size_t rows_count_;
	std::size_t cols_count_;
	std::vector<Cell> cells_;
	
	Player player_;
	std::vector<Enemy> enemies_;
    std::vector<FriendEnemy> friends_;
    std::vector<EnemyTower> enemy_towers_;

	Position player_position_;
    std::vector<Position> enemy_positions_;
    std::vector<Position> friend_positions_;
    std::vector<Position> tower_positions_;

	int current_level_; 

public:
	GameBoard(int rows, int cols);
	GameBoard(GameBoard &other);
	GameBoard &operator=(GameBoard &other);
	GameBoard(GameBoard &&other);
	GameBoard &operator=(GameBoard &&other);

	~GameBoard();

	int rows() const;
	int cols() const;

	const Player &player() const;
	Player &player(); 
	const std::vector<Enemy> &enemies() const;
    const std::vector<FriendEnemy> &friends() const;
	const std::vector<EnemyTower> &enemy_towers() const;
	std::vector<Enemy>& enemies() { return enemies_; }
	std::vector<FriendEnemy>& friends() { return friends_; }
	std::vector<EnemyTower>& enemy_towers() { return enemy_towers_; }


	bool AddPlayer(const Position &pos);
	bool MovePlayer(Direction dir);
	void HealPlayer(int amount);
	void DamagePlayer(int damage);
	void AddPlayerScore(int points);
    void SetPlayerFrozen(bool frozen);
    void SetPlayerOnFreeze(bool on_freeze);
	void ChangePlayerWeapon();

	bool AddEnemy(const Position &pos);
	bool MoveEnemy(int enemy_index, Direction dir);
	void DamageEnemy(int enemy_index, int damage);

	bool AddFriend(const Position &pos);
    bool MoveFriend(int friend_index, Direction dir);
    void DamageFriend(int friend_index, int damage);

	bool AddEnemyTower(const Position &pos, int health = 50, int spawn_interval = 5);
    void UpdateEnemyTowers();
	void DamageTower(int tower_index, int damage);
	void UpdateTowerAttacks();

	Position FindEmptySpace(const Position &center) const;
    Position PlayerPosition() const;
    Position EnemyPosition(int index) const;
    Position FriendPosition(int index) const;
	Position TowerPosition(int index) const;

    void SummonFriends(int count);

	CellType cell_type(int r, int c) const;
	void ClearCell(const Position &p);
	void SetCell(const Position &p, CellType t);
	bool InBoard(const Position &p) const;
	void SetPlayerHasTrap(bool has_trap);

    int current_level() const { return current_level_; }
    void set_level(int level) { current_level_ = level; }
};

#endif
