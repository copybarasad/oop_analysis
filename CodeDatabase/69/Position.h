#pragma once

struct Position {
    int x = 0;
    int y = 0;
    
    Position() = default;
    Position(int x_, int y_);
    
    bool operator==(Position const& o) const;
};