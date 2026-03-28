#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <map>
#include "enums.h"

struct Game_event{
    Event_type type;
    std::string source;
    std::string target;
    int value;
    int x;
    int y;
    std::string description;

    Game_event(Event_type t, const std::string& src, const std::string& tgt = "", int val = 0,
                int pos_x = -1, int pos_y = -1, const std::string& desc = "")
            : type(t), source(src), target(tgt), value(val), x(pos_x), y(pos_y), description(desc) {}
};

class Event_listener{
public:
    virtual ~Event_listener() = default;
    virtual void on_event(const Game_event& event) = 0;
};

class Event_system{
private:
    std::map<Event_type, std::vector<Event_listener*>> listeners;
    Event_system() = default;
    Event_system(const Event_type&) = delete;
    Event_system& operator=(const Event_type&) = delete;

public:
    static Event_system& get_instance(){
        static Event_system instance;
        return instance;
    }

    void subscribe(Event_type type, Event_listener* listener){
        listeners[type].push_back(listener);
    }

    void unsubscribe(Event_type type, Event_listener* listener){
        auto& list = listeners[type];
        auto it = std::find(list.begin(), list.end(), listener);
        if (it != list.end()){
            list.erase(it);
        }
    }

    void notify(const Game_event& event){
        auto it = listeners.find(event.type);
        if (it != listeners.end()){
            auto subscribers = it->second;
            for (auto listener : subscribers){
                listener->on_event(event);
            }
        }
    }
};

#endif