#include "statusable.hpp"
#include "status.hpp"

Statusable::Statusable() {
    for (int i = 0; i < static_cast<int>(Status::EnumCount); ++i) {
        m_Statuses[static_cast<Status>(i)] = 0;
    }
}

bool Statusable::hasStatus(Status status) { return m_Statuses[status] > 0; }

void Statusable::addStatus(Status status) { m_Statuses[status]++; }
void Statusable::addStatus(Status status, int amount) {
    m_Statuses[status] += amount;
}

void Statusable::update() {
    for (const auto &entry : m_Statuses) {
        m_Statuses[entry.first] = std::max(0, entry.second - 1);
    }
}
