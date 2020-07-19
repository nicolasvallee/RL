#ifndef POSITION_H
#define POSITION_H

#include <iostream>

struct Position
{
    int x, y;
    
    friend std::ostream& operator<<(std::ostream &out, Position pos);
    
};

std::ostream& operator<<(std::ostream &out, Position pos)
{
        out << "x=" << pos.x << "  y=" << pos.y << '\n';
        return out;
}

#endif