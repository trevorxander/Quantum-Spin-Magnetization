//
//  StatOperations.cpp
//  Quantum-Spin-Magnetization
//
//  Created by Trevor Xander on 2/24/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#include "StatOperations.hpp"


double StatOperations::error (std::vector<double> &predictedValues, double realValue){
    double sum = 0;
    for (double &prediction: predictedValues){
        sum += (prediction - realValue) / realValue;
    }
    return (1.0 / predictedValues.size()) * sum;
}

double StatOperations::stdDeviation (std::vector<double> &predictedValues, double realValue, double mean){
    double sum = 0;
    for (double &prediction: predictedValues){
        sum += pow(((prediction - realValue) / realValue) - mean, 2);
    }
    return sqrt((1.0 / predictedValues.size()) * sum);
}
double StatOperations::stdDeviation (std::vector<double> &predictedValues, double realValue){
    double sum = 0;
    double mean = StatOperations::error(predictedValues, realValue);
    for (double &prediction: predictedValues){
        sum += pow(((prediction - realValue) / realValue) - mean, 2);
    }
    return sqrt((1.0 / predictedValues.size()) * sum);
}

double StatOperations::average (std::vector<double> &values){
    double sum = 0;
    for (auto &num: values){
        sum += num;
    }
    return (1.0 / values.size()) * sum;
}
