#include "VideoCapture.h"
#include "DepthProcessor.h"
#include <iostream>
#include <fstream>

Freenect::Freenect freenect; //libfreenect instance
DepthProcessor* device; //depth sensing device
double freenect_angle(0);
freenect_video_format requested_format(FREENECT_VIDEO_RGB);

//buffers for depth and rgb
static std::vector<int> rgb(640 * 480 * 3);
static std::vector<float> depth(640 * 480);
static std::vector<float> normals(640*480*3);

    using namespace std;

void updateVideoCapture() {

    //Update the device
    device->updateState();

    //vector<int> rawDepth(640*480);
    //device->getDepth(depth);
    //device->getRGB(rgb);

    rgb = getRGBMap();
    depth = getDepthMap();
    normals = getNormalMap(); //basically you can call getNormalMap() from anywhere

    //Temporary GL Code
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_FLOAT, &depth[0]);

    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_FLOAT, &normals[0]);
}

//Start the Kinect device video and depth streams
void startVideoCapture() {

    device = &freenect.createDevice<DepthProcessor>(0);
    device->setLed(LED_BLINK_GREEN);

    std::cout << "Starting device...\n";
    fflush(stdout);
    device->startVideo();
    std::cout << "Video stream started.\n";
    device->startDepth();
    std::cout << "Depth stream started.\n";
    device->setLed(LED_RED);
    std::cout << "Device started.\n";
    fflush(stdout);

}

void stopVideoCapture() {
    device->stopVideo();
    device->stopDepth();
    device->setLed(LED_GREEN);

    std::cout << "Device stopped.";
}

/* Kinect LED modes:
    LED_GREEN, LED_RED, LED_YELLOW, LED_BLINK_GREEN,
    LED_BLINK_RED_YELLOW,LED_OFF...
 */
