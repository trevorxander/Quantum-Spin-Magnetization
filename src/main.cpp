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


int MAX_THREADS = 100;
int NO_OF_SPINS = 100;
double B = 10, C = -5;
double T = 1.9;

const double MAX_ERROR = 0.02;
const double MAX_STD_DEVIATION = 0.02;
int g_flipConstant = 4;
int g_metropolisPerThread = 150;
double g_theoreticalCp;


struct MinMax{
    double min =  std::numeric_limits<double>::infinity();
    double max = - std::numeric_limits<double>::infinity();
};

MinMax metropolisRange (double TLow, double THigh, unsigned long int noOfResults, std::ostream &out = std::cout){
    MinMax extremes;
    
    double Tval = TLow;
    double step = (THigh - TLow) / --noOfResults;
    out << "T,magnetization,corelation\n";
    while (Tval <= THigh) {
        Metropolis::Pair result = Metropolis::runMetropolis(NO_OF_SPINS, g_metropolisPerThread, MAX_THREADS, g_flipConstant, B, C, Tval);
        double magnetization = StatOperations::average(std::get<0>(result));
        double corelation = StatOperations::average(std::get<1>(result));
        out << Tval << "," << magnetization << "," << corelation << "\n";
        Tval += step;
        
        double *trackedValue = &magnetization;
        
        extremes.min = std::min(extremes.min, *trackedValue);
        extremes.max = std::max(extremes.max, *trackedValue);
    }
    return extremes;
}

void metropolis (std::ostream &out = std::cout){
    Metropolis::Pair result = Metropolis::runMetropolis(NO_OF_SPINS, g_metropolisPerThread, MAX_THREADS, g_flipConstant, B, C, T);
    std::vector<double> magThreadResult = std::get<0>(result);
    std::vector<double> corThreadResult = std::get<1>(result);
    double corError = StatOperations::error(corThreadResult, g_theoreticalCp);
    double stdDev = StatOperations::stdDeviation(corThreadResult, g_theoreticalCp, corError);
    
    out
    << "No Of Threads: " << MAX_THREADS << "\n"
    << "Nf: "<< g_flipConstant << ", No Of Flips: " << g_flipConstant * NO_OF_SPINS << "\n"
    << "Metropolis Per Thread: " << g_metropolisPerThread << "\n"
    
    << "\n\n";
    
    out
    << "B: " << B << ", C: " << C  << ", T: " << T << "\n"
    << "Avg Mag: " << StatOperations::average(magThreadResult) << "\n"
    << "Avg Cor: " << StatOperations::average(corThreadResult)
    << "\n\n";
    
    if (B == 0){
        out
        << "Predicted Corelation: " << StatOperations::average(corThreadResult)
        << ", Theoretical: " << g_theoreticalCp << "\n"
        << "Error: " << fabs(corError) << "\n"
        << "Standard Deviation: " << stdDev
        << "\n\n";
        if (!(stdDev < MAX_ERROR && corError < MAX_STD_DEVIATION)){
            out
            << "High error or standar deviation! Run the program again with different params."
            << "\n\n";
        }
    }
    std::flush(out);
    
}
std::vector<double> peakMagnetization (double Cmin, double Cmax, double step, std::ostream &out = std::cout){
    std::vector<double> results(3);
    double maxMagnetization = - std::numeric_limits<double>::infinity();
    std::ostream nullOut(nullptr);
    C = Cmax;
    std::cout << "B,C,max_magnetization\n";
    while(C > Cmin){
        B = -((2 * C) + 0.001);
        auto minmax = metropolisRange(0, 2, 20, nullOut);
        std::cout << B << "," << C << "," <<minmax.max << "\n";
        C -= step;
        if (minmax.max > maxMagnetization){
            maxMagnetization = minmax.max;
            results[0] = B;
            results[1] = C;
            results[2] = maxMagnetization;
        }
    }
    
    return results;
}

std::vector<double> valleyCorrelation (double Cmin, double Cmax, double step, std::ostream &out = std::cout){
    std::vector<double> results(3);
    double minCorelation = - std::numeric_limits<double>::infinity();
    std::ostream nullOut(nullptr);
    C = Cmax;
    std::cout << "B,C,min_corelation\n";
    while(C > Cmin){
        B = -((2 * C) - 0.001);
        auto minmax = metropolisRange(0, 2, 20, nullOut);
        std::cout << B << "," << C << "," <<minmax.max << "\n";
        C -= step;
        if (minmax.min < minCorelation){
            minCorelation = minmax.max;
            results[0] = B;
            results[1] = C;
            results[2] = minCorelation;
        }
    }
    
    return results;
}

int main(int argc, const char * argv[]) {
    Optimizer::setup(B, C, T, MAX_THREADS, NO_OF_SPINS);
    g_theoreticalCp = Optimizer::getTheoretical();
    
    metropolis();

    return 0;
}
