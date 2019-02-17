//
//  Spin.cpp
//  QuantumMechanics
//
//  Created by Trevor Xander on 2/14/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#include "SpinConfig.hpp"

    
SpinConfig::SpinConfig(std::vector <int> &spinValues, double B, double C, double temp){
    configurations = spinValues;
    this->B = B;
    this->C = C;
    temperature = temp;
}
SpinConfig::SpinConfig(const SpinConfig& spin){
    configurations = spin.configurations;
    this->B = spin.B;
    this->C = spin.C;
    temperature = spin.temperature;
}

double SpinConfig::corelation (int corelationOrder){
    int spinSum = 0;
    for (int spinIndex = 0; spinIndex < configurations.size(); ++spinIndex){
        int spin1 = (*this)[spinIndex];
        int spin2 = (*this)[spinIndex + corelationOrder];
        spinSum += spin1 * spin2;
    }
    return (1.0 / SpinConfig::configurations.size()) * spinSum;
}

double SpinConfig::magentization (){
    int spinSum = 0;
    for (int spinIndex = 0; spinIndex < configurations.size(); ++spinIndex){
        int spin1 = (*this)[spinIndex];
        spinSum += spin1;
    }
    return (1.0 / SpinConfig::configurations.size()) * spinSum;
}
int &SpinConfig::operator [] (int index) {
    int validIndex;
    if (index < 0) index--;
    validIndex = index % SpinConfig::configurations.size();
    return configurations[validIndex];
}
double SpinConfig::energy(){
    int spinSum = 0;
    for (int spinIndex = 0; spinIndex < configurations.size(); ++spinIndex){
        int spin1 = (*this)[spinIndex];
        int spin2 = (*this)[spinIndex + 1];
        spinSum += B * spin1 + (C * spin1 * spin2) ;
    }
    return -spinSum;
}

double SpinConfig::energyDiff (SpinConfig &config1, SpinConfig config2){
    return config2.energy() - config1.energy();
}

double SpinConfig::energyDiff (SpinConfig &config, int changedSpin){
    int oldEnergy = config.energy();
    int spin1 = config[changedSpin];
    int spin2 = config[changedSpin + 1];
    double newEnergy = oldEnergy - (config.B * spin1 + (config.C * spin1 * spin2))
                                 + (config.B * -spin1 + (config.C * -spin1 * -spin2));
    return oldEnergy - newEnergy;
}
int SpinConfig::size () const{
    return configurations.size();
}
