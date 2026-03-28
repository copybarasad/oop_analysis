#include "SpellExceptionHandler.h"
#include <iostream>

bool SpellExceptionHandler::handleSpellCast(const std::function<bool()>& spellCastOperation, const std::string& spellName) {
    try {
        return spellCastOperation();
    }
    catch (const std::exception& e) {
        std::cout << "Error during " << spellName << " cast: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "Unknown error during " << spellName << " cast!" << std::endl;
        return false;
    }
}