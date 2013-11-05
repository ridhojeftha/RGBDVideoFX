#ifndef EFFECT_H
#define	EFFECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "Shader.h"
#include "FrameBuffer.h"

extern GLuint depthTexture;
extern GLuint colourTexture;
extern GLuint normalTexture;
extern GLuint reflectanceTexture;
extern int screenWidth;
extern int screenHeight;
extern int inputWidth;
extern int inputHeight;

class Effect {
public:

    virtual ~Effect() {
    };
    virtual void display() = 0;
    virtual void resize() = 0;


private:

};

#endif	

