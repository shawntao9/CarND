#include <iostream>
#include <algorithm>
#include <vector>

#include "helpers.h"
using namespace std;

//set standard deviation of control:
float control_stdev = 1.0f;

//set standard deviation of position:
float position_stdev = 1.0f;

//meters vehicle moves per time step
float movement_per_timestep = 1.0f;

//number of x positions on map
int map_size = 25;

//set observation standard deviation:
float observation_stdev = 1.0f;

//set distance max
float distance_max = map_size;

//initialize landmarks
std::vector<float> landmark_positions {3, 9, 14, 23}; //{5, 10, 12, 20};

// //define observations
// std::vector<float> observations {5.5, 13, 15};

//define observations vector, each inner vector represents a set of observations
//for a time step
std::vector<std::vector<float> > sensor_obs {{1,7,12,21}, {0,6,11,20}, {5,10,19}, {4,9,18},
                                {3,8,17}, {2,7,16}, {1,6,15}, {0,5,14}, {4,13},
                                {3,12},{2,11},{1,10},{0,9},{8},{7},{6},{5},{4},{3},{2},{1},{0}, 
                                {}, {}, {}};

std::vector<float> initialize_priors(int map_size, std::vector<float> landmark_positions,
                                     float position_stdev);

float motion_model(float pseudo_position, float movement, std::vector<float> priors,
                   int map_size, int control_stdev);

std::vector<float> pseudo_range_estimator(std::vector<float> landmark_positions, float pseudo_position);

//observation model: calculates likelihood prob term based on landmark proximity
float observation_model(std::vector<float> landmark_positions, std::vector<float> observations, 
                        std::vector<float> pseudo_ranges, float distance_max, 
                        float observation_stdev);



int main() {
    
    // initialize priors
    std::vector<float> priors = initialize_priors(map_size, landmark_positions,
                                                  position_stdev);

    //UNCOMMENT TO SEE THIS STEP OF THE FILTER
    /*std::cout << "-----------PRIORS INIT--------------" << endl;

    for (unsigned int p = 0; p < priors.size(); p++){
        std::cout << priors[p] << endl;
    }*/  
    
    //initialize posteriors
    std::vector<float> posteriors(map_size, 0.0);

    //specify time steps
    int time_steps = sensor_obs.size();

    //declare observation vector
    std::vector<float> observations;

    //cycle through time steps
    for (unsigned int t = 0; t < time_steps; t++){

        //UNCOMMENT TO SEE THIS STEP OF THE FILTER
        /*
        std::cout << "---------------TIME STEP---------------" << endl;
        std::cout << "t = " << t << endl;

        std::cout << "-----Motion----------OBS----------------PRODUCT--" << endl;
        */

        if (!sensor_obs[t].empty()) {
            observations = sensor_obs[t]; 
       } else {
            observations = {float(distance_max)};
       }

        //step through each pseudo position x (i)
        for (unsigned int i = 0; i < map_size; ++i) {
            float pseudo_position = float(i);

            //TODO: get the motion model probability for each x position
            float motion_prob = motion_model(pseudo_position, movement_per_timestep,
                                                         priors, map_size, control_stdev);

            //TODO: get pseudo ranges
            std::vector<float> pseudo_ranges = pseudo_range_estimator(landmark_positions, pseudo_position);

            //TODO: get observation probability
            float observation_prob = observation_model(landmark_positions, observations, 
                                                    pseudo_ranges, distance_max, observation_stdev);
            
            //TODO: calculate the ith posterior and pass to posteriors vector
            posteriors[i] = motion_prob * observation_prob;

            //UNCOMMENT TO SEE THIS STEP OF THE FILTER
            /*std::cout << motion_prob << "\t" << observation_prob << "\t" 
            << "\t"  << motion_prob * observation_prob << endl;
            */

            
        } 
        
        //UNCOMMENT TO SEE THIS STEP OF THE FILTER
        /*std::cout << "----------RAW---------------" << endl;

        for (unsigned int p = 0; p < posteriors.size(); p++) {
            std::cout << posteriors[p] << endl;
        }
        */

        
        //TODO: normalize posteriors (see helpers.h for a helper function)
        posteriors = Helpers::normalize_vector(posteriors);

        //print to stdout
        //std::cout << posteriors[t] <<  "\t" << priors[t] << endl;

        //UNCOMMENT TO SEE THIS STEP OF THE FILTER
        //std::cout << "----------NORMALIZED---------------" << endl;

        //TODO: update priors
        priors = posteriors;

        //UNCOMMENT TO SEE THIS STEP OF THE FILTER
        /*for (unsigned int p = 0; p < posteriors.size(); p++) {
            std::cout << posteriors[p] << endl;
        }
        */
     

    //print final posteriors vector to stdout
    for (unsigned int p = 0; p < posteriors.size(); p++) {
            std::cout << posteriors[p] << endl;  
        } 
    }

    return 0;  
};

