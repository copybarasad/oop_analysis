#include "game.hpp"
#include "controls.hpp"
#include "database.hpp"

class App {
private:
    DataBase db;
    bool show_menu;
    Game* game;
public:
    App(std::string db_name);
    void run();
private:
    Game* load_game();
    Game* create_game(char com);
};