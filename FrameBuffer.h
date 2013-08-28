
#ifndef FRAMEBUFFER_H
#define	FRAMEBUFFER_H


#include <stdio.h>

#define GLEW_STATIC
#include<GL/glew.h>


class FrameBuffer
{
    public:
        FrameBuffer(int width, int height);
        virtual ~FrameBuffer();
        GLuint texture() const;
        GLuint fbo() const;
       
    private:
        GLuint textureID;
        GLuint fboID;


};


#endif	
