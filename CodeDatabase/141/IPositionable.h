class IPositionable {
public:
    virtual ~IPositionable() = default;

    virtual void getPosition(int &x, int &y) const = 0;
};
