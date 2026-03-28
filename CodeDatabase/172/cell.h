#pragma once

namespace game {
    // -------------- Type of cell ---------------
    enum class CellType { Wall, Floor, Slime };

    // ------------ Symbol in console ------------
    namespace Icons {
        inline constexpr char Enemy  = '?';
        inline constexpr char Player = '@';
        inline constexpr char Corpse = '%';

        inline constexpr char Floor  = '.';
        inline constexpr char Wall   = '#';
        inline constexpr char Slime  = '~';

        inline constexpr char EBuilding = 'B';
        inline constexpr char ETower = '&';

        inline constexpr char Trap   = '=';
    }

    // ------------------ Cell -------------------
    class Cell {
        private:
            CellType type_{CellType::Floor};
            char     icon_{Icons::Floor};

            bool isTrap_{false};
            int  trapDamage_{0};
        
        public:
            // == Constructor ==
            Cell(CellType type = CellType::Floor, char tile = Icons::Floor) : type_(type), icon_(tile) {}

            // == Getters ==
            char getIcon()      const { return icon_; }
            CellType getType()  const { return type_; }
            bool isWalkable()   const { return type_ != CellType::Wall; }
            bool isSlime()      const { return type_ == CellType::Slime; }

            // == Setters ==
            void setFloor() { type_ = CellType::Floor;   icon_ = Icons::Floor; }
            void setSlime() { type_ = CellType::Slime;   icon_ = Icons::Slime; }
            void setWall()  { type_ = CellType::Wall;    icon_ = Icons::Wall;  }

            void setTypeById(int id) { type_ = static_cast<CellType>(id); resetIcon(); }
            
            void placeCorpse()    { icon_ = Icons::Corpse;    }
            void placeEnemy()     { icon_ = Icons::Enemy;     }
            void placeEBuilding() { icon_ = Icons::EBuilding; }
            void placeETower()    { icon_ = Icons::ETower;    }
            void placePlayer()    { icon_ = Icons::Player;    }

            bool placeTrap(int dmg) {
                if (isTrap_) return false;
                if (type_ != CellType::Floor) return false;

                isTrap_ = true; trapDamage_ = dmg;
                return true;
            }

            bool hasTrap()       const { return isTrap_; }
            int  getTrapDamage() const { return trapDamage_; }

            void removeTrap() {
                isTrap_ = false;
                trapDamage_ = 0;
            }

            void resetIcon();
    };
}