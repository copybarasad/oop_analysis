#ifndef OBSERVER_H
#define OBSERVER_H

template<typename T>
class Observer {
public:
    virtual void update(const T& data) = 0;
    virtual ~Observer() = default;
};

#endif