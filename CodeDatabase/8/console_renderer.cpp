#include "console_renderer.h"
#include "game.h"        
#include "field.h"        
#include "node.h"        
#include "player.h"      
#include "enemy.h"       
#include "enemytower.h"  
#include "spell.h" 
using namespace std;

void ConsoleRenderer::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleRenderer::render(const Game& game) {
    
    const Field& field = *game.getField();
    for (int y = 0; y < field.getlength(); y++) {
        for (int x = 0; x < field.getwidth(); x++) {
            const Node& node = field.getNode(x, y);
            cout << getSymbolForNode(node) << " ";
        }
        cout << endl;
    }
    
}

void ConsoleRenderer::renderMessage(const string& message) {
    cout << message << endl;
}

void ConsoleRenderer::showPrompt(const string& prompt) {
    cout << prompt;
    cout.flush();
}

char ConsoleRenderer::getSymbolForNode(const Node& node) {
    if (node.hasPlayer()) return 'P';
    if (node.hasEnemy()) return 'E';
    if (node.hasTower()) return 'T';
    if (node.hasTrap()) return 'X';
    if (node.hasObstacle()) return '#';
    return '.';
}