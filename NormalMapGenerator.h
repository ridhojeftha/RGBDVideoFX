#ifndef NORMALMAPGENERATOR_H
#define NORMALMAPGENERATOR_H
#include <vector>

void updateNormals(std::vector<int> rawDepth, std::vector<float> normals);
void startNormals();
void stopNormals();

//extern int * rawDepth;
extern std::vector<float> vertexMap;
extern std::vector<float> normalMap;

#endif // NORMALMAPGENERATOR_H
