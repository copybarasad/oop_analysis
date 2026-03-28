#include "library.h"

Library::Library(const Position& position, SpellType spell) 
    : position_(position), availableSpell_(spell), visited_(false)
{}
LibraryInteractionResult Library::checkInteraction(bool hasSpellInHistory, bool isHandFull) const {
    if(visited_){
        return LibraryInteractionResult::NOTHING;
    }
    if(isHandFull){
       return LibraryInteractionResult::AUTO_RESEARCH;
    }
    return LibraryInteractionResult::CAN_CHOOSE;
}

void Library::confirmInteraction(){
    visited_ = true;
}
Position Library::getPosition() const{
    return position_;
}
bool Library::isVisited() const{
    return visited_;
}
SpellType Library::getAvailableSpell() const{
    return availableSpell_;
}