#include "ui_event_manager.h"
#include "event_system.h"      
#include "game_events.h"      

UIEventManager::UIEventManager() : event_system_(nullptr) {}


void UIEventManager::setEventSystem(EventSystem* event_system) { 
    event_system_ = event_system; 
}

EventSystem* UIEventManager::getEventSystem() const { 
    return event_system_; 
}

bool UIEventManager::hasEventSystem() const {
    return event_system_ != nullptr;
}


void UIEventManager::requestStatus() {
    current_event_ = UIEvent(UIEventType::STATUS_REQUESTED);
    logUIEvent("STATUS_REQUESTED", "Запрошен статус персонажей");
}

void UIEventManager::requestField() {
    current_event_ = UIEvent(UIEventType::FIELD_REQUESTED);
    logUIEvent("FIELD_REQUESTED", "Запрошено отображение карты");
}

void UIEventManager::requestSpells() {
    current_event_ = UIEvent(UIEventType::SPELLS_REQUESTED);
    logUIEvent("SPELLS_REQUESTED", "Запрошен список заклинаний");
}

void UIEventManager::requestHelp() {
    current_event_ = UIEvent(UIEventType::HELP_REQUESTED);
    logUIEvent("HELP_REQUESTED", "Запрошена справка по игре");
}

void UIEventManager::requestCommandHelp() {
    current_event_ = UIEvent(UIEventType::COMMAND_HELP_REQUESTED);
    logUIEvent("COMMAND_HELP_REQUESTED", "Запрошена справка по командам");
}

void UIEventManager::requestSpellSelection() {
    current_event_ = UIEvent(UIEventType::SPELL_SELECTION_REQUESTED);
    logUIEvent("SPELL_SELECTION_REQUESTED", "Запрошен выбор заклинания");
}

void UIEventManager::requestLibraryInteraction() {
    current_event_ = UIEvent(UIEventType::LIBRARY_INTERACTION);
    logUIEvent("LIBRARY_INTERACTION", "Запрошено взаимодействие с библиотекой");
}

void UIEventManager::requestLevelUpInterface() {
    current_event_ = UIEvent(UIEventType::LEVEL_UP_INTERFACE);
    logUIEvent("LEVEL_UP_INTERFACE", "Запрошен интерфейс прокачки");
}


UIEvent UIEventManager::consumeEvent() {
    UIEvent event = current_event_;
    current_event_ = UIEvent();  
    return event;
}

UIEvent UIEventManager::getCurrentEvent() const {
    return current_event_;
}

bool UIEventManager::hasEvent() const {
    return current_event_.type != UIEventType::NONE;
}

bool UIEventManager::hasEventOfType(UIEventType type) const {
    return current_event_.type == type;
}

void UIEventManager::clearEvent() {
    if (hasEvent()) {
        logUIEvent("CLEAR_EVENT", "UI событие очищено");
    }
    current_event_ = UIEvent();
}


bool UIEventManager::isStatusRequested() const {
    return current_event_.type == UIEventType::STATUS_REQUESTED;
}

bool UIEventManager::isFieldRequested() const {
    return current_event_.type == UIEventType::FIELD_REQUESTED;
}

bool UIEventManager::isSpellsRequested() const {
    return current_event_.type == UIEventType::SPELLS_REQUESTED;
}

bool UIEventManager::isSpellSelectionRequested() const {
    return current_event_.type == UIEventType::SPELL_SELECTION_REQUESTED;
}

bool UIEventManager::isLibraryInteractionRequested() const {
    return current_event_.type == UIEventType::LIBRARY_INTERACTION;
}

bool UIEventManager::isLevelUpInterfaceRequested() const {
    return current_event_.type == UIEventType::LEVEL_UP_INTERFACE;
}

bool UIEventManager::isHelpRequested() const {
    return current_event_.type == UIEventType::HELP_REQUESTED || 
           current_event_.type == UIEventType::COMMAND_HELP_REQUESTED;
}

void UIEventManager::logUIEvent(const std::string& event_type, const std::string& message) {
    if (event_system_) {
        GeneralEvent event("UI_EVENT", "[" + event_type + "] " + message);
        event_system_->notify(event);
    }
}