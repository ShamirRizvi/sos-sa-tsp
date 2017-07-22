//
//  main.cpp
//  sos-sa-final
    //
//  Created by Sebastian Coronado on 15/07/17.
//  Copyright © 2017 Sebastian Coronado. All rights reserved.
//

#include "headers_tsp.h"

using namespace std;

double cost = 0;
tour __initial_tour;

pdd swap_elements (int node_a, int node_b, vector<pdd> &nodes);
vector <tour> ecosystem;
vector <tour> parasites;
tour best_tour;

int generate_ecosystem(int ecosystem_size, tour &seed_tour)
{
    int best_organism = 0;
    double best_cost = seed_tour.__cost;
    srand ((unsigned)time(NULL));

    int swap_i = 0, swap_j = 0;
    for (int i = 0; i < ecosystem_size; i++)
    {
        do
        {
            swap_i = rand() % seed_tour.__nodes.size();
            swap_j = rand() % seed_tour.__nodes.size();
        } while (swap_i == swap_j);
            
        tour swap_copy(seed_tour);
        pdd result = swap_elements(swap_i, swap_j, swap_copy.__nodes);
        
        swap_copy.__cost = result.second;
        
        if(swap_copy.__cost < best_cost)
        {
            best_cost = swap_copy.__cost;
            best_organism = i;
        }
        ecosystem.push_back(swap_copy);
    }
    
    return best_organism;
}

void apply_mutualism(double &best_organism, int v_i, int v_j)
{
    int k1, k2;
    double randio, mutual, x1, x2;
    
    // generate random between 0 and 1
    randio = double(rand() / (double)RAND_MAX);
    
    // generate random k1 and k2
    (randio <= .4) ? k1 = 1: k2 = 2;
    
    // search mutual vector
    mutual = (ecosystem[v_i].__cost + ecosystem[v_j].__cost)/2;
    
    //Xi′=Xi+rand(0,1)×(X −Mutual ×K)
    x1 = (x1+randio) * (best_organism - mutual * k1) ;//(ecosystem[v_i].__cost + mutual *randio);
    
    //Xj′ =Xj +rand(0,1)×(Xbest −Mutualvect ×K2)
    x2 = (x2+randio) * (best_organism - mutual * k2) ;;//(ecosystem[v_j].__cost + mutual *randio);
    
    //updates
    (x1 < ecosystem[v_i].__cost)? ecosystem[v_i].__cost = x1:0;
    (x2 < ecosystem[v_j].__cost)? ecosystem[v_j].__cost = x2:0;
    
}

void apply_comensalism(double best_organism, int v_i, int v_j)
{
    double randio, x1;
    
    // generate random between 0 and 1
    randio = (rand() % 2) + -1;
    
    //Xi′=Xi+rand(−1,1)× Xbest−Xj if f X′i > f(Xi)
    x1 = (ecosystem[v_i].__cost + randio) * (best_organism -ecosystem[v_j].__cost);
    //update
    (x1 < ecosystem[v_i].__cost)? ecosystem[v_i].__cost = x1:0;
}

void apply_paristism(double best_organism, int v_i, int v_j)
{
    if (ecosystem[v_i].__cost < ecosystem[v_i].__cost)
        ecosystem[v_i] = ecosystem[v_j];
}

bool update_best_solution(pair<int, double> &best_organism)
{
    bool result = false;
    for (int i = 0; i < ecosystem.size(); i++)
    {
        if (ecosystem[i].__cost < best_organism.second)
        {
            best_organism.first = i;
            best_organism.second = ecosystem[i].__cost;
            result = true;
        }
    }
    return result;
}

void update_best_tour(tour &__a, tour &__b)
{
    if (__a.__cost < __b.__cost)
    {
        __b = __a;
    }
}

int so (unsigned long termination, int population)
{
    //time
    //int endTime = clock() + msecs;

    // generate x' && identify best organism on ecosystem
    best_tour = __initial_tour;
    
    int best_organism = generate_ecosystem(population, best_tour);
    
    update_best_tour(ecosystem[best_organism], best_tour);
    
    int v_i, v_j;
    
    for (unsigned long i = 0 ; i < termination; i++)
    {
        do
        {
            v_i = rand() % ecosystem.size();
            v_j = rand() % ecosystem.size();
        } while (v_i == v_j);
        
        cout<<best_tour.__cost<<std::endl;
        //mutualism
        apply_mutualism(ecosystem[best_organism].__cost, v_i, v_j);
        
        // comensalism
        apply_comensalism(best_organism, v_i, v_j);
        
        // parasitism
        apply_paristism(ecosystem[best_organism].__cost, v_i, v_j);
        
        //generate new ecosystem & update best
        ecosystem.clear();

        best_organism = generate_ecosystem(population, best_tour);
        
        update_best_tour(ecosystem[best_organism], best_tour);
    }
    return best_tour.__cost;
}

