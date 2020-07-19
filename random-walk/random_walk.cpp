//based on RL course david silver Lecture 4 - 55:00
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

double value[7] = {0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0};
double discount = 0.9;
double alpha = 0.1  ; // if you leave it to 1 it doesn't work !! 


struct TimeNode
{
    double value = 0;
    int last_update = 0;
};

struct scene
{
    int state;
    int reward; //imminent reward for moving to the next_state
    bool first_seen = false;
};


int get_next_state(int current_state)
{
    double p = ((double) rand() / RAND_MAX);
    int next_state = current_state;
    if(p < 0.5)
        next_state--;
    else 
        next_state++;
    return next_state;
}



void TD0()
{
    for(int episode = 0; episode < 10000000; episode++)
        {   

            int state = 3;
            while(state != 0 && state != 6)
            {
                int next_state = get_next_state(state);
                
                int reward = (next_state == 6);
                
                value[state] += alpha * (reward + value[next_state] - value[state]);
                state = next_state;
            }

        }
}



void TD_lambda_backward()
{

    double lambda = 0.1;
    for(int epi = 0; epi < 1000000; epi++)
    {
        int state = 3;
        TimeNode eligibility[7];
        int time = 0;
        while(state != 0 && state != 6)
        {
            TimeNode temp = eligibility[state];
            temp.value = temp.value * pow(discount * lambda, time - temp.last_update) + 1;
            temp.last_update = time;
            eligibility[state] = temp;

            int next_state = get_next_state(state);
            int reward = (next_state == 6);
            double TD_target = reward + discount * value[next_state];
            double TD_error = TD_target - value[state];

            value[state] = value[state] + alpha * TD_error * eligibility[state].value;
            
            state = next_state;
            time++;
        }

    }
}


void first_visit_MC()
{

    int N[7]; // number of times you have seen the node
    for(int i = 0; i < 7; i++) N[i] = 0;

    for(int epi = 0; epi < 10000; epi++)
    {
        bool seen[7];
        for(int i = 0; i < 7; i++) seen[i] = false;
        int state = 3;
        std::vector<scene> episode;
        while(state != 0 && state != 6)
        {
            scene s;
            s.state = state;    

            if(!seen[state])
            {
                N[state]++;
                s.first_seen = true;
                seen[state] = true;
            }

            int next_state = get_next_state(state);
            int reward = (next_state == 6);
            s.reward = reward;
            episode.push_back(s);
            state = next_state;
        }
        

        double total_reward = 0; // G

        for(int i = episode.size()-1; i >= 0; i--)
        {
            scene s = episode[i];
            total_reward = s.reward + discount * total_reward;
            if(s.first_seen)
            {
                value[s.state] += alpha * (total_reward - value[s.state]) / N[s.state];
            }
        }
   }
}


int main()
{
    srand(time(0));

    //TD0();
    //first_visit_MC();
    //TD_lambda_backward();
    for(int i = 0; i < 7; i++)
            cout << " " << value[i];
        cout << '\n';

    return 0;
}


// IDEA : do a user-interface with react.js that displays the graph value(state) and that lets you modify the alpha and discounts parameters 