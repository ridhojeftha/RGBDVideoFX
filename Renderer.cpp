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
#include "RelightingEffect.h"
#include "CartoonEffect.h"


int window = -1;
GLUI *glui = NULL;

int selectedEffect = 3;

int screenWidth = 0;
int screenHeight = 0;

int inputHeight = 0;
int inputWidth = 0;

GLuint colourTexture = 0;
GLuint depthTexture = 0;
GLuint normalTexture = 0;
GLuint reflectanceTexture = 0;

GLuint quadVAO = 0;
GLuint quadVBO = 0;


FogEffect * fogEffect = NULL;
DOFEffect * dofEffect = NULL;
RelightingEffect * relightingEffect = NULL;
CartoonEffect * cartoonEffect = NULL;

//Initialise the renderer and window

void initRenderer(int argc, char** argv, int w, int h) {

    screenWidth = w;
    screenHeight = h;

    if (useEffects) {

        glutInit(&argc, argv);

        glutInitWindowSize(screenWidth, screenHeight);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        window = glutCreateWindow("RGBD - Waiting for Kinect...");


        glewInit();

        colourTexture = createTexture();
        depthTexture = createTexture();
        normalTexture = createTexture();
        reflectanceTexture = createTexture();

        createQuad();


        fogEffect = new FogEffect();
        dofEffect = new DOFEffect();
        relightingEffect = new RelightingEffect();
        cartoonEffect = new CartoonEffect();

        createInterface();

    } else {

        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
        glutInitWindowSize(screenWidth, screenHeight);
        glutInitWindowPosition(0, 0);

        window = glutCreateWindow("RGBD - Test Maps");

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0);
        glDepthFunc(GL_LESS);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel(GL_SMOOTH);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glGenTextures(1, &colourTexture);
        glBindTexture(GL_TEXTURE_2D, colourTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glGenTextures(1, &normalTexture);
        glBindTexture(GL_TEXTURE_2D, colourTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }


}

//Displays the current effect

void displayEffects() {

    // clear everything
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);



    switch (selectedEffect) {
        case 0:
            fogEffect->display();
            break;
        case 1:
            dofEffect->display();
            break;
        case 2:
            relightingEffect->display();
            break;
        case 3:
            cartoonEffect->display();
            break;
    }


    //Swap the back and front buffer
    glutSwapBuffers();
}

