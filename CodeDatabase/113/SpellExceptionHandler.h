#ifndef SPELLEXCEPTIONHANDLER_H
#define SPELLEXCEPTIONHANDLER_H

#include <functional>
#include <string>

class SpellExceptionHandler {
public:
    static bool handleSpellCast(const std::function<bool()>& spellCastOperation, const std::string& spellName);
};

#endif // SPELLEXCEPTIONHANDLER_H