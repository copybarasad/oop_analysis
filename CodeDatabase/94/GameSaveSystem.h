#ifndef GAMESAVESYSTEM_H
#define GAMESAVESYSTEM_H

#include "GameController.h"
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

class SaveException : public std::exception {
private:
    std::string message;
public:
    explicit SaveException(const std::string& msg) : message("Save Error: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class LoadException : public std::exception {
private:
    std::string message;
public:
    explicit LoadException(const std::string& msg) : message("Load Error: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class CorruptedSaveException : public std::exception {
private:
    std::string message;
public:
    explicit CorruptedSaveException(const std::string& msg) : message("Corrupted Save: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class GameSaveSystem {
private:
    std::string filename;
    
    class SaveFile {
    private:
        std::fstream file;
        bool isOpen;
        
    public:
        SaveFile(const std::string& filename, std::ios_base::openmode mode) 
            : isOpen(false) {
            file.open(filename, mode);
            if (!file.is_open()) {
                throw SaveException("Cannot open file: " + filename);
            }
            isOpen = true;
        }
        
        ~SaveFile() {
            if (isOpen) {
                file.close();
            }
        }
        
        std::fstream& get() { return file; }
        
        SaveFile(const SaveFile&) = delete;
        SaveFile& operator=(const SaveFile&) = delete;
    };

public:
    GameSaveSystem(const std::string& saveFile = "game_save.dat") : filename(saveFile) {}
    
    bool saveGame(const GameController& game, int currentLevel, int playerMaxHealth);
    bool loadGame(GameController& game, int& currentLevel, int& playerMaxHealth);
    
    std::string getSaveFileName() const { return filename; }
    void setSaveFileName(const std::string& name) { filename = name; }
};

#endif