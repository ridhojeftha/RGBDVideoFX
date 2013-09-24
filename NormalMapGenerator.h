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

static vector<float> normalMap(640 * 480 * 4);

void generateNormalMap(int index, Vector3 centre_vertex, Vector3 right_vertex, Vector3 up_vertex){

    //use the cross product of normals x+1 -> x and y+1 -> y


    //create vectors
    Vector3 x_vector = right_vertex - centre_vertex;
    Vector3 y_vector = up_vertex - centre_vertex;

    //y_vector.cross(x_vector)
    Vector3 normal(y_vector.cross (x_vector));

    normal.normalize();

    normalMap[3 * index +0] = normal.x;
    normalMap[3 * index +1] = normal.y;
    normalMap[3 * index +2] = normal.z;
}

    //Method to be called to get Normal Map
			bool getNormal(vector<float> &buffer){
				//m_depth_mutex.lock(); CREATE MUTEX LOCK

				//TODO: cmaybe change m_new_depth_frame and m_new_rgb_frame to one variable as we want synchronized frames
				//if(m_new_depth_frame) {
					buffer.swap(normalMap);
					//m_new_depth_frame = false;
					//m_depth_mutex.unlock();
					return true;
				//} else {
					//m_depth_mutex.unlock();
				//	return false;
				//}
			}

#endif // NORMALMAPGENERATOR_H
