#ifndef SHADER_H
#define SHADER_H


#define GLEW_STATIC
#include<GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>


extern GLuint quadVAO;
extern GLuint quadVBO;

class Shader
{
    public:
        Shader(const char *fragFile);
        GLuint loadShaderFile(const char *shaderFile, GLenum type);
        virtual ~Shader();
        GLuint id() const;
        GLint attrib(const GLchar* attribName) const;
        GLint uniform(const GLchar* uniformName) const;
    private:
        GLuint programID;


};

#endif
