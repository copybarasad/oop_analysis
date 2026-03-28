#pragma once
#include "position.h"
#include "spell.h"

enum class LibraryInteractionResult {
    NOTHING,       // Уже посещена
    CAN_CHOOSE,    // Может выбрать между заклинанием и исследованием
    AUTO_RESEARCH  // Автоматически дает исследование (рука полная или заклинание уже есть)
};

class Library {
private:
    Position position_;
    SpellType availableSpell_;
    bool visited_;

public:
    Library(const Position& position, SpellType spell);
    
    LibraryInteractionResult checkInteraction(bool hasSpellInHistory, bool isHandFull) const;
    void confirmInteraction();
    
    Position getPosition() const;
    bool isVisited() const;
    SpellType getAvailableSpell() const;

};