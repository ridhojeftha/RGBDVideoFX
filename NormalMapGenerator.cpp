extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }
 #include "NormalMapGenerator.h"
 #include <stdio.h>
 #include <math.h>

    using namespace std;

pthread_t thread[640*480];
vector<float> vertexMap(640 * 480 * 3);
vector<float> normalMap(640 * 480 * 3);


void startNormals(){
   }

//Method to covert raw depth to meters
float convertToMeters (int rawDepthValue){

    if (rawDepthValue < 2047)
    {
        //convert to meters using Kinect's internal callibration - given by ROS
        return float (1.0 / (double (rawDepthValue) * -0.0030711016 + 3.3309495161));
    }
    return 0.0f;
}

//returns one array with three values world_x, world_y and depth_value in ints.
void * convertToWorld (void *arg){

    int * pixel = static_cast<int*>(arg);

    //depth to world constants
    static const double fx_d = 1.0 / 5.9421434211923247e+02;
    static const double fy_d = 1.0 / 5.9104053696870778e+02;
    static const double cx_d = 3.3930780975300314e+02;
    static const double cy_d = 2.4273913761751615e+02;

    //conversion formula, frst convert raw_depth to meters
    const double depth = convertToMeters(pixel[2]);
    int index = pixel[1] * 640 + pixel[0]; //row*WIDTH + col

    //put this xyz vector in a buffer
    vertexMap[3 * (index) + 0] = float((pixel[0] - cx_d) * depth * fx_d);
    vertexMap[3 * (index) + 1] = float((pixel[1] - cy_d) * depth * fy_d);
    vertexMap[3 * (index) + 2] = float(depth);

    pthread_exit(NULL);
    return 0;
}


//returns a vector with three float values stored.
void * calculateNormal (void *arg){

     int * i = static_cast<int*>(arg);
     int index = i[0];

    //use the cross product of normals x+1 -> x and y+1 -> y

    //the xy picture index
    int x = index%640;
    int y = index/640;

   //indices of x+1 and y+1 vertices
    int index_x1 = y * 640 + (x+1); //vertex to the right
    int index_y1 = (y+1) * 640 + x; //vertex above

    if (index_x1>=640){index_x1=639;}
    if (index_y1>=480){index_y1=479;}

    //values of the vertices
    float vert[3] = {vertexMap[3 * (index) +0], vertexMap[3 * (index) +1], vertexMap[3 * (index) +2]};
    float vert_x1[3] = {vertexMap[3 * (index_x1) +0], vertexMap[3 * (index_x1) +1], vertexMap[3 * (index_x1) +2]}; //vertex at x+1
    float vert_y1[3] = {vertexMap[3 * (index_y1)+0], vertexMap[3 * (index_y1) +1], vertexMap[3 * (index_y1) +2]}; //vertex at y+1

    //create vectors
    float x_vector[3] = {vert_x1[0] - vert[0], vert_x1[1] - vert[1], vert_x1[2] - vert[2]};
    float y_vector[3] = {vert_y1[0] - vert[0], vert_y1[1] - vert[1], vert_y1[2] - vert[2]};

    /*
                y * vec.z - z * vec.y,
				z * vec.x - x * vec.z,
				x * vec.y - y * vec.x
    */

    //y_vector.cross(x_vector)
    normalMap[3 * index +0] = y_vector[1] * x_vector[2] - y_vector[2] * x_vector[1];
    normalMap[3 * index +1] = y_vector[2] * x_vector[0] - y_vector[0] * x_vector[2];
    normalMap[3 * index +2] = y_vector[0] * x_vector[1] - y_vector[1] * x_vector[0];

    //normalize vector
    double vector_length = sqrt((normalMap[3 * index +0]*normalMap[3 * index +0])+
                                (normalMap[3 * index +1]*normalMap[3 * index +1])+
                                (normalMap[3 * index +2]*normalMap[3 * index +2]));

    normalMap[3 * index +0] = normalMap[3 * index +0]/vector_length;
    normalMap[3 * index +1] = normalMap[3 * index +1]/vector_length;
    normalMap[3 * index +2] = normalMap[3 * index +2]/vector_length;

    pthread_exit(NULL);
    return 0;
}

vector<float> getNormalMap (){
    return normalMap;
}


void updateNormals(std::vector<int> rawDepth, std::vector<float> normals){


    //Convert the depth map into a vertex map by converting the xyz to world coordinates
    for (int i=0; i< 640 * 480; i++){

        //pthreads can only take a void* as argument so save values in an array
        //pixel is defined as (x-index, y-index, raw_depth_value
        int pixel[3] = { i%640, i/640, rawDepth[i]};

        //run threads for each pixel
        pthread_create(&thread[i], NULL, convertToWorld, static_cast<void*>(&pixel));
    }

    // Finish threads
    for (int i = 0; i < 640*480; ++i) {
        pthread_join(thread[i], NULL);
    }

    //calculate the normals by converting vertex map into normal map
    for (int i=0; i< 640 * 480; i++){

        //send index and centre_vector.x, centre_vector.y, centre_vector.z
        pthread_create(&thread[i], NULL, calculateNormal, static_cast<void*>(&i));
    }
    // Finish threads
    for (int i = 0; i < 640*480; ++i) {
        pthread_join(thread[i], NULL);
    }

    //normal map created

    normals.swap(normalMap);
}



void stopNormals(){

}

