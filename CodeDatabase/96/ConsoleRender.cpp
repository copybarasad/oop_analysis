#include "ConsoleRender.h"

#include <iostream>

std::string SpellTypeToString(SpellType type) {
    switch(type) {
        case SpellType::directDamage: return "directDamage";
        case SpellType::areaDamage: return "areaDamage";
        case SpellType::trap: return "trap";
        case SpellType::summAlly: return "summAlly";
        case SpellType::empower: return "empower";
        case SpellType::none: return "none";
        default: return "unknown";
    }
}

void ConsoleRenderer::Render(const GameManager& gm, std::string state) {
    system("clear");
    if (state == "settings") {
        std::cout << "=== SETTINGS MENU ===" << std::endl;
        std::cout << "Type 'p' to save the game." << std::endl;
        std::cout << "Type 'l' to load the game." << std::endl;
        std::cout << "Type 'b' to return to the game." << std::endl;
        std::cout << "Type 'e' to exit the game." << std::endl;
        return;
    } if (state == "newlevel") {
        std::cout << "=== NEW LEVEL REACHED ===" << std::endl;
        std::cout << "Type 'improve health' to increase your health." << std::endl;
        std::cout << "Type 'improve hand' to improve your spell hand." << std::endl;
        return;
    } if (state == "newgame") {
        std::cout << "=== NEW GAME MENU ===" << std::endl;
        std::cout << "Type 'new' to start a new game." << std::endl;
        std::cout << "Type 'load' to load a saved game." << std::endl;
        return;
    }
    const Field& field = gm.GetField();
    const PlayerManager& player = gm.GetPlayer();
    for (int y = 0; y < field.Getheight(); ++y) {
        for (int x = 0; x < field.Getwidth(); ++x) {
            auto pos = std::make_pair(x, y);
            if (player.GetPlayerPosition() == pos) {
                std::cout << "P "; 
            } else if (field.IsOccupiedAtPos(pos)) {
                if (gm.GetEnemies().IsEnemyAtPos(pos)) {
                    std::cout << "E "; 
                } else if (gm.GetBuildings().IsBuildingAtPos(pos)) {
                    std::cout << "B "; 
                } else if (gm.GetTowers().IsTowerAtPos(pos)) {
                    std::cout << "T "; 
                } else if (gm.GetAllies().IsAllyAtPos(pos)) {
                    std::cout << "A "; 
                }  else {
                    std::cout << "? "; 
                }
            } else if (field.GetCellTypeAt(pos) == CellType::Wall) {
                std::cout << "# ";
            } else if (gm.GetField().GetCellTypeAt(pos) == CellType::Stun) {
                    std::cout << "S ";
            } else if (gm.GetField().GetTrapDamageAtPos(pos) > 0) {
                    std::cout << "^ ";
            } else {
                std::cout << ". "; 
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Player Health: " << player.GetPlayerHealth() << std::endl;
    std::cout << "Player Score: " << player.GetPlayerScore() << std::endl;
    std::cout << "Player Position: (" << player.GetPlayerPosition().first << ", " << player.GetPlayerPosition().second << ")\n";
    std::cout << "Current Spell: " << SpellTypeToString(gm.GetHand().GetCurrentSpellType()) << '\n';
    //std::cout << "Current Spell: " << gm.GetHand().GetCurrentSpellType() << '\n';
}

void ConsoleRenderer::Print(const std::string& msg) {
    std::cout << msg << std::endl;
}