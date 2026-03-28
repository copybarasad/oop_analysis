#ifndef EVENTNOTIFIER_H
#define EVENTNOTIFIER_H

#include "IObserver.h"
#include "Event.h"
#include <vector>
#include <memory>

class Notifier{
private:
    std::vector<std::shared_ptr<IObserver>> observers;
    Notifier() = default;
    Notifier(const Notifier&) = delete;
    Notifier& operator=(const Notifier&) = delete;

public:
    static Notifier& getInstance();
    void subscribe(std::shared_ptr<IObserver> observer);
    void unsubscribe(std::shared_ptr<IObserver> observer);
    void notify(const Event& event);
};

#endif