class IGameContext {
public:
    virtual ~IGameContext() = default;

    virtual void towerAttackPlayer(int towerX, int towerY, int damage) = 0;

    virtual bool isPlayerInRange(int centerX, int centerY, int radius) const = 0;
};
