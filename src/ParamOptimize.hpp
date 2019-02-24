//
//  ParamOptimize.hpp
//  Quantum-Spin-Magnetization
//
//  Created by Trevor Xander on 2/24/19.
//  Copyright © 2019 Trevor Xander. All rights reserved.
//

#ifndef ParamOptimize_hpp
#define ParamOptimize_hpp

#include <iostream>
#include "Metropolis.hpp"
#include "StatOperations.hpp"

namespace Optimizer{
    static double B, C, T;
    static int maxThreads;
    static int noOfSpins;
    static std::ostream nullStream (nullptr);
    
    static double EPSILON = 0.01;
    static double THRRESHOLD = 0.9;
    
    template <typename INP_T, typename OUT_T>
    double binaryOptim (int min, int max, INP_T control,  OUT_T (*func)(int, INP_T), OUT_T acceptValue);
    double getTheoretical ();
    int getOptimalFlipsC (int metrpolisPerThread, double maxError, std::ostream &debug = nullStream);
    int getOptimalMetrPerThread (int flipConstant, double maxDeviation, std::ostream &debug = nullStream);
    void setup (double BVal, double CVal, double TVal, int maxThread, int spins);
    
    
}



#endif /* ParamOptimize_hpp */
