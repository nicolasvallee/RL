#ifndef ACTION_H
#define ACTION_H
#include "Position.hpp"

class Action
{
    int m_id;
    Position m_dir;

    void m_setID()
    {
        int ID[3][3] = 
        {
            {0, 1, 2},
            {7, -42, 3},
            {6, 5, 4}
        };
        m_id = ID[m_dir.y+1][m_dir.x+1];
        //king moves clockwise starting from top left corner
    }

public:

    Action(int dx, int dy)
    :m_dir{Position{dx,dy}}
    {
        m_setID();
    }

    Action(Position p)
    :Action{p.x,p.y}
    {

    }

    bool operator==(Action a)
    {
        return m_id == a.getID();
    }
    int getID(){return m_id;}

    Position getDir(){return m_dir;}

};

Action errorAction{0,0};

#endif 