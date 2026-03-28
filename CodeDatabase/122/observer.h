#ifndef OBSERVER_H
#define OBSERVER_H

class Observer
{
public:
    virtual ~Observer() = default;
    virtual bool handleEvent(int x, int y) = 0;
    virtual bool isDestroyed() const = 0;
};

#endif // OBSERVER_H
