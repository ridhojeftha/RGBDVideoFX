
#include "Main.h"
#include "Kinect.h"
#include "Renderer.h"

int main(int argc, char** argv) {


    if (useKinect) {
        inputWidth = 640;
        inputHeight = 480;
    } else {
        inputWidth = 720;
        inputHeight = 540;
    }


    //Intialise window and effects
    if (useEffects) {

        initRenderer(argc, argv, inputWidth, inputHeight);
    } else {

        initRenderer(argc, argv, inputWidth * 2, inputHeight);
    }


    if (useKinect) {

        startKinect();

    } else {

        loadTextureFiles();

        glBindTexture(GL_TEXTURE_2D, colourTexture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &inputWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &inputHeight);

    }




    //Bind window callback functions
    glutCloseFunc(&close);
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&update);
    GLUI_Master.set_glutIdleFunc(&idle);

    //Begin looping
    glutMainLoop();

    return EXIT_SUCCESS;
}

void close() {

    if (useKinect) {
        stopKinect();
    }

    //Dispose of openGL resources
    cleanupRenderer();

    exit(EXIT_SUCCESS);
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case '\x1B':
            close();
            break;

        case 'b':
            //start timer
            if (!benchmarking) {
                benchmarking = true;
                std::cout << "Benchmark Started...\n";
                frameCounter = 0;
                startTime = timeGetTime();
            }
            break;
    }
}

void update() {

    if (useKinect) {
        updateKinect();
    }

    if (benchmarking) {
        float milliseconds = timeGetTime() - startTime;

        if (milliseconds >= 10000) {
            //Print performance
            std::cout << "Frames: " << frameCounter << "\n";
            std::cout << "Time: " << milliseconds << "ms\n";
            std::cout << "FPS: " << frameCounter / (milliseconds / 1000) << "\n";
            benchmarking = false;
        }
    } else {

        if (frameCounter % 10 == 0) {
            float fps = 10.0 / (timeGetTime() - tenFrameTime)*1000;
            fps = floor(fps + 0.5);
            std::ostringstream ss;
            ss << "RGBD - FPS: " << fps;
            std::string s(ss.str());
            glutSetWindowTitle(s.c_str());
            tenFrameTime = timeGetTime();
        }

    }

    if (useEffects) {
        displayEffects();
    } else {
        displayMaps();
    }
    frameCounter++;

    fflush(stderr);
    fflush(stdout);




}

//Display as often as possible

void idle() {
    if (glutGetWindow() != window)
        glutSetWindow(window);
    glutPostRedisplay();
}

void loadTextureFiles() {


    //Load textures from files
    loadBMP("Images/teapot_normals.bmp", normalTexture);
    loadBMP("Images/teapot_reflectance.bmp", reflectanceTexture);


    //  loadBMP("Images/teapot_colour.bmp", colourTexture);
    //   loadBMP("Images/teapot_depth.bmp", depthTexture);

    //   loadBMP("Images/dofpro_billiardsRGB.bmp", colourTexture);
    //  loadBMP("Images/dofpro_billiardsDM.bmp", depthTexture);


    //   loadBMP("Images/dofpro_diet_pepsiRGB.bmp", colourTexture);
    //  loadBMP("Images/dofpro_diet_pepsiDM.bmp", depthTexture);

    loadBMP("Images/dofpro_teacupRGB.bmp", colourTexture);
    loadBMP("Images/dofpro_teacupDM.bmp", depthTexture);

    // loadBMP("Images/dofpro_chessRGB.bmp", colourTexture);
    // loadBMP("Images/dofpro_chessDM.bmp", depthTexture);

    // loadBMP("Images/dofpro_chairsRGB.bmp", colourTexture);
    //  loadBMP("Images/dofpro_chairsDM.bmp", depthTexture);

    //   loadBMP("Images/bird_RGB.bmp", colourTexture);
    //   loadBMP("Images/bird_DM.bmp", depthTexture);

    //  loadBMP("Images/child_RGB.bmp", colourTexture);
    // loadBMP("Images/child_DM.bmp", depthTexture);
}

