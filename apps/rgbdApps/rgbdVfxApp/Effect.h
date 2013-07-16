#ifndef EFFECT_H
#define	EFFECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "Shader.h"
#include "FrameBuffer.h"

extern GLuint depthTexture;
extern GLuint colourTexture;
extern GLuint normalTexture;
extern int width;
extern int height;

class Effect {
public:

    virtual ~Effect() {
    };
    virtual void keyboard(unsigned char key) = 0;
    virtual void display() = 0;


private:

};

#endif	

