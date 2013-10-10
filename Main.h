
#ifndef MAIN_H
#define	MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include <libfreenect.hpp>



extern GLuint depthTexture;
extern GLuint colourTexture;
extern GLuint normalTexture;
extern GLuint reflectanceTexture;
extern int inputWidth;
extern int inputHeight;
extern int window;

//CHANGE THESE
bool useKinect=true;
bool useEffects=true;

int frameCounter=0;
bool benchmarking=false;
double totalTime=0;

unsigned long  startTime, endTime, tenFrameTime;

int main(int argc, char** argv);
void keyboard(unsigned char key, int x, int y);
void update();
void close();
void idle();

void loadTextureFiles();

#endif

