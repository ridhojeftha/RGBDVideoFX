#ifndef NORMALMAPGENERATOR_H
#define NORMALMAPGENERATOR_H
extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }
 #include <stdio.h>
 #include <math.h>
 #include <vector>
 #include <iostream>
 #include "Vector3.h"

    using namespace std;

static vector<float> normalMap(640 * 480 * 4, 0.0);

void generateNormalMap(int index, Vector3 centre_vertex, Vector3 right_vertex, Vector3 up_vertex){

    //use the cross product of normals x+1 -> x and y+1 -> y


    //create vectors
    Vector3 x_vector = right_vertex - centre_vertex;
    Vector3 y_vector = up_vertex - centre_vertex;

    //y_vector.cross(x_vector)
    Vector3 normal(x_vector.cross (y_vector));

    normal.normalize();

    normalMap[3 * index +0] = (normal.x+1)/2;
    normalMap[3 * index +1] = (normal.y+1)/2;
    normalMap[3 * index +2] = (normal.z+1)/2;
}

    //Method to be called to get Normal Map
    //In the range of [0.0 - 1.0]
    vector<float> getNormalMap(){
    return normalMap;
	}

#endif // NORMALMAPGENERATOR_H
