#include "Kmeans.hpp"

Kmeans::Kmeans()
{
    model = new Model();
    srand(27182378127381273);
    numGroups = 0;
}
Kmeans::~Kmeans()
{

}
void Kmeans::initialCenters()
{
    //get Numgroups centers using a random object on the model
    double **m = model->getMatrix();
    int index =0;
    for(int i = 0; i < numGroups; i++)
    {
        index = rand()%model->getNumRows();
        for(int j = 0; j < model->getNumColumns(); j++)
        {
            centers[i][j] = m[index][j];

        }
    }
}
void Kmeans::Agroup()
{
    double sum;
    double** matrix = model->getMatrix();
    double* distCenters = new double[model->getNumRows()];
    //calculate for center index 0
    for(int i = 0; i < model->getNumRows(); i++)
    {
        sum = 0;
        for(int j = 0; j < model->getNumColumns(); j++)
        {
            sum+=(centers[0][j] - matrix[i][j])*(centers[0][j] - matrix[i][j]);
        }
        distCenters[i] = sqrt(sum);
        elementGroup[i] = 0;
    }
    //calculate for all centers - first
    for(int k = 1; k < numGroups; k++)
    {
        for(int i = 0; i < model->getNumRows(); i++)
        {
            sum = 0;
            for(int j = 0; j < model->getNumColumns(); j++)
            {
                //cout << centers[k][j] << " : j: " << j << " : k: "<< k<< endl;
                sum+=(centers[k][j] - matrix[i][j])*(centers[k][j] - matrix[i][j]);
            }
            sum = sqrt(sum);
            if(sum < distCenters[i])
            {
                distCenters[i] = sum;
                elementGroup[i] = k;
            }
        }
    }
}
void Kmeans::calcNewCenters()
{
    double* medium = new double[model->getNumColumns()];
    int elementsPerGroup;
    for(int i = 0; i < numGroups; i++)
    {
        for(int j = 0; j < model->getNumColumns(); j ++)
        {
            centersEqual[i][j] = false;
        }
    }
    double** m = model->getMatrix();
    for(int k = 0; k < numGroups; k++)
    {
        elementsPerGroup = 0;
        for(int i = 0; i < model->getNumColumns(); i ++)
        {
            medium[i] = 0.0;
        }
        for(int i = 0; i < model->getNumRows(); i++)
        {
            if(elementGroup[i]==k)
            {
                elementsPerGroup++;
                for(int j = 0; j < model->getNumColumns(); j ++)
                {
                    medium[j] += m[i][j];
                }
            }
        }
        for(int i = 0; i < model->getNumColumns(); i++)
        {
            double centerOld = centers[k][i];
            centers[k][i] = medium[i]/elementsPerGroup;
            if(centers[k][i] == centerOld)
            {
                centersEqual[k][i] = true;
            }
        }
    }
    sameCenters = true;
    for(int k = 0; k < numGroups; k++)
    {
        for(int i = 0; i < model->getNumColumns(); i++)
        {
            if(centersEqual[k][i]==false)
            {
                sameCenters = false;
                break;
            }
        }
    }
}
void Kmeans::saveResult(string filename){
    fstream outFile;
    string buffer;
    for(int i = 0; i < filename.size(); i++){
        if(filename[i]=='.')
        {
            buffer+=("_output");
        }
        buffer+=filename[i];
    }
    outFile.open(buffer, fstream::out | fstream::trunc);
    if(outFile.is_open()){
        stringstream line;
        double** m = model->getMatrix();
        for(int i = 0; i < model->getNumRows(); i++){
            for(int j = 0; j < model->getNumColumns(); j++){
                line << m[i][j] << ", ";
            }
            line << elementGroup[i] << endl;
        }
        outFile << line.str();
        cout << "Result saved on " << buffer << endl;
    }
}
void Kmeans::run(string filename, int maxIterations, int numGroups)
{
    this->maxIterations = maxIterations;
    this->numGroups = numGroups;
    model->loadModel(filename);
    if(model->getLoadStatus())
    {
        //model->normalize();
        elementGroup= new int[model->getNumRows()];
        centersEqual = new bool*[numGroups];
        centers = new double*[numGroups];
        for(int i =0; i < numGroups; i++)
        {
            centers[i] = new double[model->getNumColumns()];
            centersEqual[i] = new bool[model->getNumColumns()];
        }
        initialCenters();
        int it = 0;
        sameCenters=false;
        while(it <=maxIterations && sameCenters==false)
        {
            Agroup();
            calcNewCenters();
            it++;
        }
        saveResult(filename);
        cout << "Algorithm finished." << endl;
    }
    else
    {
        cout << "Failed to load file!" <<endl;
    }
}
