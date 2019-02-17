//
//  Metropolis.hpp
//  QuantumMechanics
//
//  Created by Trevor Xander on 2/14/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#ifndef Metropolis_hpp
#define Metropolis_hpp

#include <stdio.h>
#include <random>
#include <mutex>
#include "SpinConfig.hpp"

namespace Metropolis {
    
    static std::mutex mutex;
    static int flipConstant = 1000;
    static std::vector<double> magnetization;
    static std::vector<double> corelation;
    SpinConfig &minimize (SpinConfig &startConfig);
    void metropolisThread (SpinConfig spinConfig, int noOfOptimizations);
    double getRandom (double from, double to);
    SpinConfig& chooseConfig (SpinConfig &config1, SpinConfig &config2);
    SpinConfig& chooseConfig (SpinConfig &config1, int changedSpin);
    double average (std::vector<double> &values);
    double getAverageMagnetization ();
    double getAverageCorelation ();
};

#endif /* Metropolis_hpp */
