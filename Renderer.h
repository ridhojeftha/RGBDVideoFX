#ifndef RENDERER_H
#define	RENDERER_H


#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL/glui.h>




void displayEffects(void);
void initRenderer(int argc, char** argv, int, int h);
void cleanupRenderer();
void loadBMP(const char * filename, GLuint textureID);
void fillTextureFromBuffer(unsigned char * data, GLuint textureID, int w, int h);
GLuint createTexture();
void createQuad();
void renderQuad(GLuint fb);
void gluiCallback(int control);
void createInterface();
void displayMaps();

extern bool useEffects;
extern bool useKinect;

#endif


