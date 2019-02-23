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
    struct Result {
        std::vector<double> magnetization;
        std::vector<double> corelation;
    };
    static std::vector<Result> resultVec;
    static std::mutex mutex;
    SpinConfig &minimize (SpinConfig &startConfig, int flipConstant);
    void metropolisThread (SpinConfig spinConfig, int noOfOptimizations, int flipConstant);
    double getRandom (double from, double to);
    SpinConfig& chooseConfig (SpinConfig &config1, SpinConfig &config2);
    SpinConfig& chooseConfig (SpinConfig &config1, int changedSpin);
    double average (std::vector<double> &values);
    std::vector<Result> &getResults();
};

#endif /* Metropolis_hpp */
