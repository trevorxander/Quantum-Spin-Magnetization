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
int noOfThreads = 50;
int metropolisPerThread = 50;

int main(int argc, const char * argv[]) {
    std::vector<int> spins;
    for (int spin = 0; spin < noOfSpins; ++spin) spins.push_back(1);
    SpinConfig startingConfig(spins, 1, 0, 1.5);
    
    std::vector<std::thread> metropolisThreads;
    for (int threadCount = 0; threadCount < noOfThreads; ++threadCount){
        metropolisThreads.push_back(std::thread(Metropolis::metropolisThread, startingConfig, metropolisPerThread));
    }
    for (auto &thread: metropolisThreads) thread.join();
    std::cout<< "Average magnetization: "<<Metropolis::getAverageMagnetization() << "\n"
                << "Average pair corelation: " <<Metropolis::getAverageCorelation() << "\n";
    
    return 0;
}
