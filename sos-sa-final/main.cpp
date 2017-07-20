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

pair<int, double> generate_ecosystem(int ecosystem_size, tour &seed_tour)
{
    int best_organism = 0;
    double best_cost = seed_tour.__cost;
    srand ((unsigned)time(NULL));
    pair<int, double> result;
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
    
    result.first = best_organism;
    result.second = best_cost;
    return result;
}

void apply_mutualism(pair<int, double> &best_organism, int v_i, int v_j)
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
    x1 = (x1+randio) * (x1 -mutual * k1) ;//(ecosystem[v_i].__cost + mutual *randio);
    
    //Xj′ =Xj +rand(0,1)×(Xbest −Mutualvect ×K2)
    x2 = (x2+randio) * (x2 - mutual * k2) ;;//(ecosystem[v_j].__cost + mutual *randio);
    
    //updates
    (x1 < ecosystem[v_i].__cost)? ecosystem[v_i].__cost = x1:0;
    (x2 < ecosystem[v_j].__cost)? ecosystem[v_j].__cost = x2:0;
    
}

void apply_comensalism(pair<int, double> &best_organism, int v_i, int v_j)
{
    double randio, x1;
    
    // generate random between 0 and 1
    randio = (rand() % 2) + -1;
    
    //Xi′=Xi+rand(−1,1)× Xbest−Xj if f X′i > f(Xi)
    x1 = (ecosystem[v_i].__cost + randio) * (best_organism.second -ecosystem[v_j].__cost);
    //update
    (x1 < ecosystem[v_i].__cost)? ecosystem[v_i].__cost = x1:0;
}

void apply_paristism(pair<int, double> &best_organism, int v_i, int v_j)
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

pair<int, double> so (double termination, int population)
{
    // generate x' && identify best organism on ecosystem
    pair<int, double> best_organism = generate_ecosystem(population, __initial_tour);
    tour best_tour(ecosystem[best_organism.first]);
    int v_i, v_j;
    
    for (int i = 0; i < termination; i++)
    {
        
        do
        {
            v_i = rand() % ecosystem.size();
            v_j = rand() % ecosystem.size();
        } while (v_i == v_j);
        
        cout<<best_organism.second<<std::endl;
        //mutualism
        apply_mutualism(best_organism, v_i, v_j);
        
        // comensalism
        apply_comensalism(best_organism, v_i, v_j);
        
        // parasitism
        apply_paristism(best_organism, v_i, v_j);
        
        //generate new ecosystem & update best
        ecosystem.clear();
        parasites.clear();
        pair<int, double> t = generate_ecosystem(population, best_tour);
        if (t.second < best_organism.second)
        {
            best_organism = t;
            best_tour = ecosystem[best_organism.first];
        }
    }
    return best_organism;
}

void so_sa (double temperature, double cooling_rate, double termination)
{
    // generate x'
    // apply sa
    // identify best organism
    //mutualism
    // comensalism
    // parasitism
    // apply cooling
    // update best solution
    // is termination criteria reached?
}

double calculate_distance (pdd a,pdd b)
{
    double x = a.first - b.first ;
    double y = a.second - b.second;
    
    return sqrt(pow(x, 2) + pow(y, 2));
}

vector <pair<int, int> > calculate_swap_vector(int node_a, int node_b, vector<pdd> nodes)
{
    vector <pair<int, int> > nodes_change;
    // calculate nodes to change
    int x = node_a - 1,
        x_1 = node_a +1,
        y = node_b -1,
        y_1 = node_b + 1;
    
    if (node_a != 0)
    {
        nodes_change.push_back(make_pair(x, node_a));
    }
    
    if (node_b < nodes.size())
    {
        nodes_change.push_back(make_pair(node_b, y_1));
    }
    
    if (node_a + 1 != node_b) {
        nodes_change.push_back(make_pair(node_a, x_1));
        nodes_change.push_back(make_pair(y, node_b));
    } else
    {
        nodes_change.push_back(make_pair(node_a, x_1));
    }
    return nodes_change;
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

    pair<int, double> best_organism = so(10000, 50);
    printf("ecosystem best cost: %f\n", best_organism.second);
    
}
