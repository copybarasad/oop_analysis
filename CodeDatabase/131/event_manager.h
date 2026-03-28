#ifndef OOP_EVENT_MANAGER_H_
#define OOP_EVENT_MANAGER_H_

#include "game_event.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace rpg {
    class EventManager final {
    public:
        EventManager() = default;
        ~EventManager() = default;

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager(EventManager&&) = default;
        EventManager& operator=(EventManager&&) = default;

        void Subscribe(IEventListener* listener) {
            if (listener && std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end()) {
                listeners_.push_back(listener);
            }
        }

        void Unsubscribe(IEventListener* listener) {
            listeners_.erase(
                std::remove(listeners_.begin(), listeners_.end(), listener),
                listeners_.end()
            );
        }

        void Notify(const GameEvent& event) const {
            for (auto* listener : listeners_) {
                if (listener) {
                    listener->OnEvent(event);
                }
            }
        }

        void Clear() {
            listeners_.clear();
        }

        [[nodiscard]] size_t GetListenerCount() const {
            return listeners_.size();
        }

    private:
        std::vector<IEventListener*> listeners_;
    };
}

#endif

