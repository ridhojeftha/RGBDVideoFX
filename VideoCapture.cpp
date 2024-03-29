/**
 * RGBDVideoFX - Class that deals with Kinect hardware
 * ridhojeftha
 */
#include "VideoCapture.h"
#include "DepthProcessor.h"

#include <iostream>
#include <fstream>

Freenect::Freenect freenect; //libfreenect instance
DepthProcessor* device; //depth sensing device
double freenect_angle(0);
freenect_video_format requested_format(FREENECT_VIDEO_RGB);

//buffers for depth and rgb
static std::vector<float> rgb(640 * 480 * 3);
static std::vector<float> depth(640 * 480);
static std::vector<float> normals(640 * 480 * 3);

using namespace std;

void updateVideoCapture() {

    //Update the device
    device->updateState();

    rgb = getRGBMap();
 //   device->getRGB(rgb);
    
   // for (int i=0;i<50;i++){
    //   std::cout<<depth[0]<<" "<<depth[1]<<"\n";
     
   // }
    
    
    depth = getDepthMap() /*getColourDepthMap()*/;
   //  depth = getCalibratedDepthMap();
    normals = getNormalMap(); //basically you can call getNormalMap() from anywhere

    //Temporary GL Code
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_RED, GL_FLOAT, &depth[0]);
   // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_RED, GL_FLOAT, &depth[0]);

    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_FLOAT, &rgb[0]);
   // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8UI, 640, 480, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT_8_8_8_8, &rgb[0]);


    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_FLOAT, &normals[0]);
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_FLOAT, &normals[0]);
   
}

//Start the Kinect device video and depth streams

void startVideoCapture() {

    device = &freenect.createDevice<DepthProcessor>(0);
    device->setLed(LED_BLINK_GREEN);

    std::cout << "Starting device...\n";
    fflush(stdout);
    device->setLed(LED_BLINK_RED_YELLOW);
    device->startVideo();
    std::cout << "Video stream started.\n";
    device->startDepth();
    std::cout << "Depth stream started.\n";
    device->setLed(LED_RED);
    std::cout << "Device started.\n";
    fflush(stdout);

}

void stopVideoCapture() {
    std::cout << "Stopping device...\n";
    device->stopVideo();
    device->stopDepth();
    device->setLed(LED_GREEN);

    std::cout << "Device stopped.";
}

/* Kinect LED modes:
    LED_GREEN, LED_RED, LED_YELLOW, LED_BLINK_GREEN,
    LED_BLINK_RED_YELLOW,LED_OFF...
 */
