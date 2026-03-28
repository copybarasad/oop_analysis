#include "cell.h"

namespace game {
    // ----------- Reset icon according to type -----------
    void Cell::resetIcon()
    {
        char newIcon = Icons::Floor;
        switch (type_) {
            case CellType::Floor: newIcon = Icons::Floor; break;
            case CellType::Wall:  newIcon = Icons::Wall;  break;
            case CellType::Slime: newIcon = Icons::Slime; break;
        }

        icon_ = newIcon;
    }
}