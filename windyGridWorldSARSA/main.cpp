//TO DO
//The algo does not converge.. Even after 100000 episodes on 
//basic 5*5 grids it takes some waky routes...
//Not sure how to solve this.
//Use std::map instead of messing with IDs

// !!! The Q values don't seem to be more negative the 
// further away you are, while Q is supposed to be the expected return !!! 

#include <iostream>
#include <random>
#include "Action.hpp"
#include <map>

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;

const int NB_STATES = MAP_WIDTH * MAP_HEIGHT;
const int NB_ACTIONS = 8;
double alpha = 0.7;

const double discount = 1;
const double oo = 1e8;

double epsilon = 0.1;

double policy[NB_STATES][NB_ACTIONS]; //epsilon-greedy;
double Q[NB_STATES][NB_ACTIONS];


Position kingMoves[8] = 
{
    {-1,-1}, {0,-1}, {1, -1}, {1, 0}, {1, 1}, {0,1}, {-1,1}, {-1,0}
};


class State
{
    int m_id;
    Position m_pos;

    void m_setID()
    {
        m_id = m_pos.y * MAP_WIDTH + m_pos.x;
    }


public:

    State(int x, int y)
    :m_pos{Position{x,y}}
    {
        m_setID();
    }
    
    bool operator==(State s)
    {
        return m_id == s.getID();
    }
    bool operator!=(State s)
    {
        return m_id != s.getID();
    }

    int getID(){return m_id;}
    Position getPos(){return m_pos;}

    std::vector<Action> getPossibleActions()
    {
        std::vector<Action> v;

        for(int d = 0; d < 8; d++)
        {
            Position dir = kingMoves[d];
            Position newPos = {m_pos.x + dir.x, m_pos.y + dir.y};
            if(newPos.x >= 0 && newPos.x < MAP_WIDTH
            && newPos.y >= 0 && newPos.y < MAP_HEIGHT)
            {
                v.push_back(Action{dir.x, dir.y});
            }
        }
        return v;
    }

    
    Action getPolicyAction()
    {
        double p = (double)rand() / RAND_MAX;
        //std::cout << p << '\n';
        double sum = 0;

        std::vector<Action> actions = getPossibleActions();
        Action bestAction = actions[rand() % (actions.size())]; //random possible action

        for(Action a : actions)
        {
            sum += policy[m_id][a.getID()];
            if(p < sum)
            {
                bestAction = a;
                break;
            }
            
        }

        return bestAction;
    }

    Action getBestDeterministicAction() // to be used when printing the path !!!
    {
        std::vector<Action> actions = getPossibleActions();
        Action bestAction = actions[0];
        for(Action a : actions)
            if(Q[m_id][a.getID()] > Q[m_id][bestAction.getID()])
                bestAction = a;

        return bestAction;
    }

    State getNextState(Action a)
    {
        //action is assumed to be doable (bounds have been checked)

        Position p = m_pos;
        Position d = a.getDir();
        return State{p.x + d.x, p.y + d.y};
    }

};

const State startState{0,0};
const State endState{MAP_WIDTH-1,0};


double getReward(State s, Action a)
{
    if(s.getNextState(a) == endState)
        return 0;
    else
        return -1;
    
    
}

void updatePolicy(State s)
{
    std::vector<Action> actions = s.getPossibleActions();
    Action bestAction{0,0};
    double maxQ = -oo;

    for(Action a : actions)
    {
        double q = Q[s.getID()][a.getID()];
        if(q > maxQ)
        {
            maxQ = q;
            bestAction = a;
        }
    }
    int m = actions.size();
    for(Action a : actions)
    {
        if(a == bestAction)
            policy[s.getID()][a.getID()] = epsilon / (double)m  + 1 - epsilon;
        else
        {
            policy[s.getID()][a.getID()] = epsilon / (double)m;
        }
        
    }
}

void initialize()  
{
    //initialize with a uniform policy over the possible actions
    for(int i = 0; i < MAP_HEIGHT; i++)
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            State s{j,i};
            std::vector<Action> v = s.getPossibleActions();
            int m = v.size();
            for(Action a : v)
            {
                policy[s.getID()][a.getID()] = 1.0 / m;
            }
        }
}

void printPath();
void SARSA(int nb_episodes)
{
    initialize();

    for(int epi = 0; epi < nb_episodes; epi++)
    {

        epsilon = (double)nb_episodes / (19 * epi + nb_episodes);
        if(epi % 10000 == 0)
        std::cout << "EPISODE " << epi << '\n';

        State state = startState;
        Action action = state.getPolicyAction();
        int time = 0;
        do
        {  
            time++;
            alpha = 1.0 / time;

            //std::cout << state.getPos();
            double reward = getReward(state, action);

            State nextS = state.getNextState(action);
            Action nextA = nextS.getPolicyAction();

            int s1 = state.getID();
            int a1 = action.getID(); 
            int s2 = nextS.getID();
            int a2 = nextA.getID();
            
            double TD_target = reward + discount * Q[s2][a2];
            double TD_error = TD_target - Q[s1][a1];
            Q[s1][a1] += alpha * TD_error;

            updatePolicy(state);

            state = nextS;
            action = nextA;

        }
        while (state != endState);

      //  printPath();
        
    }
}

void printPath()
{
    
    std::string world[MAP_HEIGHT][MAP_WIDTH];
    State state = startState;

    for(int i = 0; i < MAP_HEIGHT; i++)
        for(int j = 0; j < MAP_WIDTH; j++)
            world[i][j] = ".";
    int time = 0;
    int end = 100; // give it some time in case of imperfection (randomness in policy)
    Position p = state.getPos();
    world[p.y][p.x] = "0";
    while(time <= end && state != endState)
    {
        time++;
        Action action = state.getBestDeterministicAction();
        state = state.getNextState(action);
        Position p = state.getPos();
        world[p.y][p.x] += "-" + std::to_string(time);
    }

    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
            std::cout << world[i][j] << "  ";
        
        std::cout << "\n\n";
    }
        
    
}

int main()
{
    srand(time(0));
    SARSA(10000);

    printPath();

    return 0;
}