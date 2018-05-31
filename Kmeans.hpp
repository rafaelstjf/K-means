#ifndef KMEANS_HPP
#define KMEANS_HPP
#include <iostream>

#include "Model.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include <stdlib.h>
#include <algorithm>
using namespace std;

class Kmeans
{
private:
    double numGroups;
    Model* model;
    int maxIterations;
    double** centers;
    bool** centersEqual;
    int* elementGroup;
    bool sameCenters;
    void initialCenters();
    void Agroup();
    void calcNewCenters();
    void saveResult(string filename);
public:
    Kmeans();
    void run(string filename, int maxIterations, int numGroups);
    ~Kmeans();
};

#endif
