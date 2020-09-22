#include <functional>
#include <iostream>
#include "cost.h"
#include "cmath"


using namespace std;

float goal_distance_cost(int goal_lane, int intended_lane, int final_lane, float distance_to_goal) {
    /*
    The cost increases with both the distance of intended lane from the goal
    and the distance of the final lane from the goal. The cost of being out of the 
    goal lane also becomes larger as vehicle approaches the goal.
    */
    
    //TODO: Replace cost = 0 with an appropriate cost function.
    int delta_d = abs(intended_lane - goal_lane) + abs(final_lane - goal_lane);
    float delta_s = distance_to_goal;
    float cost = 1.0 - exp(-(delta_d / delta_s));
    return cost;
}