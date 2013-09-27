#ifndef PIXELFILTER_H_INCLUDED
#define PIXELFILTER_H_INCLUDED
#include <math.h>
#include <cmath>

int widthMax = 640 - 1;
int heightMax = 480 - 1;

uint16_t filterPixel(int index, uint16_t *&rawDepth){

    int sum = 0;
    int counter = 0;

    std::vector<int> neighbours;

    float sumWeight = 0;
    float outputDepth = 0;

    //the index of the current pixel
    int x = index%640;
    int y = index/640;

    //look through the neighbouring pixels
    for (int blockY = -2; blockY < 3; blockY++){
        for (int blockX = -2; blockX < 3; blockX++){

         //dont want to consider the centre pixel
         if (blockY !=0 || blockX !=0){

            //modified coords
            int xIndex = x + blockX;
            int yIndex = y + blockY;

            //check that the index is in bounds
            if (xIndex >=0 && xIndex < 640 && yIndex >=0 && yIndex <480){

                //get the array index
                int currentPixel = yIndex * 640 + xIndex;

                //if the value is valid, add it to the list
                if (rawDepth[currentPixel] < 2047){

                   //mean

                   counter++;
                   sum += rawDepth[currentPixel];


                   //mode
                   /*
                   bool encountered = false;

                   //check if the depth has been encountered before
                    for (int k=0; k< neighbours.size()/2; k++){
                        if (rawDepth[currentPixel]==neighbours[2*k+0]){
                            neighbours[2*k+1] = neighbours[2*k+1]+1;
                            encountered = true;
                            break;
                        }
                    }

                    if (!encountered){
                            neighbours.push_back(rawDepth[currentPixel]);
                            neighbours.push_back(1);
                            }
                    */
                   // std::cout << rawDepth[currentPixel] << std::endl;

                    /*
                    //bilateral filtering
                    int space = std::sqrt (blockX*blockX+blockY*blockY);
                    int intensity = std::sqrt ((rawDepth[currentPixel]-0)*(rawDepth[currentPixel]-0));

                    //std::cout << rawDepth[currentPixel] << " " << intensity << std::endl;

                    float currentWeight = 1.0/(std::exp(space*space/72)*std::exp(intensity*intensity*8));
                    sumWeight += currentWeight;
                    outputDepth += currentWeight*rawDepth[currentPixel];*/
                    }
            }
         }
        }
    }

    //mean

    if (counter!=0){
        int temp = sum/counter;
        return temp;
    }
     return rawDepth[index];

    //mode
    /*
    int freq = 0;
    int depth = rawDepth[index];

    for (int k=0; k< neighbours.size()/2; k++){
        if (neighbours[2*k+1] > freq){

            depth = neighbours[2*k+0];
            freq = neighbours[2*k+1];
        }
    }
    return depth;
    */
}


#endif // PIXELFILTER_H_INCLUDED
