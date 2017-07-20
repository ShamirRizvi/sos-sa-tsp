//
//  tour.h
//  sos-sa-final
//
//  Created by Sebastian Coronado on 19/07/17.
//  Copyright © 2017 Sebastian Coronado. All rights reserved.
//


#ifndef tour_h
#define tour_h

#include "headers_tsp.h"
using namespace std;

typedef std::pair<double, double> pdd;

struct tour
{
    vector<pdd> __nodes; double __cost;
    
    tour()
    {
        __cost = 0;
    }
    
    tour(double cost)
    {
        __cost = cost;
    }
    
    tour(tour const &t)
    {
        for(
            int i = 0;
            i < t.__nodes.size();
            i++
            )
        {
            __nodes.push_back(t.__nodes[i]);
        }
        __cost = t.__cost;
    }
    
    void operator = (tour &t)
    {
        __nodes.clear();
        
        for(
            int i = 0;
            i < t.__nodes.size();
            i++
            )
        {
            __nodes.push_back(t.__nodes[i]);
        }
        __cost = t.__cost;
    }
};

#endif /* tour_h */
