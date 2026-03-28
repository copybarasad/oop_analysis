//
// Created by Artem on 17.12.2025.
//

#ifndef LABAOOP2_INPUTKEYSMODEL_H
#define LABAOOP2_INPUTKEYSMODEL_H

#include <string>
#include <map>
#include <vector>

class InputKeysModel{
public:
    char saveGameKey, confirmActionKey, dismissActionKey, attackKey, spellKey, skipKey, continueKey;

    InputKeysModel(char closeGameKey, char confirmActionKey, char dismissActionKey, char attackKey, char spellKey,
                   char skipMoveKey, char continueKey) : saveGameKey(closeGameKey), confirmActionKey(confirmActionKey),
                                                         dismissActionKey(dismissActionKey), attackKey(attackKey),
                                                         spellKey(spellKey), skipKey(skipMoveKey),
                                                         continueKey(continueKey) {}
    static InputKeysModel* deserialize(std::map<std::string, std::string>& json);
    std::string serialize();
private:
    static char getCharValueFromJson(const std::map<std::string, std::string>& json, const std::string& key, char defaultValue);
    static bool hasDuplicateValues(const std::vector<char>& values);
    static const char DEFAULT_SAVE_GAME_KEY = 99;       // 'c'
    static const char DEFAULT_CONFIRM_ACTION_KEY = 121; // 'y'
    static const char DEFAULT_DISMISS_ACTION_KEY = 110; // 'n'
    static const char DEFAULT_ATTACK_KEY = 97;          // 'a'
    static const char DEFAULT_SPELL_KEY = 115;          // 's'
    static const char DEFAULT_SKIP_KEY = 107;           // 'k'
    static const char DEFAULT_CONTINUE_KEY = 117;       // 'u'
};

#endif //LABAOOP2_INPUTKEYSMODEL_H