void createInterface() {

    GLUI *glui = GLUI_Master.create_glui("RGBD Effects", 0, 1050, 100);

    //Effect Selection
    GLUI_Panel *effectSelectionPanel = glui->add_panel("Effect");
    GLUI_RadioGroup *effectRadioGroup = glui->add_radiogroup_to_panel(effectSelectionPanel, &selectedEffect, -1, gluiCallback);
    glui->add_radiobutton_to_group(effectRadioGroup, "Fog");
    glui->add_radiobutton_to_group(effectRadioGroup, "Depth of Field");
    glui->add_radiobutton_to_group(effectRadioGroup, "Relighting");
    glui->add_radiobutton_to_group(effectRadioGroup, "Cartoon Shading");


    //FOG
    GLUI_Rollout *fogRollout = glui->add_rollout("Fog", 0);
    fogRollout->set_w(218);
    glui->add_statictext_to_panel(fogRollout, "")->set_w(200);
    GLUI_Spinner *fogDensitySpinner = glui->add_spinner_to_panel(fogRollout, "Density ", GLUI_SPINNER_FLOAT, &fogEffect->fogDensity, -1, gluiCallback);
    fogDensitySpinner->set_float_limits(0.0, 1);

    glui->add_statictext_to_panel(fogRollout, "");
    GLUI_Panel* fogColourPanel = glui->add_panel_to_panel(fogRollout, "Colour", GLUI_PANEL_EMBOSSED);

    GLUI_Spinner *fogColourRedSpinner = glui->add_spinner_to_panel(fogColourPanel, "Red ", GLUI_SPINNER_FLOAT, &fogEffect->fogColourRed, -1, gluiCallback);
    fogColourRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *fogColourGreenSpinner = glui->add_spinner_to_panel(fogColourPanel, "Green ", GLUI_SPINNER_FLOAT, &fogEffect->fogColourGreen, -1, gluiCallback);
    fogColourGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *fogColourBlueSpinner = glui->add_spinner_to_panel(fogColourPanel, "Blue ", GLUI_SPINNER_FLOAT, &fogEffect->fogColourBlue, -1, gluiCallback);
    fogColourBlueSpinner->set_float_limits(0.0, 1);


    //DEPTH OF FIELD
    GLUI_Rollout *DOFRollout = glui->add_rollout("Depth of Field", 0);
    DOFRollout->set_w(218);
    glui->add_statictext_to_panel(DOFRollout, "")->set_w(200);

    GLUI_Spinner *lensDiameterSpinner = glui->add_spinner_to_panel(DOFRollout, "Aperture Size ", GLUI_SPINNER_FLOAT, &dofEffect->lensDiameter, -1, gluiCallback);
    lensDiameterSpinner->set_float_limits(0, 25);
    GLUI_Spinner *focalLengthSpinner = glui->add_spinner_to_panel(DOFRollout, "Focal Length ", GLUI_SPINNER_FLOAT, &dofEffect->focalLength, -1, gluiCallback);
    focalLengthSpinner->set_float_limits(0.0, 10);
    GLUI_Spinner *focalDepthSpinner = glui->add_spinner_to_panel(DOFRollout, "Focal Depth ", GLUI_SPINNER_FLOAT, &dofEffect->focalPlaneDepth, -1, gluiCallback);
    focalDepthSpinner->set_float_limits(0.0, 1);


    //Relighting
    GLUI_Rollout *relightingRollout = glui->add_rollout("Relighting", 0);
    relightingRollout->set_w(218);
    glui->add_statictext_to_panel(relightingRollout, "")->set_w(200);
    glui->add_rotation_to_panel(relightingRollout, "Light Direction", relightingEffect->lightDirection, -1, gluiCallback);

    glui->add_statictext_to_panel(relightingRollout, "");
    GLUI_Panel *relightingBasePanel = glui->add_panel_to_panel(relightingRollout, "Base Image");
    GLUI_RadioGroup *relightingBaseRadioGroup = glui->add_radiogroup_to_panel(relightingBasePanel, &relightingEffect->relightingBase, -1, gluiCallback);
    glui->add_radiobutton_to_group(relightingBaseRadioGroup, "Colour");
    glui->add_radiobutton_to_group(relightingBaseRadioGroup, "Reflectance");



    GLUI_Rollout* materialRollout = glui->add_rollout_to_panel(relightingRollout, "Material", 0, GLUI_PANEL_EMBOSSED);

    GLUI_Panel* materialAmbientPanel = glui->add_panel_to_panel(materialRollout, "Ambient", GLUI_PANEL_EMBOSSED);
    GLUI_Spinner *materialAmbientRedSpinner = glui->add_spinner_to_panel(materialAmbientPanel, "Red ", GLUI_SPINNER_FLOAT, &relightingEffect->materialAmbient[0], -1, gluiCallback);
    materialAmbientRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *materialAmbientGreenSpinner = glui->add_spinner_to_panel(materialAmbientPanel, "Green ", GLUI_SPINNER_FLOAT, &relightingEffect->materialAmbient[1], -1, gluiCallback);
    materialAmbientGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *materialAmbientBlueSpinner = glui->add_spinner_to_panel(materialAmbientPanel, "Blue ", GLUI_SPINNER_FLOAT, &relightingEffect->materialAmbient[2], -1, gluiCallback);
    materialAmbientBlueSpinner->set_float_limits(0.0, 1);

    GLUI_Panel* materialDiffusePanel = glui->add_panel_to_panel(materialRollout, "Diffuse", GLUI_PANEL_EMBOSSED);
    GLUI_Spinner *materialDiffuseRedSpinner = glui->add_spinner_to_panel(materialDiffusePanel, "Red ", GLUI_SPINNER_FLOAT, &relightingEffect->materialDiffuse[0], -1, gluiCallback);
    materialDiffuseRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *materialDiffuseGreenSpinner = glui->add_spinner_to_panel(materialDiffusePanel, "Green ", GLUI_SPINNER_FLOAT, &relightingEffect->materialDiffuse[1], -1, gluiCallback);
    materialDiffuseGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *materialDiffuseBlueSpinner = glui->add_spinner_to_panel(materialDiffusePanel, "Blue ", GLUI_SPINNER_FLOAT, &relightingEffect->materialDiffuse[2], -1, gluiCallback);
    materialDiffuseBlueSpinner->set_float_limits(0.0, 1);

    GLUI_Panel* materialSpecularPanel = glui->add_panel_to_panel(materialRollout, "Specular", GLUI_PANEL_EMBOSSED);
    GLUI_Spinner *materialSpecularRedSpinner = glui->add_spinner_to_panel(materialSpecularPanel, "Red ", GLUI_SPINNER_FLOAT, &relightingEffect->materialSpecular[0], -1, gluiCallback);
    materialSpecularRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *materialSpecularGreenSpinner = glui->add_spinner_to_panel(materialSpecularPanel, "Green ", GLUI_SPINNER_FLOAT, &relightingEffect->materialSpecular[1], -1, gluiCallback);
    materialSpecularGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *materialSpecularBlueSpinner = glui->add_spinner_to_panel(materialSpecularPanel, "Blue ", GLUI_SPINNER_FLOAT, &relightingEffect->materialSpecular[2], -1, gluiCallback);
    materialSpecularBlueSpinner->set_float_limits(0.0, 1);

    GLUI_Rollout* lightRollout = glui->add_rollout_to_panel(relightingRollout, "Light", 0, GLUI_PANEL_EMBOSSED);

    GLUI_Panel* lightAmbientPanel = glui->add_panel_to_panel(lightRollout, "Ambient", GLUI_PANEL_EMBOSSED);
    GLUI_Spinner *lightAmbientRedSpinner = glui->add_spinner_to_panel(lightAmbientPanel, "Red ", GLUI_SPINNER_FLOAT, &relightingEffect->lightAmbient[0], -1, gluiCallback);
    lightAmbientRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *lightAmbientGreenSpinner = glui->add_spinner_to_panel(lightAmbientPanel, "Green ", GLUI_SPINNER_FLOAT, &relightingEffect->lightAmbient[1], -1, gluiCallback);
    lightAmbientGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *lightAmbientBlueSpinner = glui->add_spinner_to_panel(lightAmbientPanel, "Blue ", GLUI_SPINNER_FLOAT, &relightingEffect->lightAmbient[2], -1, gluiCallback);
    lightAmbientBlueSpinner->set_float_limits(0.0, 1);

    GLUI_Panel* lightDiffusePanel = glui->add_panel_to_panel(lightRollout, "Diffuse", GLUI_PANEL_EMBOSSED);
    GLUI_Spinner *lightDiffuseRedSpinner = glui->add_spinner_to_panel(lightDiffusePanel, "Red ", GLUI_SPINNER_FLOAT, &relightingEffect->lightDiffuse[0], -1, gluiCallback);
    lightDiffuseRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *lightDiffuseGreenSpinner = glui->add_spinner_to_panel(lightDiffusePanel, "Green ", GLUI_SPINNER_FLOAT, &relightingEffect->lightDiffuse[1], -1, gluiCallback);
    lightDiffuseGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *lightDiffuseBlueSpinner = glui->add_spinner_to_panel(lightDiffusePanel, "Blue ", GLUI_SPINNER_FLOAT, &relightingEffect->lightDiffuse[2], -1, gluiCallback);
    lightDiffuseBlueSpinner->set_float_limits(0.0, 1);

    GLUI_Panel* lightSpecularPanel = glui->add_panel_to_panel(lightRollout, "Specular", GLUI_PANEL_EMBOSSED);
    GLUI_Spinner *lightSpecularRedSpinner = glui->add_spinner_to_panel(lightSpecularPanel, "Red ", GLUI_SPINNER_FLOAT, &relightingEffect->lightSpecular[0], -1, gluiCallback);
    lightSpecularRedSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *lightSpecularGreenSpinner = glui->add_spinner_to_panel(lightSpecularPanel, "Green ", GLUI_SPINNER_FLOAT, &relightingEffect->lightSpecular[1], -1, gluiCallback);
    lightSpecularGreenSpinner->set_float_limits(0.0, 1);
    GLUI_Spinner *lightSpecularBlueSpinner = glui->add_spinner_to_panel(lightSpecularPanel, "Blue ", GLUI_SPINNER_FLOAT, &relightingEffect->lightSpecular[2], -1, gluiCallback);
    lightSpecularBlueSpinner->set_float_limits(0.0, 1);

    glui->add_statictext_to_panel(relightingRollout, "");
    GLUI_Spinner *specularPowerSpinner = glui->add_spinner_to_panel(relightingRollout, "Specular Power ", GLUI_SPINNER_FLOAT, &relightingEffect->specularPower, -1, gluiCallback);
    specularPowerSpinner->set_float_limits(0.0, 10);


    //CARTOON SHADING
    GLUI_Rollout *cartoonRollout = glui->add_rollout("Cartoon Shading", 0);
    cartoonRollout->set_w(218);
    glui->add_statictext_to_panel(cartoonRollout, "")->set_w(200);

    GLUI_Panel *filteringPanel = glui->add_panel_to_panel(cartoonRollout, "Filtering");
    glui->add_statictext_to_panel(filteringPanel, "");
    GLUI_RadioGroup *filteringMethodRadioGroup = glui->add_radiogroup_to_panel(filteringPanel, &cartoonEffect->filteringMethod, -1, gluiCallback);
    glui->add_radiobutton_to_group(filteringMethodRadioGroup, "None");
    glui->add_radiobutton_to_group(filteringMethodRadioGroup, "Bilateral");
    glui->add_radiobutton_to_group(filteringMethodRadioGroup, "Kuwahara");

    glui->add_statictext_to_panel(filteringPanel, "");
    glui->add_checkbox_to_panel(filteringPanel, "Quantize Colours", &cartoonEffect->quantize, -1, gluiCallback);


    //Bilateral.
    glui->add_statictext_to_panel(filteringPanel, "");
    GLUI_Rollout* bilateralRollout = glui->add_rollout_to_panel(filteringPanel, "Bilateral", 0, GLUI_PANEL_EMBOSSED);

    GLUI_Spinner *bilateralFilterPassesSpinner = glui->add_spinner_to_panel(bilateralRollout, "Filter Passes ", GLUI_SPINNER_INT, &cartoonEffect->bilateralFilterPasses, -1, gluiCallback);
    bilateralFilterPassesSpinner->set_int_limits(1, 10);
    GLUI_Spinner *bilateralKernelSizeSpinner = glui->add_spinner_to_panel(bilateralRollout, "Kernel Size ", GLUI_SPINNER_INT, &cartoonEffect->bilateralKernelSize, -1, gluiCallback);
    bilateralKernelSizeSpinner->set_int_limits(1, 100);
    GLUI_Spinner *bilateralSigmaSpinner = glui->add_spinner_to_panel(bilateralRollout, "Sigma ", GLUI_SPINNER_FLOAT, &cartoonEffect->bilateralSigma, -1, gluiCallback);
    bilateralSigmaSpinner->set_float_limits(0.0, 100);

    //Kuwahara.
    GLUI_Rollout* kuwaharaRollout = glui->add_rollout_to_panel(filteringPanel, "Kuwahara", 0, GLUI_PANEL_EMBOSSED);

    GLUI_Spinner *kuwaharaFilterPassesSpinner = glui->add_spinner_to_panel(kuwaharaRollout, "Filter Passes ", GLUI_SPINNER_INT, &cartoonEffect->kuwaharaFilterPasses, -1, gluiCallback);
    kuwaharaFilterPassesSpinner->set_int_limits(1, 10);
    GLUI_Spinner *kuwaharaKernelSizeSpinner = glui->add_spinner_to_panel(kuwaharaRollout, "Kernel Size ", GLUI_SPINNER_INT, &cartoonEffect->kuwaharaKernelSize, -1, gluiCallback);
    kuwaharaKernelSizeSpinner->set_int_limits(1, 100);
    GLUI_Spinner *kuwaharaSigmaSpinner = glui->add_spinner_to_panel(kuwaharaRollout, "Sigma ", GLUI_SPINNER_FLOAT, &cartoonEffect->kuwaharaSigma, -1, gluiCallback);
    kuwaharaSigmaSpinner->set_float_limits(0.0, 1);


    //Edge
    glui->add_statictext_to_panel(cartoonRollout, "");
    GLUI_Panel *edgeDetectionPanel = glui->add_panel_to_panel(cartoonRollout, "Edge Detection");
    glui->add_statictext_to_panel(edgeDetectionPanel, "");
    GLUI_RadioGroup *edgeMethodRadioGroup = glui->add_radiogroup_to_panel(edgeDetectionPanel, &cartoonEffect->edgeMethod, -1, gluiCallback);
    glui->add_radiobutton_to_group(edgeMethodRadioGroup, "None");
    glui->add_radiobutton_to_group(edgeMethodRadioGroup, "Depth Discontinuity");
    glui->add_radiobutton_to_group(edgeMethodRadioGroup, "Sobel Depth");
    glui->add_radiobutton_to_group(edgeMethodRadioGroup, "Sobel Colour");
    glui->add_radiobutton_to_group(edgeMethodRadioGroup, "Radial Valley");




    glui->add_statictext_to_panel(edgeDetectionPanel, "");
    GLUI_Spinner *edgeThresholdSpinner = glui->add_spinner_to_panel(edgeDetectionPanel, "Threshold ", GLUI_SPINNER_FLOAT, &cartoonEffect->edgeThreshold, -1, gluiCallback);
    edgeThresholdSpinner->set_float_limits(0.0, 2.0);


    //glui->add_button("Quit", 0, (GLUI_Update_CB) exit);
    /** Tell GLUI window which other window to recognize as the main gfx window **/
    glui->set_main_gfx_window(window);

    //GLUI_Master.set_glutIdleFunc(NULL);


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


}


//Render the quad to a specified framebuffer

void renderQuad(GLuint fb) {

    glBindFramebuffer(GL_FRAMEBUFFER, fb); //Use specified frame buffer
    glViewport(0, 0, screenWidth, screenHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // bind the VAO
    glBindVertexArray(quadVAO);

    // draw the VAO
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // unbind the VAO
    glBindVertexArray(0);

}

//Fill a texture from a bmp file

void loadBMP(const char * filename, GLuint textureID) {

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

    if (useEffects) {
        //Delete effects (and effect specific resources)
        delete fogEffect;
        delete relightingEffect;
        delete dofEffect;
        delete cartoonEffect;
    }

    //Delete textures
    glDeleteTextures(1, &colourTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteTextures(1, &normalTexture);

    //Delete arrays and buffers
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
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

void gluiCallback(int control) {

}

void displayMaps() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, screenWidth, screenHeight);


    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 0);
    glTexCoord2f(1, 0);
    glVertex3f(0, -1, 0);
    glTexCoord2f(1, 1);
    glVertex3f(0, 1, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(0, -1, 0);
    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 0);
    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, 1, 0);
    glEnd();


    //Swap the back and front buffer
    glutSwapBuffers();
}

