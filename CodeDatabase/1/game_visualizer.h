#pragma once

#include "game.h"
#include "game_renderer.h"
#include "game_observer.h"
#include "ui_event_manager.h"

template<typename RendererType>
class GameVisualizer : public GameObserver {
private:
    Game& game_;
    RendererType renderer_;
    ConstGameContext createConstContext() const {
        return ConstGameContext(
            game_.getField(),
            game_.getHunter(),
            game_.getDracula()
        );
    }
    
public:
    GameVisualizer(Game& game) : game_(game), renderer_() {
        game_.addObserver(this);
    }
    
    ~GameVisualizer() {
        game_.removeObserver(this);
    }
    
    // Реализация GameObserver
    void onGameStateChanged(const Game& game) override {
        if (game.isPlayerTurn() && !game.isPlayerTurnCompleted()) {
        renderer_.renderGameState(game.getContext());
    }
        processUIEvents();
    }
    
private:
    void processUIEvents() {
        UIEvent event = game_.getUIEventManager().consumeEvent();
        
        if (event.type == UIEventType::NONE) {
            return;
        }
        
        switch (event.type) {
            case UIEventType::STATUS_REQUESTED:
                renderer_.displayStatus(createConstContext());
                break;
                
            case UIEventType::FIELD_REQUESTED:
                renderer_.displayField(createConstContext());
                break;
                
            case UIEventType::SPELLS_REQUESTED:
                renderer_.displaySpells(game_.getHunter());
                break;
                
            case UIEventType::HELP_REQUESTED:
                renderer_.displayHelp(game_.getCommandRegistry());
                break;
                
            case UIEventType::COMMAND_HELP_REQUESTED:
                renderer_.displayHelp(game_.getKeyBindings());
                break;
                
            case UIEventType::SPELL_SELECTION_REQUESTED:
                handleSpellSelectionRequest();
                break;
                
            case UIEventType::LIBRARY_INTERACTION:
                handleLibraryInteractionRequest();
                break;
                
            case UIEventType::LEVEL_UP_INTERFACE:
                handleLevelUpInterfaceRequest();
                break;
                
            case UIEventType::NONE:
                break;
        }
    }
    
    void handleSpellSelectionRequest() {
        const SpellHand& hand = game_.getHunter().getSpellHand();
        int spellCount = hand.getSpellCount();
        
        if (spellCount == 0) {
            renderer_.displayMessage("У вас нет заклинаний!");
            return;
        }
        
        try {
            int spellIndex = renderer_.getSpellIndex(spellCount);
            Position target = renderer_.getSpellTarget();
            
            if (game_.processSpellSelection(spellIndex, target)) {
                renderer_.displayMessage("✨ Заклинание применено успешно!");
            }
        } catch (const std::exception& e) {
            renderer_.displayMessage("Ошибка: " + std::string(e.what()));
        }
    }
    
    void handleLibraryInteractionRequest() {
        Position hunterPos = game_.getHunter().getPosition();
        const Library* library = game_.getField().getLibraryAt(hunterPos);
        
        if (!library) {
            renderer_.displayMessage("Здесь нет библиотеки");
            return;
        }
        
        renderer_.displayLibraryChoice(*library);
        std::string choice = renderer_.getPlayerChoice();
        
        if (choice == "1" || choice == "2") {
            int choiceInt = std::stoi(choice);
            
            if (game_.processLibraryChoice(choiceInt, hunterPos)) {
                std::string message = (choiceInt == 1) 
                    ? "📚 Изучено новое заклинание!" 
                    : "🔬 Проведено исследование Дракулы!";
                renderer_.displayMessage(message);
            }
        } else {
            renderer_.displayMessage("Неверный выбор");
        }
    }
    
   void handleLevelUpInterfaceRequest() {
    int completedLevel = game_.getLevelSystem().getCurrentLevel();
    
    const auto& upgrades = game_.getUpgradeOptions();
    if (upgrades.empty()) {
        renderer_.displayMessage("Нет доступных улучшений");
        return;
    }
    
    renderer_.displayLevelUpOptions(upgrades);
    
    try {
        int choice = renderer_.getUpgradeChoice(upgrades.size());
        if (game_.processUpgradeChoice(choice)) {
            renderer_.displayMessage("Улучшение применено!");
            std::string temp;
            std::getline(std::cin, temp);
            displayLevelInfo();
        }
    } catch (const std::exception& e) {
        renderer_.displayMessage("Ошибка: " + std::string(e.what()));
    }
}
    
public:
    void displayWelcome() {
        renderer_.displayWelcome();
    }
    
    void displayLevelInfo() {
        renderer_.displayLevelInfo(
            createConstContext(),
            game_.getLevelSystem().getCurrentLevel(),
            game_.getLevelSystem().getCurrentLevelConfig()
        );
    }
    
    void displayMessage(const std::string& message) {
        renderer_.displayMessage(message);
    }
    
    void displayQuickHelp() {
        renderer_.displayQuickHelp();
    }
    
    void render() {
        renderer_.renderGameState(createConstContext());
        processUIEvents();
    }
    
    RendererType& getRenderer() { return renderer_; }
};