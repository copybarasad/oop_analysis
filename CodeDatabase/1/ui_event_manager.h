#pragma once

#include <string>
#include <vector>

class EventSystem;
class GeneralEvent;

enum class UIEventType {
    NONE,                       // Нет события
    STATUS_REQUESTED,           // Показать статус
    FIELD_REQUESTED,            // Показать карту  
    SPELLS_REQUESTED,           // Показать заклинания
    HELP_REQUESTED,             // Показать помощь
    COMMAND_HELP_REQUESTED,     // Показать команды
    SPELL_SELECTION_REQUESTED,  // Выбор заклинания
    LIBRARY_INTERACTION,        // Взаимодействие с библиотекой
    LEVEL_UP_INTERFACE          // Интерфейс прокачки
};

struct UIEvent {
    UIEventType type = UIEventType::NONE;
    
    UIEvent() = default;
    explicit UIEvent(UIEventType t) : type(t) {}
};

class UIEventManager {
private:
    UIEvent current_event_;
    EventSystem* event_system_; 
    
public:
    UIEventManager();
    
    
    void setEventSystem(EventSystem* event_system);
    EventSystem* getEventSystem() const;
    bool hasEventSystem() const;
    
    
    void requestStatus();
    void requestField();
    void requestSpells();
    void requestHelp();
    void requestCommandHelp();
    void requestSpellSelection();
    void requestLibraryInteraction();
    void requestLevelUpInterface();
    
    
    UIEvent consumeEvent();
    UIEvent getCurrentEvent() const;
    bool hasEvent() const;
    bool hasEventOfType(UIEventType type) const;
    void clearEvent();
    
    bool isStatusRequested() const;
    bool isFieldRequested() const;
    bool isSpellsRequested() const;
    bool isSpellSelectionRequested() const;
    bool isLibraryInteractionRequested() const;
    bool isLevelUpInterfaceRequested() const;
    bool isHelpRequested() const;
    
private:
    void logUIEvent(const std::string& event_type, const std::string& message);
};