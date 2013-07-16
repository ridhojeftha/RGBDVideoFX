#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Renderer.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Effect.h"
#include "FogEffect.h"
#include "DOFEffect.h"


int width = 0;
int height = 0;

GLuint colourTexture = 0;
GLuint depthTexture = 0;
GLuint normalTexture = 0;

GLuint quadVAO = 0;
GLuint quadVBO = 0;


Effect * fogEffect = NULL;
Effect * dofEffect = NULL;

//Initialise the renderer and window
void initRenderer(int argc, char** argv, int w, int h) {

    width = w;
    height = h;

    glutInit(&argc, argv);

    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("RGBD");

    glewInit();

    createQuad();

    colourTexture = createTexture();
    depthTexture = createTexture();
    normalTexture = createTexture();


    fogEffect = new FogEffect();
    dofEffect = new DOFEffect();


}

//Displays the current effect
void displayEffects() {

    // clear everything
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

   // fogEffect->display();
    dofEffect->display();

    //Swap the back and front buffer
    glutSwapBuffers();
}


// loads a quad into the VAO global

void createQuad() {

    // make and bind the VAO
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    // make and bind the VBO
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    // Put the verticies into the VBO
    GLfloat vertexData[] = {
        //  X     Y     Z     U     V
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    };

    glBufferData(GL_ARRAY_BUFFER, sizeof (vertexData), vertexData, GL_STATIC_DRAW);

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    delete [] vertexData;
}


//Render the quad to a specified framebuffer

void renderQuad(GLuint fb) {

    glBindFramebuffer(GL_FRAMEBUFFER, fb); //Use specified frame buffer
    glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // bind the VAO
    glBindVertexArray(quadVAO);

    // draw the VAO
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // unbind the VAO
    glBindVertexArray(0);

}

//Fill a texture from a bmp file

void fillTextureFromBMP(const char * filename, GLuint textureID) {

    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos; // Position in the file where the actual data begins
    unsigned int width, height; // dimensions
    unsigned int imageSize; // = width*height*3
    unsigned char * data; // Actual BGR data

    // Open the file
    FILE * file = fopen(filename, "rb");
    if (!file) {
        std::cerr << filename << " - Image could not be opened\n";
        return;
    }

    if (fread(header, 1, 54, file) != 54) {
        std::cerr << filename << " - Not a correct BMP file\n";
        return;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << filename << " - Not a correct BMP file\n";
        return;
    }

    // Read ints from the byte array
    dataPos = *(int*) &(header[0x0A]);
    imageSize = *(int*) &(header[0x22]);
    width = *(int*) &(header[0x12]);
    height = *(int*) &(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0) dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    //Fill the texture with BGR data
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);


    delete data;

}

//Free OpenGL resources

void cleanupRenderer() {

    //Delete effects (and effect specific resources)
    delete fogEffect;

    //Delete textures
    glDeleteTextures(1, &colourTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteTextures(1, &normalTexture);

    //Delete arrays and buffers
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void rendererKeyboard(unsigned char key){
    dofEffect->keyboard(key);
}



//Creates an OpenGL texture

GLuint createTexture() {

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    //Setup filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;

}

//Fills a texture from a buffer

void fillTextureFromBuffer(unsigned char * data, GLuint textureID,int w, int h) {

    //Fill the texture with RGB data
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

}

