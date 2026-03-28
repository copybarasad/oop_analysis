#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

class FieldGrid;

class PlayerController {
public:
	PlayerController();
	
	void SetPosition(int x, int y);
	int GetX() const;
	int GetY() const;
	
	bool Move(int deltaX, int deltaY, FieldGrid& grid);
	bool CanMoveTo(int x, int y, FieldGrid& grid) const;

private:
	int playerX;
	int playerY;
};

#endif



