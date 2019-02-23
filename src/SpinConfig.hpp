//
//  Spin.hpp
//  QuantumMechanics
//
//  Created by Trevor Xander on 2/14/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#ifndef Spin_hpp
#define Spin_hpp

#include <vector>
#include <numeric>
#include <iostream>



class SpinConfig {
public:
    std::vector<int> configurations;
    double C = 0, B = 0, temperature = 0;
    SpinConfig(std::vector <int> &spinValues, double B, double C, double temp);
    SpinConfig(const SpinConfig& spin);
    double magentization ();
    double corelation (int corelationOrder = 1);
    double energy();
    unsigned long size () const;
    static double energyDiff (SpinConfig &config1, SpinConfig config2);
    static double energyDiff (SpinConfig &config, int changedSpin);
    
    int &operator [] (int index);
};

#endif /* Spin_hpp */
