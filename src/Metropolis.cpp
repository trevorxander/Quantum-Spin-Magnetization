//
//  Metropolis.cpp
//  QuantumMechanics
//
//  Created by Trevor Xander on 2/14/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#include "Metropolis.hpp"
#include <iostream>
double Metropolis::getRandom (double from, double to){
    static std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> dis(from, to);
    return dis(gen);
}

SpinConfig& Metropolis::chooseConfig (SpinConfig &config1, SpinConfig &config2){
    double energyDiff = SpinConfig::energyDiff(config1, config2);
    if (energyDiff < 0){
        return config2;//change config
    }
    else {
        double p = exp(-energyDiff/config1.temperature);
        if (Metropolis::getRandom(0, 1) < p) return config2;// change config
        else return config1; //old config
    }
}

bool cmp(double a, double b) {return fabs(a - b) < 0.000001;}

SpinConfig& Metropolis::chooseConfig (SpinConfig &config1, int changedSpin){
    double energyDiff = SpinConfig::energyDiff(config1, changedSpin);
    if (energyDiff < 0) config1[changedSpin] *= -1;
    else {
        double p;
        p = exp(-energyDiff/config1.temperature);
        double random = getRandom(0, 1);
        if (random < p) config1[changedSpin] *= -1;
    }
    return config1;
}

SpinConfig& Metropolis::minimize (SpinConfig &startConfig, int flipConstant){
    unsigned long noOfiterations = flipConstant * startConfig.size();
    
    for (int flips = 0; flips < noOfiterations; ++flips){
        int spinIndex = static_cast<int>(getRandom(0, startConfig.size()));
        startConfig = chooseConfig(startConfig, spinIndex);
        
//        auto copyConfig (startConfig);
//        copyConfig[spinIndex] *= -1;
//        startConfig = chooseConfig(startConfig, copyConfig);
        
    }
    return startConfig;
}
void Metropolis::metropolisThread (SpinConfig spinConfig, int noOfOptimizations, int flipConstant){
    Result threadResult;
    for (int optim = 0; optim < noOfOptimizations; ++optim){
        SpinConfig optimized = minimize(spinConfig, flipConstant);
        threadResult.magnetization.push_back(optimized.magentization());
        threadResult.corelation.push_back(optimized.corelation());
        
    }
    mutex.lock();
    resultVec.push_back(threadResult);
    mutex.unlock();
    
}
double Metropolis::average (std::vector<double> &values){
    double sum = 0;
    for (auto &num: values){
        sum += num;
    }
    return (1.0 / values.size()) * sum;
}
std::vector<Metropolis::Result> Metropolis::getResults (){
    struct AfterReturn {
        ~AfterReturn() {resultVec.clear();}
    };
    AfterReturn ret;
    return resultVec;
}
    
