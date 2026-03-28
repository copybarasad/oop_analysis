#ifndef CELL_H
#define CELL_H

class Cell {
private:
    char content;

public:
    Cell();
    char getContent() const;
    void setContent(char newContent);
    bool isEmpty() const;
    void clear();
};

#endif

