#include "cell.h"

Cell::Cell(): content_(CellContent::Empty), occupantId_(-1) {}
CellContent Cell::getContent() const { return content_; }
int Cell::getOccupantId() const { return occupantId_; }
void Cell::setEmpty() { content_ = CellContent::Empty; occupantId_ = -1; }
void Cell::setOccupant(CellContent type, int id) { content_ = type; occupantId_ = id; }
