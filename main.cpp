#include <iostream>
#include <string>
#include "Kmeans.hpp"
using namespace std;

int main()
{
    string filename;
    int numGroups, maxIt;
    Kmeans* k = new Kmeans();
    cout << "K-means 0.5" << endl;
    cout << "Please enter the name + extension of the file." << endl;
    cin >> filename;
    cout << "Insert the number of clusters." << endl;
    cin >> numGroups;
    cout << "Insert the maximum of iterations." << endl;
    cin >> maxIt;
    cout << "Running..." << endl;
    k->run(filename, maxIt, numGroups);
    return 0;
}
