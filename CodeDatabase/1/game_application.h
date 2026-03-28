// game_application.h
#pragma once

#include "game.h"
#include "event_system.h"

class FileLogger;
class ConsoleLogger;

class GameApplication {
private:
    EventSystem* event_system_;
    FileLogger* file_logger_;
    ConsoleLogger* console_logger_;
    Game* current_game_;
    bool is_running_;  
    
    void setupLogging(int log_choice);
    void cleanup();
    bool promptForRestart();
    std::string getPlayerName() const;
    void runGameWithControllers(Game& game);
    int getLoggingChoice();
    bool handleSaveGame(const std::string& hunter_name);
    void startNewGame(const std::string& hunter_name);
    void playGameSession();
    void showWelcomeMessage();
    
public:
    GameApplication();
    ~GameApplication();
    
    void run();
    

    void stop();  
    bool isRunning() const { return is_running_; }
};