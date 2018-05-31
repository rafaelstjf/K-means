#include "Model.hpp"

Model::Model()
{
    isLoaded = false;
}
void Model::loadModel(string filename)
{
    char separator;
    string line;
    int pos;
    file.open(filename, fstream::in);
    if(file.is_open())
    {
        cout << "Type the value separator (ex. dot, comma, semicolon)" << endl;
        cin >> separator;
        //get numColumns
        file.clear();
        file.seekg(0, ios::beg);
        getline(file, line);
        numColumns = 0;
        for(int i = 0; i < line.size(); i++)
        {
            if(line[i]==separator)
                numColumns++;

        }
        numColumns++;
        //get numRows
        numRows = 0;
        file.clear();
        file.seekg(0, ios::beg);
        while(getline(file,line))
        {
            numRows++;
        }
        //create matrix
        matrix = new double*[numRows];
        for(int i = 0; i < numRows; i++)
        {
            matrix[i] = new double[numColumns];
        }
        //get values
        file.clear();
        file.seekg(0, ios::beg);
        int r = 0;
        while(getline(file, line))
        {
            string buffer;
            int c = 0;
            for(int j = 0; j < line.size(); j++)
            {

                if(line[j]==separator)
                {
                    matrix[r][c] = atof(buffer.c_str());
                    buffer.clear();
                    c++;

                }
                else
                {
                    buffer+=line[j];
                }
                if(j==line.size()-1)
                {
                    matrix[r][c] = atof(buffer.c_str());
                }
            }
            r++;
        }
        isLoaded = true;
    }
}
void Model::printMatrix()
{
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

}
void Model::normalize()
{

    int min = matrix[0][0];
    int max = matrix[0][0];
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            if(matrix[i][j] > max)
                max = matrix[i][j];
            if(matrix[i][j] < min)
                min = matrix[i][j];
        }
    }
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            matrix[i][j] = (matrix[i][j]-min)/(max-min);
        }
    }
}
int Model::getNumColumns()
{
    return numColumns;
}
int Model::getNumRows()
{
    return numRows;
}
double** Model::getMatrix()
{
    return matrix;
}
bool Model::getLoadStatus()
{
    return isLoaded;
}
