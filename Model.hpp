#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <iostream>
#include <fstream>
using namespace std;
class Model
{
  private:
  int numColumns;
  int numRows;
  double** matrix;
  bool isLoaded;
  fstream file;
  public:
  Model();
  void loadModel(string filename);
  double** getMatrix();
  int getNumColumns();
  int getNumRows();
  void printMatrix();
  bool getLoadStatus();
  void normalize();
  ~Model();


};
#endif
