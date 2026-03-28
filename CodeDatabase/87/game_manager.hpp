#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP
#include <string>

class Game_manager{
    private:
        int get_input(const std::string& str, int min_v, int max_v);
        bool ask_yes_no(const std::string& str);
    public:
        void start();
};

#endif