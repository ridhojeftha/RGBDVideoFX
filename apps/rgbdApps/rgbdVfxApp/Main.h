
#ifndef MAIN_H
#define	MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Renderer.h"

extern GLuint depthTexture;
extern GLuint colourTexture;
extern GLuint normalTexture;

int frameCounter=0;
double totalTime=0;
LARGE_INTEGER frequency;
LARGE_INTEGER startTime, endTime;         
int main(int argc, char** argv);
void keyboard(unsigned char key, int x, int y);
void update();
void close();


#endif

