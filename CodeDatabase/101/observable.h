#ifndef OBSERVABLE_H
#define OBSERVABLE_H
#include "observer.h"
#include <vector>
#include <algorithm>

class Observable
{
private:
    std::vector<Observer*> obs;

public:
    void add_observer(Observer* ob);
    void del_observer(Observer* ob);
    void notify();
};

#endif // OBSERVABLE_H
