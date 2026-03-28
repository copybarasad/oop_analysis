#ifndef STATUSABLE_HPP
#define STATUSABLE_HPP

#include <map>

#include "status.hpp"

class Statusable {
  public:
    Statusable();
    void addStatus(Status status);
    void addStatus(Status status, int amount);
    bool hasStatus(Status status);

  protected:
    std::map<Status, int> m_Statuses;
    void update();
};

#endif /* STATUSABLE_HPP */
