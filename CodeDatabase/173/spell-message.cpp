#include "spell-message.hpp"

SpellMessage::SpellMessage(int index, Field *target)
    : m_Index(index), m_Target(target) {}

void SpellMessage::handle(Player *player) {
    if (player->canCastSpell(m_Index, m_Target)) {
        player->castSpell(m_Index, m_Target);
    }
}
