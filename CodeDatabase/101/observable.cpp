#include "observable.h"

void Observable::add_observer(Observer *ob)
{
    if(std::find(obs.begin(), obs.end(), ob) == obs.end()) obs.push_back(ob);
}

void Observable::del_observer(Observer* ob)
{
    auto it = std::find(obs.begin(), obs.end(), ob);
    if(it != obs.end())
        obs.erase(it);
}

void Observable::notify()
{


    if (obs.empty()) {
        return;
    }

    for(size_t i = 0; i < obs.size(); ++i) {
        obs[i]->updating();
    }
}


