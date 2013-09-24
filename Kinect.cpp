//Change this class to VideoCapture
#include "Kinect.h"
#include "NormalMapGenerator.h"
#include <iostream>
#include <fstream>

Freenect::Freenect freenect;
Capture* device;
double freenect_angle(0);
freenect_video_format requested_format(FREENECT_VIDEO_RGB);

    using namespace std;

void updateKinect() {

    //buffers for depth and rgb
    static std::vector<uint8_t> rgb(640 * 480 * 4);
    static std::vector<uint8_t> depth(640 * 480 * 4);

    // using getTiltDegs() in a closed loop is unstable
    /*if(device->getState().m_code == TILT_STATUS_STOPPED){
            freenect_angle = device->getState().getTiltDegs();
    }*/
    device->updateState();
    //printf("\r demanded tilt angle: %+4.2f device tilt angle: %+4.2f", freenect_angle, device->getState().getTiltDegs());
    //fflush(stdout);

    vector<int> rawDepth(640*480);
    device->getDepth(depth, rawDepth);
    device->getRGB(rgb);

    //call the Normal Map Generator from here (actually call it from Capture.h)
   // std::vector<float> normals;

    //if (!rawDepth.empty())
    //    updateNormals(rawDepth, normals);

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);


    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
    //  if (device->getVideoFormat() == FREENECT_VIDEO_RGB || device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB){
    //  glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);
    //  }else{
    //  glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &depth[0]);
    // }


}




void startKinect() {


    //  FREENECT_DEPTH_11BIT        = 0, /**< 11 bit depth information in one uint16_t/pixel */
    //	FREENECT_DEPTH_10BIT        = 1, /**< 10 bit depth information in one uint16_t/pixel */
    //	FREENECT_DEPTH_11BIT_PACKED = 2, /**< 11 bit packed depth information */
    //	FREENECT_DEPTH_10BIT_PACKED = 3, /**< 10 bit packed depth information */
    //	FREENECT_DEPTH_REGISTERED   = 4, /**< processed depth data in mm, aligned to 640x480 RGB */
    //	FREENECT_DEPTH_MM           = 5, /**< depth to each pixel in mm, but left unaligned to RGB image */
    //	FREENECT_DEPTH_DUMMY        = 2147483647, /**< Dummy value to force enum to be 32 bits wide */


    device = &freenect.createDevice<Capture>(0);
  //  device->setDepthFormat(FREENECT_DEPTH_11BIT);
    device->setLed(LED_BLINK_GREEN);


    std::cout << "Starting device...\n";
    fflush(stdout);
    device->startVideo();
    device->startDepth();
    device->setLed(LED_RED);
    std::cout << "Device started.";
    fflush(stdout);

}

void stopKinect() {
    device->stopVideo();
    device->stopDepth();
    device->setLed(LED_GREEN);
}

/*void keyPressed(unsigned char key, int x, int y) {
    //these will change to Robbie's implementation
    if (key == 27) {
        glutDestroyWindow(window);
        die = true;
    }
    if (key == '1') {
        device->setLed(LED_GREEN);
    }
    if (key == '2') {
        device->setLed(LED_RED);
    }
    if (key == '3') {
        device->setLed(LED_YELLOW);
    }
    if (key == '4') {
        device->setLed(LED_BLINK_GREEN);
    }
    if (key == '5') {
        // 5 is the same as 4
        device->setLed(LED_BLINK_GREEN);
    }
    if (key == '6') {
        device->setLed(LED_BLINK_RED_YELLOW);
    }
    if (key == '0') {
        device->setLed(LED_OFF);
    }
    if (key == 'f') {
        if (requested_format == FREENECT_VIDEO_IR_8BIT)
            requested_format = FREENECT_VIDEO_RGB;
        else if (requested_format == FREENECT_VIDEO_RGB)
            requested_format = FREENECT_VIDEO_YUV_RGB;
        else
            requested_format = FREENECT_VIDEO_IR_8BIT;
        device->setVideoFormat(requested_format);
    }

    if (key == 'w') {
        freenect_angle++;
        if (freenect_angle > 30) {
            freenect_angle = 30;
        }
    }
    if (key == 's' || key == 'd') {
        freenect_angle = 0;
    }
    if (key == 'x') {
        freenect_angle--;
        if (freenect_angle < -30) {
            freenect_angle = -30;
        }
    }
    if (key == 'e') {
        freenect_angle = 10;
    }
    if (key == 'c') {
        freenect_angle = -10;
    }
    device->setTiltDegrees(freenect_angle);
}
 */
