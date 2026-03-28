#include "ConsoleRenderer.h"

#ifdef _WIN64
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif


#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_WHITE "\033[37m"
#define COLOR_LIGHTGRAY "\033[90m"
#define COLOR_DARK_MAGENTA "\033[35m"




std::string CellSymbolColored(Cell::CellType type) {
    switch (type) {
        case Cell::CellType::Empty:
            return COLOR_LIGHTGRAY " . " COLOR_RESET;
        case Cell::CellType::Wall:
            return COLOR_WHITE " # " COLOR_RESET;
        case Cell::CellType::Slow:
            return COLOR_BLUE " ~ " COLOR_RESET;
        default:
            return " ? ";
    }
}



void ConsoleRenderer::Render(GameWorld& world, GameLogic& logic) {
    ClearConsole();
    Print(world, logic);
}



void ConsoleRenderer::ClearConsole() {
    if (system(CLEAR_COMMAND) != 0)
        std::cerr << "Warning: failed to clear console\n";
}



void ConsoleRenderer::Print(GameWorld& world, const GameLogic& logic) {
    const GameMap& map = world.GetMap();
    Player& player = world.GetPlayer();
    const auto& enemies = world.GetEnemies();
    const auto& spawners = world.GetSpawners();
    const auto& towers = world.GetTowers();
    const auto& traps = world.GetTraps();
    const auto& allies = world.GetAllies();
    const auto& log = logic.GetEventSystem().GetRecentEvents();

    int width = map.GetWidth();
    int height = map.GetHeight();
    int logSize = log.size();


    std::cout << "┌";
    for (int x = 0; x < width; ++x) std::cout << "───";
    std::cout << "┐";

    if (logic.GetEventSystem().GetTarget() == LogTarget::Screen){
        if (logSize > 0)
            std::cout << COLOR_GREEN << "      │" << COLOR_RESET << "Action Log:";
    }
    std::cout << "\n";



    for (int y = 0; y < height; ++y) {
        std::cout << "│";

        for (int x = 0; x < width; ++x) {
            Position pos{x, y};
            bool drawn = false;


            if (pos == player.GetPosition()) {
                std::cout << COLOR_GREEN << " P " << COLOR_RESET;
                continue;
            }


            for (const auto& e : enemies) {
                if (e && e->GetPosition() == pos && e->IsAlive()) {
                    std::cout << COLOR_RED << " E " << COLOR_RESET;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;


            for (const auto& tr : traps) {
                if (tr && tr->GetPosition() == pos) {
                    std::cout << COLOR_YELLOW << " ^ " << COLOR_RESET;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;


            for (const auto& al : allies) {
                if (al && al->GetPosition() == pos) {
                    std::cout << COLOR_GREEN << " A " << COLOR_RESET;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;


            for (const auto& s : spawners) {
                if (s && s->GetPosition() == pos && s->IsAlive()) {
                    std::cout << COLOR_YELLOW << " B " << COLOR_RESET;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;


            for (const auto& t : towers) {
                if (t && t->IsAlive() && t->GetPosition() == pos) {
                    std::cout << COLOR_DARK_MAGENTA << " T " << COLOR_RESET;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;


            std::cout << CellSymbolColored(map.GetCellType(x, y));
        }


        std::cout << "│ " << (y < 10 ? " " : "") << y;
        if (logic.GetEventSystem().GetTarget() == LogTarget::Screen){
            int logIndex = y;
            if (logIndex < logSize) {
                std::cout << COLOR_GREEN << "   │" << COLOR_RESET << log[logIndex];
            }
        }

        std::cout << "\n";
    }


    std::cout << "└";
    for (int x = 0; x < width; ++x) std::cout << "───";
    std::cout << "┘\n";



    std::cout << " ";
    for (int x = 0; x < width; ++x) {
        if (x < 10)
            std::cout << " " << x;
        else
            std::cout << x;
        std::cout << " ";
    }
    std::cout << "\n\n";



    std::cout << "Player HP: " << player.GetHealth()
              << " | Score: " << player.GetScore()
              << " | Damage: " << player.GetDamage()
              << " | Mode: "
              << (player.GetMode() == Player::CombatMode::Melee ? "Melee" : "Ranged")
              << " | Level: " << world.GetCurrentLevel()
              << "\n";


    auto& hand = player.GetHand().GetSpells();
    std::cout << "\nSpells: ";
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << "[" << i << "] " << hand[i]->GetName() << "  ";
    }
    std::cout << "\n";

    if (logic.IsGameOver()) {
        std::cout << "\n=== GAME OVER ===\n";
    }
}
