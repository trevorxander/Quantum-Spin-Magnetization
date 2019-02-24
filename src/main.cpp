//
//  main.cpp
//  QuantumMechanics
//
//  Created by Trevor Xander on 2/14/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#include <iostream>
#include <thread>

#include "SpinConfig.hpp"
#include "Metropolis.hpp"
#include "StatOperations.hpp"
#include "ParamOptimize.hpp"


int MAX_THREADS = 40;

int NO_OF_SPINS = 100;
double B = 0, C = -1;
double T = 1.9;

double MAX_ERROR = 0.02;
double MAX_STD_DEVIATION = 0.02;

int main(int argc, const char * argv[]) {
    Optimizer::setup(B, C, T, MAX_THREADS, NO_OF_SPINS);
    double theoreticalCp = Optimizer::getTheoretical();
    
    int flipConstant = 4;
    int metropolisPerThread = 150;
    
    std::cout << "No Of Threads: " << MAX_THREADS << "\n";
    std::cout << "Nf: "<< flipConstant << " No Of Flips: " << flipConstant * NO_OF_SPINS << "\n";
    std::cout << "Metropolis Thread: " << metropolisPerThread << "\n";
    
    
    
    auto iter = 0;
    
    Metropolis::Pair result = Metropolis::runMetropolis(NO_OF_SPINS, metropolisPerThread, MAX_THREADS, flipConstant, B, C, T);
    std::vector<double> magThreadResult = std::get<0>(result);
    std::vector<double> corThreadResult = std::get<1>(result);
    double corError = StatOperations::error(corThreadResult, theoreticalCp);
    double stdDev = StatOperations::stdDeviation(corThreadResult, theoreticalCp, corError);
    
    std::cout << "Avg Mag: " << StatOperations::average(magThreadResult) << "\n"
    << "Avg Cor: " << StatOperations::average(corThreadResult) << "\n";
    
    if (B == 0){
        std::cout << "Predicted: " << StatOperations::average(corThreadResult)
        << " Theoretical: " << theoreticalCp << "\n"
        << "Error: " << fabs(corError) << "\n"
        << "Std Deviation: " << stdDev << "\n\n";
        std::flush(std::cout);
        ++iter;
        if (!(stdDev < MAX_STD_DEVIATION && corError < MAX_STD_DEVIATION)){
            std::cout<< "High error or standar deviation! Run the program again  " << "\n";
        }
    }
    



    return 0;
}
