/**
 * RGBDVideoFX - Class that deals with Kinect hardware
 * ridhojeftha
 */
#ifndef VIDEOCAPTURE_H
#define	VIDEOCAPTURE_H
#define GLEW_STATIC
#include "GL/glew.h"
#include <GL\freeglut.h>

void updateVideoCapture();
void startVideoCapture();
void stopVideoCapture();


extern GLuint depthTexture;
extern GLuint colourTexture;
extern GLuint normalTexture;
extern GLuint reflectanceTexture;

extern bool useEffects;
extern int window;

#endif

