#include "renderer.h"
#include "field.h"
#include "player.h"
#include "hand.h"
#include "ally.h"
#include <iomanip>

char Renderer::CellToChar(CellType type) const {
    switch (type) {
        case CellType::kEmpty: return '.';
        case CellType::kObstacle: return '#';
        case CellType::kSlow: return '~';
        case CellType::kPlayer: return 'P';
        case CellType::kEnemy: return 'E';
        case CellType::kBuilding: return 'B';
        case CellType::kTrap: return 'T';
        case CellType::kTower: return 'W';
        case CellType::kAlly: return 'A';
        default: return '?';
    }
}

void Renderer::RenderSeparator(int width) const {
    std::cout << "+";
    for (int i = 0; i < width; ++i) {
        std::cout << "-";
    }
    std::cout << "+\n";
}

void Renderer::RenderField(const Field& field) const {
    const auto& grid = field.GetGrid();
    int width = field.GetWidth();
    int height = field.GetHeight();
   
    RenderSeparator(width);
    
    for (int y = 0; y < height; ++y) {
        std::cout << "|";
        for (int x = 0; x < width; ++x) {
            bool isTrap = false;
            for (const auto& trap : field.GetTraps()) { 
                if (trap->GetX() == x && trap->GetY() == y) {
                    isTrap = true;
                    break;
                }
            }
            
            bool isTower = false;
            for (const auto& tower : field.GetTowers()) { 
                if (tower->GetX() == x && tower->GetY() == y) {
                    isTower = true;
                    break;
                }
            }
            
            bool isAlly = false;
            for (const auto& ally : field.GetAllies()) {  
                if (ally->GetX() == x && ally->GetY() == y) {
                    isAlly = true;
                    break;
                }
            }
            
            if (isTrap) {
                std::cout << 'T';
            } else if (isTower) {
                std::cout << 'W';
            } else if (isAlly) {
                std::cout << 'A';
            } else {
                std::cout << CellToChar(grid[y][x].GetType());
            }
        }
        std::cout << "|\n";
    }
    
    // Нижняя граница
    RenderSeparator(width);
}

void Renderer::RenderPlayerInfo(const Player& player) const {
    std::cout << "\n=== PLAYER INFO ===\n";
    std::cout << "Health: " << player.GetHealth() << "/" << player.GetMaxHealth() << "\n";
    std::cout << "Points: " << player.GetPoints() << "\n";
    std::cout << "Fight Mode: " << player.GetFightModeName() << "\n";
    std::cout << "Damage: " << player.GetDamage() << "\n";
    std::cout << "Kills: " << player.GetKillCount() << "\n";
    
    std::cout << "Buffs:\n";
    std::cout << "  Range: +" << player.GetBuffRange() << "\n";
    std::cout << "  Area: +" << player.GetBuffArea() << "\n";
    std::cout << "  Trap Damage: +" << player.GetBuffTrapDamage() << "\n";
    std::cout << "  Summons: +" << player.GetBuffSummons() << "\n";
}

void Renderer::RenderHand(const Hand& hand) const {
    std::cout << "\n=== YOUR HAND ===\n";
    if (hand.GetSize() == 0) {
        std::cout << "Hand is empty\n";
    } else {
        hand.Display();
    }
    std::cout << "Size: " << hand.GetSize() << "/5\n";
}

void Renderer::RenderGameState(const Field& field) const {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "               GAME STATE              \n";
    std::cout << "========================================\n";
    
    RenderField(field);
    
    if (field.GetPlayer()) {
        RenderPlayerInfo(*field.GetPlayer());
        RenderHand(field.GetPlayer()->GetHand());
    }
    
    std::cout << "\n=== ENEMIES ===\n";
    std::cout << "Total enemies: " << field.GetEnemies().size() << "\n";
    std::cout << "Total buildings: " << field.GetBuildings().size() << "\n";
    std::cout << "Total towers: " << field.GetTowers().size() << "\n";
    std::cout << "Total allies: " << field.GetAllies().size() << "\n";
    std::cout << "Total traps: " << field.GetTraps().size() << "\n";
    
    std::cout << "\n";
}

void Renderer::RenderMessage(const std::string& message) const {
    std::cout << "> " << message << "\n";
}

void Renderer::RenderHelp() const {
    std::cout << "\n=== AVAILABLE COMMANDS ===\n";
    std::cout << "Movement:\n";
    std::cout << "  w - move up\n";
    std::cout << "  a - move left\n";
    std::cout << "  s - move down\n";
    std::cout << "  d - move right\n";
    
    std::cout << "\nActions:\n";
    std::cout << "  attack <x> <y> - attack target at coordinates\n";
    std::cout << "  switch_mode - switch between melee and ranged\n";
    std::cout << "  buy_spell - buy a random spell (cost: 1 point)\n";
    std::cout << "  info - show detailed game information\n";
    
    std::cout << "\nSpells:\n";
    std::cout << "  cast <index> <x> <y> - cast spell from hand\n";
    
    std::cout << "\nGame:\n";
    std::cout << "  save - save game\n";
    std::cout << "  load - load game\n";
    std::cout << "  quit - quit game\n";
    
    std::cout << "\n";
}
