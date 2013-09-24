#ifndef VIDEOCAPTURE_H
#define	VIDEOCAPTURE_H

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

