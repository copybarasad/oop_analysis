#include "gameevent.h"

class PlayerMovedEvent : public GameEvent {
private:
    int fromX_, fromY_, toX_, toY_;

public:
    PlayerMovedEvent(int fromX, int fromY, int toX, int toY);

    std::string toString() const override;
};
