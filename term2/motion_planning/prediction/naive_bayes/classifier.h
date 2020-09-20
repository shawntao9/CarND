#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include "Eigen/Dense"

using namespace std;
using Eigen::ArrayXd;

class GNB {
public:

	vector<string> possible_labels = {"left","keep","right"};
	ArrayXd left_means;
	ArrayXd left_sds; // standard deviation
	double left_prior;
	
	ArrayXd keep_means;
	ArrayXd keep_sds;
	double keep_prior;
	
	ArrayXd right_means;
	ArrayXd right_sds;
	double right_prior;

	/**
  	* Constructor
  	*/
 	GNB();

	/**
 	* Destructor
 	*/
 	virtual ~GNB();

 	void train(vector<vector<double> > data, vector<string>  labels);

  	string predict(vector<double>);

};

#endif