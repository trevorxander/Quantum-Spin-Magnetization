//
//  ParamOptimize.cpp
//  Quantum-Spin-Magnetization
//
//  Created by Trevor Xander on 2/24/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#include "ParamOptimize.hpp"

template <typename INP_T, typename OUT_T>
double Optimizer::binaryOptim (int min, int max, INP_T control,  OUT_T (*func)(int, INP_T), OUT_T acceptValue){
    acceptValue *= THRRESHOLD;
    int mid = (min + max) / 2;
    if (mid == min) return max;
    OUT_T value = (*func) (mid, control);
    if (acceptValue - value < EPSILON) return max;
    else if (value < acceptValue) return binaryOptim(min, mid, control, func, acceptValue);
    else return binaryOptim(mid, max, control, func, acceptValue);
}


double Optimizer::getTheoretical (){
    return (exp(C/T) - exp(-C / T)) / (exp(C/T) + exp(-C / T));
}
int Optimizer::getOptimalFlipsC (int metrpolisPerThread, double maxError, std::ostream &debug){
    int flipConstant = 1;
    auto *err = +[](int flipC, int iter){
        Metropolis::Pair result = Metropolis::runMetropolis(noOfSpins, iter, maxThreads, flipC, B, C, T);
        std::vector<double> corThreadResult = std::get<1>(result);
        double corError = StatOperations::error(corThreadResult, getTheoretical());
        return fabs(corError);
    };
    debug << "flip_constant,error,product\n";
    while (true) {
        double corError = (*err) (flipConstant, metrpolisPerThread);
        debug << flipConstant << "," << corError << "," << fabs(corError) * sqrt(flipConstant) << "\n";
        if (corError < maxError ) break;
        flipConstant *= 2;
    }
    flipConstant = binaryOptim(flipConstant / 2, flipConstant, metrpolisPerThread, err, maxError);
    return flipConstant;
}


int Optimizer::getOptimalMetrPerThread (int flipConstant, double maxDeviation, std::ostream &debug){
    int metropolisPerThread = 1;
    
    auto *var = +[](int iter, int flipC){
        Metropolis::Pair result = Metropolis::runMetropolis(noOfSpins, iter, maxThreads, flipC, B, C, T);
        std::vector<double> corThreadResult = std::get<1>(result);
        double corError = StatOperations::error(corThreadResult, getTheoretical());
        double stdDev = StatOperations::stdDeviation(corThreadResult, getTheoretical(), corError);
        return stdDev;
    };
    
    debug << "metropolis_iter,std_deviation,product\n";
    while (true) {
        double stdDev = (*var)(metropolisPerThread, flipConstant);
        debug << metropolisPerThread << "," << stdDev << "," << stdDev * sqrt(metropolisPerThread) << "\n";
        
        if (stdDev < maxDeviation) break;
        metropolisPerThread *= 2;
    }
    metropolisPerThread = binaryOptim(metropolisPerThread / 2, metropolisPerThread, flipConstant, var, maxDeviation);
    return metropolisPerThread;
}
void Optimizer::setup (double BVal, double CVal, double TVal, int maxThread, int spins){
    B = BVal;
    C = CVal;
    T = TVal;
    maxThreads = maxThread;
    noOfSpins = spins;
}
