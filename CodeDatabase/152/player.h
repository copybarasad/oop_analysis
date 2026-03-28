#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_
class Player {
public:
    explicit Player(int health = 100, int damage = 10, int score = 0);

    int health() const { return health_; }
    int damage() const { return damage_; }
    int score() const { return score_; }
    void take_damage(int damage);
    void add_score(int points);
    bool is_alive() const;
    void set_position(int row, int col);
    int row() const { return row_; }
    int col() const { return col_; }
private:
    int health_;
    int damage_;
    int score_;
    int row_ = -1;
    int col_ = -1;
};
#endif