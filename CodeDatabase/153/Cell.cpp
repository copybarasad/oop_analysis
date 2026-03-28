#include "Cell.h"

Cell::Cell() : content('.') {
}

char Cell::getContent() const {
    return content;
}

void Cell::setContent(char newContent) {
    content = newContent;
}

bool Cell::isEmpty() const {
    return content == '.';
}

void Cell::clear() {
    content = '.';
}
