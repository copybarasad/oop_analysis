#ifndef OOP_CELL_H_
#define OOP_CELL_H_

namespace rpg {
    enum class CellType {
        kNormal,
        kImpassable,
        kSlowing
    };

    class Cell final {
    public:
        Cell();

        explicit Cell(CellType type);

        [[nodiscard]] bool IsPassable() const;

        [[nodiscard]] bool IsSlowing() const;

        [[nodiscard]] CellType GetType() const;

        void SetType(CellType type);

    private:
        CellType type_;
    };
}

#endif
