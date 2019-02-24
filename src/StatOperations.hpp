//
//  StatOperations.hpp
//  Quantum-Spin-Magnetization
//
//  Created by Trevor Xander on 2/24/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#ifndef StatOperations_hpp
#define StatOperations_hpp


#include <iostream>
#include <vector>
#include <cmath>


namespace StatOperations {
    double error (std::vector<double> &predictedValues, double realValue);
    double stdDeviation (std::vector<double> &predictedValues, double realValue, double mean);
    double stdDeviation (std::vector<double> &predictedValues, double realValue);
    double average (std::vector<double> &values);
}

#endif /* StatOperations_hpp */
