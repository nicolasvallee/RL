#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int NB_STATES = 5;
const int NB_ACTIONS = 2;
const double gamma = 1;
double value[NB_STATES];
double P[NB_STATES][NB_STATES][NB_ACTIONS] = {
    {{1,0},{0,1},{0,0},{0,0},{0,0}},
    {{1,0},{0,0},{0,1},{0,0},{0,0}},
    {{0,0},{0,0},{0,0},{1,0},{0,0}},
    {{0,0},{0.2,0},{0.4,0},{0.4,0},{0,1}},
    {{0,0},{0,0},{0,0},{0,0},{0,0}}
    };;

double reward[NB_STATES][NB_ACTIONS] = {
    {-1,0},{-1,-2},{-2,0},{1,10},{0,0}
};
const int oo = 1e8;

const double epsilon = 0.1;

void valueIteration()
{
    bool valueHasChanged = true;
    while (valueHasChanged)
    {
        valueHasChanged = false;
        for(int state1 = 0; state1 < NB_STATES; state1++)
        {
            double max_value = value[state1];
            for(int action = 0; action < NB_ACTIONS; action++)
            {
                double sum = 0;
                sum = reward[state1][action];
                for(int state2 = 0; state2 < NB_STATES; state2++)
                {
                    sum += gamma * P[state1][state2][action] * value[state2]; 
                }
                max_value = max(max_value, sum);
            }
            if(max_value - value[state1] > epsilon)
                valueHasChanged = true;
            value[state1] = max_value;
        }
    }
}

void input()
{
    cout << "probabilities \n";

    for(int i = 0; i < NB_STATES; i++)
        for(int j = 0; j < NB_STATES; j++)
            for(int k = 0; k < NB_ACTIONS; k++)
            {
                cout << "state1 " << i << " state2 " << j << " action " << k << '\n';
                cin >> P[i][j][k];
            }
    
    cout << "rewards \n";

    for(int i = 0; i < NB_STATES; i++)
        for(int j = 0; j < NB_ACTIONS; j++)
        {
            cout << "state " << i << " action " << j << '\n';
            cin >> reward[i][j];
        }
}

int main()
{

    valueIteration();


    cout << "results \n";
    for(int i = 0; i < NB_STATES; i++)
        cout << "state" << i << " " << value[i] << '\n';

    return 0;
}