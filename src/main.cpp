//
//  main.cpp
//  QuantumMechanics
//
//  Created by Trevor Xander on 2/14/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#include <iostream>
#include <thread>
#include <future>
#include "SpinConfig.hpp"
#include "Metropolis.hpp"



int MAX_THREADS = 1000;

int NO_OF_SPINS = 100;
double g_B = 0, g_C = -1;
double g_T =  1.5;
double THEORETICAL_CORELATION = (exp(g_C/g_T) - exp(-g_C / g_T))
                                / (exp(g_C/g_T) + exp(-g_C / g_T));

double MAX_ERROR = 0.02;
double MAX_STD_DEVIATION = 0.02;


double averageVec (std::vector<double> &values){
    double sum = 0;
    for (auto &num: values){
        sum += num;
    }
    return (1.0 / values.size()) * sum;
}

using Pair = std::tuple<std::vector<double>,std::vector<double>>;
Pair runMetropolis (int maxIterations, int flipConstant, double B = g_B, double C = g_C, double T = g_T) {
    std::vector<int> spins;
    for (int spin = 0; spin < NO_OF_SPINS; ++spin) spins.push_back(1);
    SpinConfig startingConfig(spins, B, C, T);
    
    std::vector<std::thread> metropolisThreads;
    for (int threadCount = 0; threadCount < MAX_THREADS; ++threadCount){
        metropolisThreads.push_back(std::thread(Metropolis::metropolisThread, startingConfig, maxIterations, flipConstant));
    }
    for (auto &thread: metropolisThreads) thread.join();
    auto results  = Metropolis::getResults();
    
    std::vector<double> magThreadAvgs;
    std::vector<double> corThreadAvgs;
    corThreadAvgs.clear();
    for (auto &threadResult: results){
        magThreadAvgs.push_back(averageVec(threadResult.magnetization));
        corThreadAvgs.push_back(averageVec(threadResult.corelation));
    }

    return std::make_tuple(magThreadAvgs, corThreadAvgs);
}

double error (std::vector<double> &predictedValues, double realValue){
    double sum = 0;
    for (double &prediction: predictedValues){
       sum += (prediction - realValue) / realValue;
    }
    return (1.0 / predictedValues.size()) * sum;
}

double stdDeviation (std::vector<double> &predictedValues, double realValue, double mean){
    double sum = 0;
    for (double &prediction: predictedValues){
        sum += pow(((prediction - realValue) / realValue) - mean, 2);
    }
    return sqrt((1.0 / predictedValues.size()) * sum);
}

double EPSILON = 0.01;
double THRRESHOLD = 0.9;

template <typename INP_T, typename OUT_T>
double binaryOptim (int min, int max, INP_T control,  OUT_T (*func)(int, INP_T), OUT_T acceptValue){
    acceptValue *= THRRESHOLD;
    INP_T mid = (min + max) / 2;
    if (mid == min) return max;
    OUT_T value = (*func) (mid, control);
    if (acceptValue - value < EPSILON) return max;
    else if (value < acceptValue) return binaryOptim(min, mid, control, func, acceptValue);
    else return binaryOptim(mid, max, control, func, acceptValue);
}

int getOptimalFlipsC (std::ostream &debug = std::cout){
    int metrpolisPerThread = 10;
    int flipConstant = 1;
    auto *err = +[](int flipC, int iter){
        Pair result = runMetropolis(iter, flipC);
        std::vector<double> corThreadResult = std::get<1>(result);
        double corError = error(corThreadResult, THEORETICAL_CORELATION);
        return fabs(corError);
    };
    
    while (true) {
        
        double corError = (*err) (flipConstant, metrpolisPerThread);
        debug << "FlipConstant: " << flipConstant << "\n"
              <<"error: " << corError <<" Product: " << fabs(corError) * sqrt(flipConstant) << "\n\n";
        if (corError < MAX_ERROR ) break;
        flipConstant *= 2;
    }
    flipConstant = binaryOptim(flipConstant / 2, flipConstant, metrpolisPerThread, err, MAX_ERROR);
    return flipConstant;
}


int getOptimalMetrPerThread (std::ostream &debug = std::cout){
    int flipConstant = 10;
    int metropolisPerThread = 1;
    
    auto *var = +[](int iter, int flipC){
        Pair result = runMetropolis(iter, flipC);
        std::vector<double> corThreadResult = std::get<1>(result);
        double corError = error(corThreadResult, THEORETICAL_CORELATION);
        double stdDev = stdDeviation(corThreadResult, THEORETICAL_CORELATION, corError);
        return stdDev;
    };
    
    while (true) {
        double stdDev = (*var)(metropolisPerThread, flipConstant);
        debug << "Metropolis per thread: " << metropolisPerThread << "\n"
              << "standard Deviation: " << stdDev <<" Product: " << stdDev * sqrt(metropolisPerThread) << "\n\n";
        
        if (stdDev < MAX_STD_DEVIATION) break;
        metropolisPerThread *= 2;
    }
    metropolisPerThread = binaryOptim(metropolisPerThread / 2, metropolisPerThread, flipConstant, var, MAX_STD_DEVIATION);
    return metropolisPerThread;
}


int main(int argc, const char * argv[]) {

    std::ostream debugOut (nullptr);
    int flipConstant = 4;
    int metropolisPerThread = 100;
    
    std::cout << "No Of Threads: " << MAX_THREADS << "\n";
    std::cout << "Nf: "<< flipConstant << " No Of Flips: " << flipConstant * NO_OF_SPINS << "\n";
    std::cout << "Metropolis Thread: " << metropolisPerThread << "\n";
    
    auto iter = 0;
    while (true) {
        Pair result = runMetropolis(metropolisPerThread, flipConstant);
        std::vector<double> magThreadResult = std::get<0>(result);
        std::vector<double> corThreadResult = std::get<1>(result);
        double corError = error(corThreadResult, THEORETICAL_CORELATION);
        double stdDev = stdDeviation(corThreadResult, THEORETICAL_CORELATION, corError);
        
        std::cout << "Avg Mag: " << averageVec(magThreadResult) << "\n"
        << "Avg Cor: " << averageVec(corThreadResult) << "\n";
        
        if (g_B == 0){
            std::cout << "Predicted: " << averageVec(corThreadResult)
            << " Theoretical: " << THEORETICAL_CORELATION << "\n"
            << "Error: " << fabs(corError) << "\n"
            << "Std Deviation: " << stdDev << "\n\n";
            std::flush(std::cout);
            ++iter;
            if (!(stdDev < MAX_STD_DEVIATION && corError < MAX_STD_DEVIATION)){
                std::cout<< iter << "\n";
                break;
            }
        }
        
        break;
    }


    return 0;
}
