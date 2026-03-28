#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <algorithm>
#include <vector>

template<typename T>
class Subject {
protected:
    std::vector<Observer<T>*> observers;
    virtual void notifyObservers() {
        T* self = static_cast<T*>(this);
        for (Observer<T>* observer : observers) {
            observer->update(*self);
        }
    }
    
public:
    virtual void registerObserver(Observer<T>* observer) {
        observers.push_back(observer);
    }
    
    virtual void removeObserver(Observer<T>* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    virtual ~Subject() = default;
};

#endif