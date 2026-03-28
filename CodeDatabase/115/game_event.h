#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>

enum class EventType {
    Message,            
    CommandExecuted,    
    StateChanged,       
    
    DamageDealt,        
    DamageTaken,        
    UnitDied,          
    
    WeaponSwitched,     
    ScoreGained,        
    ScoreSpent,         
    Healed,             
    
    SpellCast,          
    SpellAcquired,      
    EnhancementAdded   
};

struct GameEvent {
    EventType type;
    std::string message;
    int val1;  
    int val2;  
};

#endif