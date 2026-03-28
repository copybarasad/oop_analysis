#ifndef GAME_H
#define GAME_H

#include "GameGrid.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class FileSystemException : public GameException {
public:
    explicit FileSystemException(const std::string& msg) : GameException(msg) {}
};

class FileNotFoundException : public FileSystemException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : FileSystemException("Файл не найден: " + filename) {}
};

class FilePermissionException : public FileSystemException {
public:
    explicit FilePermissionException(const std::string& filename, const std::string& operation)
        : FileSystemException("Нет прав: невозможно " + operation + " файл: " + filename) {}
};

class CorruptedSaveException : public GameException {
public:
    explicit CorruptedSaveException(const std::string& details)
        : GameException("Файл сохранения поврежден: " + details) {}
};

class GameStateException : public GameException {
public:
    explicit GameStateException(const std::string& msg) : GameException(msg) {}
};

class SaveFile {
private:
    std::fstream file;
    std::string filename;
    
public:
    SaveFile(const std::string& filename, std::ios::openmode mode);
    ~SaveFile();
    
    std::fstream& get() { return file; }
    bool isOpen() const { return file.is_open(); }
    void close();
    
    SaveFile(const SaveFile&) = delete;
    SaveFile& operator=(const SaveFile&) = delete;
    
    SaveFile(SaveFile&& other) noexcept;
    SaveFile& operator=(SaveFile&& other) noexcept;
};

class Game {
private:
    std::unique_ptr<GameGrid> currentLevel;
    int currentLevelNumber;
    int totalScore;
    bool isPaused;
    
    enum class InternalState {
        MAIN_MENU,
        PLAYING,
        LEVEL_COMPLETE,
        GAME_OVER
    } internalState;
    
    enum class PlayerCommand {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        TOGGLE_MODE,
        CAST_SPELL,
        SAVE_GAME,
        LOAD_GAME,
        SHOW_HELP,
        PAUSE,
        QUIT,
        NONE
    };
    
    bool shouldExit;
    
    void processMainMenu();
    void processPlayingState();
    void processLevelComplete();
    void processGameOver();
    
    void executePlayerTurn(PlayerCommand command);
    void executeAlliesTurn();
    void executeEnemiesTurn();
    void executeStructuresTurn();
    
    void displayMainMenu() const;
    void displayHelp() const;
    void displayGameState() const;
    
    PlayerCommand parseInput(char input) const;
    bool checkLevelCompletion() const;
    bool checkGameOver() const;
    
    void saveGameData(SaveFile& file) const;
    void loadGameData(SaveFile& file);
    
    void showSaveMenu();
    void showLoadMenu();
    
public:
    Game();
    ~Game() = default;
    
    void run();
    
    void handleInput(char input);
    void update();
    void render() const;
    
    static bool saveExists(const std::string& filename);
    static std::vector<std::string> listSaves();
    static bool validateSaveFile(const std::string& filename);
    
    int getCurrentLevel() const { return currentLevelNumber; }
    int getScore() const { return totalScore; }
    bool isGameOver() const { return internalState == InternalState::GAME_OVER; }
    bool isLevelComplete() const { return internalState == InternalState::LEVEL_COMPLETE; }
};

#endif
