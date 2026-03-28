#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <stdexcept>

class Field{
public:
    struct Cell{
        bool isWall;
        int occupantID = -1;
        bool isPlayer = false;
        bool isEnemy = false;

        Cell() : isWall(false), occupantID(-1), isPlayer(false), isEnemy(false) {}
    };

private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    void copyField(const Field& other);

public:
    Field(int w, int h, const std::vector<std::pair<int, int>>& walls = {});
    Field(const Field& other);                      
    Field(Field&& other) noexcept;                  
    Field& operator=(const Field& other);           
    Field& operator=(Field&& other) noexcept; 
    
    bool isValidPos(int x, int y) const;
    bool isPassable(int x, int y) const;
    bool isCellEmpty(int x, int y) const;
    bool canMoveTo(int x, int y) const;

    void placeCharacter(int ID, int x, int y, bool isPlayer, bool isEnemy);
    void removeCharacter(int x, int y);
    void moveCharacter(int oldX, int oldY, int newX, int newY, int ID, bool isPlayer, bool isEnemy);

    int getWidth() const {return width;}
    int getHeight() const {return height;}
    const Cell& getCell (int x, int y) const;

    void save(std::ostream& out) const;
    void load(std::istream& in);
};

#endif