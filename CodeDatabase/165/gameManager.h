#ifndef GAMEMANAGERH
#define GAMEMANAGERH
#include "game.h"
#include "consoleReader.h"
template<typename InputReader>
class gameManager{
    InputReader* reader;
    //VisualRender* render;
public:
    void sendCommand(game* gameObj){
        command cmd = this->reader->parseCommand(this->reader->readLine());

        if(!cmd.valid){
            std::cout << "Invalid command format\n";
            return;
        }

        if(cmd.name == "New_game") {
            gameObj->newGame();
        }else if (cmd.name == "Continue") {
            gameObj->continueGame();
        }else if (cmd.name == "Load_game"){
            gameObj->load();
        }else if(cmd.name == "Save_game"){
            gameObj->save();
        }else if(cmd.name == "Menu"){
            gameObj->menu();
        }else if(cmd.name == "Exit"){
            gameObj->close();
        }else{
            std::cout << "Unknown command: " << cmd.name << '\n';
        }
    };

    void updateVisual(game* gameObj){

    };
};
#endif