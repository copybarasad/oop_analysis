class IPositionChecker {
public:
    virtual ~IPositionChecker() = default;

    virtual bool isPositionFree(int x, int y) const = 0;

    virtual bool isValidPosition(int x, int y) const = 0;
};
