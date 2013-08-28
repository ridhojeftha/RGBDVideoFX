
#ifndef KINECT_H
#define	KINECT_H

#include "Capture.h"

void updateKinect();
void startKinect();
void stopKinect();


extern GLuint depthTexture;
extern GLuint colourTexture;
extern GLuint normalTexture;
extern GLuint reflectanceTexture;

extern bool useEffects;
extern int window;

#endif	

