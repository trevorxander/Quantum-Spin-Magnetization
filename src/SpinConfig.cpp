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

SpinConfig::SpinConfig(int noOfSpins, double B, double C, double temp){
    for (int spinCount = 0; spinCount < noOfSpins; ++spinCount){
        if (C >= 0) configurations.push_back(1);
        else configurations.push_back(pow(-1, spinCount));
    }
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
    if (index < 0) index--;
    return configurations[index % SpinConfig::configurations.size()];
}
double SpinConfig::energy(){
    int spinSum = 0;
    for (int spinIndex = 0; spinIndex < configurations.size(); ++spinIndex){
        int spin1 = (*this)[spinIndex];
        int spin2 = (*this)[spinIndex + 1];
        spinSum += (B * spin1) + (C * spin1 * spin2);
    }
    return -spinSum;
}

//Slow version
double SpinConfig::energyDiff (SpinConfig &config1, SpinConfig config2){
    return config2.energy() - config1.energy();
}

double SpinConfig::energyDiff (SpinConfig &config, int changedSpinIndex){
    double oldEnergy = config.energy();
    
    double prevSpin = config[changedSpinIndex - 1];
    double changedSpin = config[changedSpinIndex];
    double nextSpin = config[changedSpinIndex + 1];

    double newEnergy = -oldEnergy;
    newEnergy -= (config.B * prevSpin) + (config.C * prevSpin * changedSpin);
    newEnergy += (config.B * prevSpin) + (config.C * prevSpin * -changedSpin);
    
    newEnergy -= (config.B * changedSpin) + (config.C * changedSpin * nextSpin);
    newEnergy += (config.B * -changedSpin) + (config.C * -changedSpin * nextSpin);
    
    newEnergy = -newEnergy;
    
    return newEnergy - oldEnergy;
}
unsigned long SpinConfig::size () const{
    return configurations.size();
}
