#ifndef OOP_POSITION_H
#define OOP_POSITION_H

class Position {
    int x;
    int y;
public:
    explicit Position(int x = 0, int y = 0);

    int get_x() const;
    int get_y() const;

    void set_x(int new_x);
    void set_y(int new_y);

    // перегрузка оператора сравнения двух позиций
    bool operator==(const Position& other) const;

    int find_distance_to(Position target_position) const;
};

enum class Direction { UP, RIGHT, DOWN, LEFT };

#endif //OOP_POSITION_H