//initialize priors assumimg vehicle at landmark +/- 1.0 meters position stdev
std::vector<float> initialize_priors(int map_size, std::vector<float> landmark_positions,
                                     float position_stdev) {
//initialize priors assumimg vehicle at landmark +/- 1.0 meters position stdev

    //set all priors to 0.0
    std::vector<float> priors(map_size, 0.0);

    //set each landmark positon +/1 to 1.0/9.0 (9 possible postions)
    float normalization_term = landmark_positions.size() * (position_stdev * 2 + 1);
    for (unsigned int i = 0; i < landmark_positions.size(); i++){
        int landmark_center = landmark_positions[i];
        priors[landmark_center] = 1.0f/normalization_term;
        priors[landmark_center - 1] = 1.0f/normalization_term;
        priors[landmark_center + 1] = 1.0f/normalization_term;
    }
    return priors;
}

//TODO, implement the motion model: calculates prob of being at an estimated position at time t
float motion_model(float pseudo_position, float movement, std::vector<float> priors,
                   int map_size, int control_stdev) {

    //initialize probability
    float position_prob = 0.0f;
    
    //YOUR CODE HERE
    //loop lover state space for all possible x
    for(unsigned int j = 0; j < map_size; j++){
        float next_pseudo_position = float(j);
        float distance_ij = next_pseudo_position - pseudo_position;

        //transition probability
        float transition_prob = Helpers::normpdf(distance_ij, movement, control_stdev);

        //estimate probability for the motion model, i.e. the prior
        position_prob += transition_prob * priors[j];
    }

    
    return position_prob;
}

//TODO: Complete pseudo range estimator function
std::vector<float> pseudo_range_estimator(std::vector<float> landmark_positions, float pseudo_position) {
    
    //define pseudo observation vector:
    std::vector<float> pseudo_ranges;
            
    //loop over number of landmarks and estimate pseudo ranges:
        //YOUR CODE HERE
    for (unsigned int i = 0; i < landmark_positions.size(); i++){
        float range_i = landmark_positions[i] - pseudo_position;
        if(range_i > 0.0){
            pseudo_ranges.push_back(range_i);
        }
    }

    //sort pseudo range vector:
        //YOUR CODE HERE
    sort(pseudo_ranges.begin(), pseudo_ranges.end());
    return pseudo_ranges;
}

//TODO Complete the observation model function
//calculates likelihood prob term based on landmark proximity
float observation_model(std::vector<float> landmark_positions, std::vector<float> observations, 
                        std::vector<float> pseudo_ranges, float distance_max,
                        float observation_stdev) {

    //YOUR CODE HERE
    //initialize observation probability
    float distance_prob = 1.0f;

    //loop over current observation vector
    for(unsigned int i = 0; i < observations.size(); i++){
        //define min distance
        float pseudo_range_min;

        //check if distance vector exists
        if(pseudo_ranges.size() > 0){
            //set min diatance
            pseudo_range_min = pseudo_ranges[0];
            //remove this entry from pseudo range vector
            pseudo_ranges.erase(pseudo_ranges.begin());
        }

        //if not exist, set min distance to a distance max
        else{
           pseudo_range_min =  distance_max;
        }

        //estimate the probability for observation model, this is likelihood
        distance_prob *= Helpers::normpdf(observations[i], pseudo_range_min, observation_stdev);

    }
    
    return distance_prob;
}