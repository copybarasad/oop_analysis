#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "command.hpp"
#include "field/point.hpp"

class Game;

template<typename InputReader>
class GameController {
private:
    InputReader input_reader; 

public:
    GameController();
    
    void process_input(Game* game);
    
    Command get_next_command();
    
    Command get_confirm_command();
    
    bool move_cursor(Command command, Point& point, int xmin, int xmax, int ymin, int ymax);
};

#endif