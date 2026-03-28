#ifndef CELL_H
#define CELL_H

enum class CellContent { Empty, Player, Enemy, Building, Trap, Impassable, Slow, Ally };

class Cell {
public:
	Cell();
	CellContent getContent() const;
	int getOccupantId() const;
	void setEmpty();
	void setOccupant(CellContent type, int id);
private:
	CellContent content_;
	int occupantId_;
};

#endif