void apply_sa(double &temperature, double &cooling_rate)
{
    int swap_i, swap_j;
    pdd result;
    // swap randomly to get better solution
    
    tour candidate(best_tour);

    for (int __i = 0; __i < 10; __i++)
    {     
        do
        {
            swap_i = rand() % ecosystem.size();
            swap_j = rand() % ecosystem.size();
        } while (swap_i == swap_j);
        result = swap_elements(swap_i, swap_j, candidate.__nodes);        
    }
    // update 
    candidate.__cost = result.second;
    
    double diffcost = (candidate.__cost - best_tour.__cost) /  best_tour.__cost;

    if (candidate.__cost < best_tour.__cost)
    {
        best_tour = candidate;
    }else
    {
        if ( diffcost < 0 )
        {
            best_tour = candidate;
        } else
        {
            if (exp(-diffcost/temperature))
            {
                best_tour = candidate;
                //cout<<"asaaaaaaexp"<<std::endl;
            }
        }
    }
    temperature *= cooling_rate;
}

double so_sa (double termination, int population,
                         double &temperature, double &cooling_rate)
{
    best_tour = __initial_tour;
    
    int best_organism = generate_ecosystem(population, best_tour);
    
    update_best_tour(ecosystem[best_organism], best_tour);
    
    int v_i, v_j;
    
    double last_print = 0;
    
    for (unsigned long i = 0 ; i < termination; i++)
    {

        apply_sa(temperature, cooling_rate);
        
        do
        {
            v_i = rand() % ecosystem.size();
            v_j = rand() % ecosystem.size();
        } while (v_i == v_j);
        
        if (best_tour.__cost != last_print)
            cout<<best_tour.__cost<<std::endl;
        last_print = best_tour.__cost;
        //mutualism
        apply_mutualism(ecosystem[best_organism].__cost, v_i, v_j);
        
        // comensalism
        apply_comensalism(ecosystem[best_organism].__cost, v_i, v_j);
        
        // parasitism
        apply_paristism(ecosystem[best_organism].__cost, v_i, v_j);
        
        //generate new ecosystem & update best
        ecosystem.clear();
       
        best_organism = generate_ecosystem(population, best_tour);
        
        update_best_tour(ecosystem[best_organism], best_tour);
    }
    return best_tour.__cost;
}

double calculate_distance (pdd a,pdd b)
{
    double x = a.first - b.first ;
    double y = a.second - b.second;
    
    return sqrt(pow(x, 2) + pow(y, 2));
}

bool base_swap (int a, int b, vector<pdd> &__nodes)
{
    if (a < 0 ||
        b < 0 ||
        b > __nodes.size() ||
        a > __nodes.size())
        return false;
    
    pair<double, double> tmp = __nodes[a];
    __nodes[a] = __nodes[b];
    __nodes[b] = tmp;
    return true;
}

pdd swap_elements (int node_a, int node_b, vector<pdd> &nodes)
{
    double cost_after_swap = 0;
    pdd result;
    
    //swap elements
    base_swap(node_a, node_b, nodes);
    
    //c after swap cost
    for (int i = 1 ; i < nodes.size(); i++)
    {
        
        cost_after_swap += calculate_distance(nodes[i-1], nodes[i]);
    }
    
    result.first =  0;
    result.second = cost_after_swap;
    
    return result;
}

int main(int argc, const char * argv[])

{
    //read & insert all edges
    float n,x,y;
    cin>>n>>x>>y;
    __initial_tour.__nodes.push_back(pdd(x,y));
    unsigned int count = 0;
    
    while (cin>>n>>x>>y && n != -1)
    {
        __initial_tour.__nodes.push_back(pdd(x,y));
        cost += calculate_distance(
                __initial_tour.__nodes[count],
                __initial_tour.__nodes[count+1]
        );
        count++;
    }
    
    __initial_tour.__cost = cost;
    printf("ecosystem initial cost: %f\n", cost);

    // SO PURO
    double best_organism = so(80000, 50);
    //printf("ecosystem best cost: %f\n", best_organism.second);

    //SO_SA
    double temperature = 0.025, cooling_rate = 0.99;
    //double best_organism = so_sa(100000, 50, temperature, cooling_rate);
    printf("ecosystem best cost: %f\n", best_organism);
    //write result to file
    // ofstream myfile ("tsp.txt");
    // if (myfile.is_open())
    // {
    //     for (int i = 0; i < best_tour.__nodes.size(); i++)
    //     {
    //          myfile << i<<" "<<best_tour.__nodes[i].first <<" "<<best_tour.__nodes[i].second<<std::endl;
    //     }
    // }
    
}
