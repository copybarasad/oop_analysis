#ifndef IEVENTSUBJECT_H
#define IEVENTSUBJECT_H

class IObserver;

class IEventSubject {
public:
    virtual ~IEventSubject() = default;
    virtual void subscribe(IObserver* observer) = 0;
    virtual void unsubscribe(IObserver* observer) = 0;
};

#endif

