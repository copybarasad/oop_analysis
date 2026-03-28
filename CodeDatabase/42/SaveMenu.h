#pragma once
#include "SaveManager.h"
#include "InputHandler.h"
#include "Renderer.h"
#include <string>
#include <memory>

class SaveMenu {
private:
    SaveManager saveManager;
    
    void displaySaveSlots() const;
    int getSlotChoice() const;
    bool confirmOverwrite(int slot) const;
    
public:
    SaveMenu();
    
    bool showSaveMenu(const SaveData& data);
    bool showLoadMenu(SaveData& data);
    void showDeleteMenu();
    void showSaveInfo() const;
};