#include "Cell.h"

Cell::Cell() : content(CellType::Empty) {}

void Cell::SetContent(CellType type) {
    content = type;
}

Cell::CellType Cell::GetContent() const {
    return content;
}
