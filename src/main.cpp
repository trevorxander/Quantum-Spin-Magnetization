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


int noOfSpins = 100;
int noOfThreads = 8;
int metropolisPerThread = 6;

double average (std::vector<double> &values){
    double sum = 0;
    for (auto &num: values){
        sum += num;
    }
    return (1.0 / values.size()) * sum;
}

int main(int argc, const char * argv[]) {
    std::vector<int> spins;
    for (int spin = 0; spin < noOfSpins; ++spin) spins.push_back(1);
    SpinConfig startingConfig(spins, 1, 0, 1.5);
    
    std::vector<std::thread> metropolisThreads;
    for (int threadCount = 0; threadCount < noOfThreads; ++threadCount){
        metropolisThreads.push_back(std::thread(Metropolis::metropolisThread, startingConfig, metropolisPerThread));
    }
    for (auto &thread: metropolisThreads) thread.join();
    auto results = Metropolis::getResults();
    
    std::vector<double> magThreadAvgs;
    std::vector<double> corThreadAvgs;
    for (auto &threadResult: results){
        magThreadAvgs.push_back(average(threadResult.magnetization));
        corThreadAvgs.push_back(average(threadResult.corelation));
    }
   
    std::cout << "Average magnetization: " << average(magThreadAvgs) << "\n";
    std::cout << "Average corelation: " << average(corThreadAvgs) << "\n";
  
    
    return 0;
}
