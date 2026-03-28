#include <string>
#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "game_master.h"
#include "saver.h"
#ifndef COMMAND_PROCH
#define COMMAND_PROCH

class game_master;

struct Command
{
    int int_comm;
    std::string str_comm;
};

class IInput
{
    public:
        virtual Command* get_command() = 0;
};

class command_proc: public IInput
{
    public:

        command_proc();

        Command* get_command();

        ~command_proc();

};

template<typename input_type> 
class game_controller
{
    input_type input;
    game_master* gm;
    public:
        game_controller(game_master* _gm) : gm(_gm){}

        void process_input()
        {
            Command* cmd = input.get_command();
            switch (cmd->int_comm)
            {
                case 0:
                {
                    gm->endless_game(false, 0);
                    break;
                }
                case 1:
                {
                    std::vector<std::string> files;
                    std::cout << "Choose file to load (file should be in loadfiles directory): " << std::endl;
                    std::string directory_path = "D:/code/OOP/player_game/loadfiles";
                    auto it = files.end();
                    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) 
                    {
                        if (entry.is_regular_file()) 
                        {
                            std::cout << entry.path().string() << std::endl;
                            files.push_back(entry.path().string());
                        } 
                    }
                    bool flag;
                    std::string input, data;
                    do
                    {
                        std::cin >> input;
                        flag = false;
                        it = std::find(files.begin(), files.end(), input);
                        if (it == files.end())
                        {
                            std::cout << "Wrong input, no such file in directory" << std::endl;
                            flag = true;
                        }
                    } while (flag);
                    saver* save = new saver();
                    data = save->load(input);
                    save->data_decipher(data, gm);
                    gm->endless_game(true, gm->get_level());
                    break;
                }
                case 2:
                {
                    std::cout << HELP << std::endl;
                    gm->menu();
                    break;
                }
                case 3:
                {
                    exit(0);
                    break;
                }
                default:
                    std::cout << "No such option" << std::endl;
                    gm->menu();
                    break;
            }
            delete cmd;
        };

        ~game_controller(){};
};

#endif