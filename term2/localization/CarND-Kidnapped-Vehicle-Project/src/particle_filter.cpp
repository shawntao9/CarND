/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <limits>

#include "helper_functions.h"

using std::string;
using std::vector;

std::default_random_engine gen;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */
  if(is_initialized) return;

  // initialize numbers of particles
  num_particles = 100;

  //create normal distribution
  std::normal_distribution<double> dist_x(0, std[0]);
  std::normal_distribution<double> dist_y(0, std[1]);
  std::normal_distribution<double> dist_theta(0, std[2]);

  //generate particles with normal distribution (random Gaussian noise)
  for(int i = 0; i < num_particles; i++){
    Particle particle;
    particle.id = i;
    particle.x = x + dist_x(gen);
    particle.y = y + dist_y(gen);
    particle.theta = theta + dist_theta(gen);
    particle.weight = 1.0;

    particles.push_back(particle);
  }

  is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */

  //create normal distribution
  std::normal_distribution<double> dist_x(0, std_pos[0]);
  std::normal_distribution<double> dist_y(0, std_pos[1]);
  std::normal_distribution<double> dist_theta(0, std_pos[2]);

  for(int i = 0; i < num_particles; i++){
    //new state
    if(fabs(yaw_rate) < 0.00001){  
      particles[i].x += velocity * delta_t * cos(particles[i].theta);
      particles[i].y += velocity * delta_t * sin(particles[i].theta);
    } 
    else{
      particles[i].x += velocity / yaw_rate * (sin(particles[i].theta + yaw_rate*delta_t) - sin(particles[i].theta));
      particles[i].y += velocity / yaw_rate * (cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*delta_t));
      particles[i].theta += yaw_rate * delta_t;
    }

    //add noise
    particles[i].x += dist_x(gen);
    particles[i].y += dist_y(gen);
    particles[i].theta += dist_theta(gen);
  }
}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  unsigned int nObservations = observations.size();
  unsigned int nPredictions = predicted.size();

  for(unsigned int i = 0; i < nObservations; i++){
    LandmarkObs o = observations[i];
    //initialize min distance as a large number
    double minDistance = std::numeric_limits<double>::max();

    int mapId = -1;
    for(unsigned int j = 0; j < nPredictions; j++){
      LandmarkObs p = predicted[j];
      double distance = dist(o.x, o.y, p.x, p.y);

      if(distance < minDistance){
        minDistance = distance;
        mapId = p.id;
      }
    }

    observations[i].id = mapId;
  }

}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */

  for(int i = 0; i < num_particles; i++){
    double x = particles[i].x;
    double y = particles[i].y;
    double theta = particles[i].theta;

    //landmarks within particle's sensor range
    vector<LandmarkObs> inrangeLandmarks;

    for(unsigned int j = 0; j < map_landmarks.landmark_list.size(); j++){
      float lm_x = map_landmarks.landmark_list[j].x_f;
      float lm_y = map_landmarks.landmark_list[j].y_f;
      int lm_id = map_landmarks.landmark_list[j].id_i;

      if(fabs(lm_x - x) <= sensor_range && fabs(lm_y - y) <= sensor_range){
        inrangeLandmarks.push_back(LandmarkObs{lm_id, lm_x, lm_y});
      }
    }

    //transformation
    vector<LandmarkObs> transformedObservations;
    for(unsigned int j = 0; j < observations.size(); j++){
      double xx = cos(theta)*observations[j].x - sin(theta)*observations[j].y + x;
      double yy = sin(theta)*observations[j].x + cos(theta)*observations[j].y + y;
      transformedObservations.push_back(LandmarkObs{ observations[j].id, xx, yy });
    }

    //associate observations with landmarks
    dataAssociation(inrangeLandmarks, transformedObservations);

    //reinit weight
    particles[i].weight = 1.0;
    //update weight
    for(unsigned int j = 0; j < transformedObservations.size(); j++){
      double o_x = transformedObservations[j].x;
      double o_y = transformedObservations[j].y;

      int landmark_id = transformedObservations[j].id;

      double landmark_x, landmark_y;
      //get x, y coordinates of landmarks
      for(unsigned int k = 0; k < inrangeLandmarks.size(); k++){
        if(inrangeLandmarks[k].id == landmark_id){
          landmark_x = inrangeLandmarks[k].x;
          landmark_y = inrangeLandmarks[k].y;
          break;
        }
      }

      //calculate weight using multivariate Gaussian
      double gauss_norm = 1/(2 * M_PI * std_landmark[0] * std_landmark[1]);
      double exponent = pow(landmark_x - o_x, 2) / (2 * pow(std_landmark[0], 2))
                        + pow(landmark_y - o_y, 2) / (2 * pow(std_landmark[1], 2));
      double weight = gauss_norm * exp(-exponent);
      
      particles[i].weight *= weight;
    }
    
  }
}

void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */

  // Get weights and max weight.
  vector<double> weights;
  double maxWeight = std::numeric_limits<double>::min();
  for(int i = 0; i < num_particles; i++) {
    weights.push_back(particles[i].weight);
    if ( particles[i].weight > maxWeight ) {
      maxWeight = particles[i].weight;
    }
  }

  //create distribution
  std::uniform_real_distribution<double> unirealdist(0.0, maxWeight);
  std::uniform_int_distribution<int> uniintdist(0, num_particles - 1);

  int index = uniintdist(gen);
  double beta = 0.0;
  vector<Particle> resampledParticles;
  for(int i = 0; i < num_particles; i++){
    beta += unirealdist(gen) * 2.0;
    while(beta > weights[index]){
      beta -= weights[index];
      index = (index + 1) % num_particles;
    }
    
    resampledParticles.push_back(particles[index]);
  }

  particles = resampledParticles;
}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}