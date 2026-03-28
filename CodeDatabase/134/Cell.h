#pragma once

class Cell {
public:
    enum class CellType { Empty, Player, Enemy };

    Cell();

    void SetContent(CellType type);
    CellType GetContent() const;    

private:
    CellType content; 
};