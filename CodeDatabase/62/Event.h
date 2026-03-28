#pragma once
#include <string>
#include <vector>

enum class EventType {
    MovePlayer,
    ChangePlayerAttackMode,
    HandleMouseClick,
    SaveGame,
    LoadGame,
    UseSpellFromHand,      // Применение заклинания из руки (1-5)
    BuyShopItem,          // Покупка предмета из магазина (6,7,8,9,0,-)
    AttackDirection,      // Атака в направлении (стрелки)
    SelectButton          // Выбор кнопки в меню (z,x,c,v)
};

class Event {
private:
    EventType type_;
    std::vector<float> floatArgs_;
    std::vector<int> intArgs_;
    std::vector<std::string> stringArgs_;

public:
    Event(EventType type);
    
    EventType getType() const { return type_; }
    
    void addFloatArg(float arg);
    void addIntArg(int arg);
    void addStringArg(const std::string& arg);
    
    float getFloatArg(size_t index) const;
    int getIntArg(size_t index) const;
    const std::string& getStringArg(size_t index) const;
    
    size_t getFloatArgsCount() const { return floatArgs_.size(); }
    size_t getIntArgsCount() const { return intArgs_.size(); }
    size_t getStringArgsCount() const { return stringArgs_.size(); }
};

