#include "move-message.hpp"

MoveMessage::MoveMessage(Direction dir) : m_Direction(dir) {}

void MoveMessage::handle(Player *player) {
    if (player->canMove()) {
        player->move(m_Direction);
    }
}
