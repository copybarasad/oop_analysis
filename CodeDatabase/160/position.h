#ifndef INCLUDE_POSITION
#define INCLUDE_POSITION

class Position {
private:
	int row_;
	int col_;

public:
	Position() : row_(0), col_(0) {};
	Position(int row, int col) : row_(row), col_(col) {};

	int row() const { return this->row_; };
	int col() const { return this->col_; };

	bool operator == (Position &other) {
		return this->row_ == other.row_ && this->col_ == other.col_;
	}
};

#endif
