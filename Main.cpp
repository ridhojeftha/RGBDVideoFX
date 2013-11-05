
#include "Main.h"
#include "VideoCapture.h"
#include "Renderer.h"
#include "Effect.h"
//#include "NormalMapGenerator.h"

int main(int argc, char** argv) {


    inputWidth = 640;
    inputHeight = 480;



    //Intialise window and effects
    if (useEffects) {
        initRenderer(argc, argv, inputWidth, inputHeight);
    } else {
        initRenderer(argc, argv, inputWidth * 2, inputHeight * 2);
    }




    if (useKinect) {

        startVideoCapture();

    } else {

        loadTextureFiles();

        if (useEffects) {
            glBindTexture(GL_TEXTURE_2D, colourTexture);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &inputWidth);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &inputHeight);
            screenWidth = inputWidth;
            screenHeight = inputHeight;
            glutReshapeWindow(inputWidth, inputHeight);
            resizeEffects();
        }

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
        stopVideoCapture();
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
        case 'm':

            if (currentMap == colourTexture) {

                currentMap = depthTexture;
            } else if (currentMap == depthTexture) {

                currentMap = normalTexture;
            } else {

                currentMap = colourTexture;

            }
            break;

        case 's':
            selectedTextureFile++;
            if (selectedTextureFile == rgbTextureFiles.size()) {
                selectedTextureFile = 0;
            }
            loadBMP(rgbTextureFiles[selectedTextureFile].c_str(), colourTexture);
            loadBMP(depthTextureFiles[selectedTextureFile].c_str(), depthTexture);
            loadBMP(normalTextureFiles[selectedTextureFile].c_str(), normalTexture);

            if (useEffects) {
                glBindTexture(GL_TEXTURE_2D, colourTexture);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &inputWidth);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &inputHeight);
                screenWidth = inputWidth;
                screenHeight = inputHeight;
                glutReshapeWindow(inputWidth, inputHeight);
                resizeEffects();
            }

            break;

        case 'a':
            selectedTextureFile--;
            if (selectedTextureFile < 0) {
                selectedTextureFile = rgbTextureFiles.size() - 1;
            }
            loadBMP(rgbTextureFiles[selectedTextureFile].c_str(), colourTexture);
            loadBMP(depthTextureFiles[selectedTextureFile].c_str(), depthTexture);
            loadBMP(normalTextureFiles[selectedTextureFile].c_str(), normalTexture);

            if (useEffects) {
                glBindTexture(GL_TEXTURE_2D, colourTexture);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &inputWidth);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &inputHeight);
                screenWidth = inputWidth;
                screenHeight = inputHeight;
                glutReshapeWindow(inputWidth, inputHeight);
                resizeEffects();
            }

            break;

    }
}

void update() {

    if (useKinect) {
        updateVideoCapture();
    }

    if (benchmarking) {
        float milliseconds = timeGetTime() - startTime;
        std::ostringstream ss;
        ss << "RGBD - Benchmarking: " << floor(milliseconds / 1000.0);
        std::string s(ss.str());
        glutSetWindowTitle(s.c_str());

        if (milliseconds >= 300000.0) {
            //Print performance
            std::cout << "Frames: " << frameCounter << "\n";
            std::cout << "Time: " << milliseconds << "ms\n";
            std::cout << "FPS: " << frameCounter / (milliseconds / 1000.0) << "\n";
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



    rgbTextureFiles.push_back("Images/rgb/1.bmp");
    depthTextureFiles.push_back("Images/depth/1.bmp");
    normalTextureFiles.push_back("Images/normals/1.bmp");

    rgbTextureFiles.push_back("Images/rgb/2.bmp");
    depthTextureFiles.push_back("Images/depth/2.bmp");
    normalTextureFiles.push_back("Images/normals/2.bmp");

    rgbTextureFiles.push_back("Images/rgb/3.bmp");
    depthTextureFiles.push_back("Images/depth/3.bmp");
    normalTextureFiles.push_back("Images/normals/3.bmp");

    rgbTextureFiles.push_back("Images/rgb/4.bmp");
    depthTextureFiles.push_back("Images/depth/4.bmp");
    normalTextureFiles.push_back("Images/normals/4.bmp");

    rgbTextureFiles.push_back("Images/rgb/5.bmp");
    depthTextureFiles.push_back("Images/depth/5.bmp");
    normalTextureFiles.push_back("Images/normals/5.bmp");

    rgbTextureFiles.push_back("Images/rgb/6.bmp");
    depthTextureFiles.push_back("Images/depth/6.bmp");
    normalTextureFiles.push_back("Images/normals/6.bmp");

    rgbTextureFiles.push_back("Images/rgb/7.bmp");
    depthTextureFiles.push_back("Images/depth/7.bmp");
    normalTextureFiles.push_back("Images/normals/7.bmp");

    rgbTextureFiles.push_back("Images/rgb/8.bmp");
    depthTextureFiles.push_back("Images/depth/8.bmp");
    normalTextureFiles.push_back("Images/normals/8.bmp");

    rgbTextureFiles.push_back("Images/rgb/9.bmp");
    depthTextureFiles.push_back("Images/depth/9.bmp");
    normalTextureFiles.push_back("Images/normals/9.bmp");

    rgbTextureFiles.push_back("Images/rgb/10.bmp");
    depthTextureFiles.push_back("Images/depth/10.bmp");
    normalTextureFiles.push_back("Images/normals/10.bmp");

    rgbTextureFiles.push_back("Images/rgb/11.bmp");
    depthTextureFiles.push_back("Images/depth/11.bmp");
    normalTextureFiles.push_back("Images/normals/11.bmp");

    rgbTextureFiles.push_back("Images/rgb/12.bmp");
    depthTextureFiles.push_back("Images/depth/12.bmp");
    normalTextureFiles.push_back("Images/normals/12.bmp");

    rgbTextureFiles.push_back("Images/rgb/13.bmp");
    depthTextureFiles.push_back("Images/depth/13.bmp");
    normalTextureFiles.push_back("Images/normals/13.bmp");

    rgbTextureFiles.push_back("Images/rgb/14.bmp");
    depthTextureFiles.push_back("Images/depth/14.bmp");
    normalTextureFiles.push_back("Images/normals/14.bmp");

    rgbTextureFiles.push_back("Images/rgb/15.bmp");
    depthTextureFiles.push_back("Images/depth/15.bmp");
    normalTextureFiles.push_back("Images/normals/15.bmp");

    rgbTextureFiles.push_back("Images/rgb/16.bmp");
    depthTextureFiles.push_back("Images/depth/16.bmp");
    normalTextureFiles.push_back("Images/normals/16.bmp");

    rgbTextureFiles.push_back("Images/rgb/17.bmp");
    depthTextureFiles.push_back("Images/depth/17.bmp");
    normalTextureFiles.push_back("Images/normals/17.bmp");

    rgbTextureFiles.push_back("Images/rgb/18.bmp");
    depthTextureFiles.push_back("Images/depth/18.bmp");
    normalTextureFiles.push_back("Images/normals/18.bmp");

    rgbTextureFiles.push_back("Images/rgb/19.bmp");
    depthTextureFiles.push_back("Images/depth/19.bmp");
    normalTextureFiles.push_back("Images/normals/19.bmp");

    rgbTextureFiles.push_back("Images/rgb/20.bmp");
    depthTextureFiles.push_back("Images/depth/20.bmp");
    normalTextureFiles.push_back("Images/normals/20.bmp");

    rgbTextureFiles.push_back("Images/rgb/21.bmp");
    depthTextureFiles.push_back("Images/depth/21.bmp");
    normalTextureFiles.push_back("Images/normals/21.bmp");

    rgbTextureFiles.push_back("Images/rgb/22.bmp");
    depthTextureFiles.push_back("Images/depth/22.bmp");
    normalTextureFiles.push_back("Images/normals/22.bmp");

    rgbTextureFiles.push_back("Images/rgb/23.bmp");
    depthTextureFiles.push_back("Images/depth/23.bmp");
    normalTextureFiles.push_back("Images/normals/23.bmp");

    rgbTextureFiles.push_back("Images/rgb/24.bmp");
    depthTextureFiles.push_back("Images/depth/24.bmp");
    normalTextureFiles.push_back("Images/normals/24.bmp");

    rgbTextureFiles.push_back("Images/rgb/25.bmp");
    depthTextureFiles.push_back("Images/depth/25.bmp");
    normalTextureFiles.push_back("Images/normals/25.bmp");

    rgbTextureFiles.push_back("Images/rgb/26.bmp");
    depthTextureFiles.push_back("Images/depth/2.bmp");
    normalTextureFiles.push_back("Images/normals/2.bmp");

    rgbTextureFiles.push_back("Images/rgb/27.bmp");
    depthTextureFiles.push_back("Images/depth/2.bmp");
    normalTextureFiles.push_back("Images/normals/2.bmp");

    rgbTextureFiles.push_back("Images/rgb/28.bmp");
    depthTextureFiles.push_back("Images/depth/28.bmp");
    normalTextureFiles.push_back("Images/normals/28.bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (1).bmp");
    depthTextureFiles.push_back("Images/depth/depth (1).bmp");
    normalTextureFiles.push_back("Images/normals/normals (1).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (2).bmp");
    depthTextureFiles.push_back("Images/depth/depth (2).bmp");
    normalTextureFiles.push_back("Images/normals/normals (2).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (3).bmp");
    depthTextureFiles.push_back("Images/depth/depth (3).bmp");
    normalTextureFiles.push_back("Images/normals/normals (3).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (4).bmp");
    depthTextureFiles.push_back("Images/depth/depth (4).bmp");
    normalTextureFiles.push_back("Images/normals/normals (4).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (5).bmp");
    depthTextureFiles.push_back("Images/depth/depth (5).bmp");
    normalTextureFiles.push_back("Images/normals/normals (5).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (6).bmp");
    depthTextureFiles.push_back("Images/depth/depth (6).bmp");
    normalTextureFiles.push_back("Images/normals/normals (6).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (7).bmp");
    depthTextureFiles.push_back("Images/depth/depth (7).bmp");
    normalTextureFiles.push_back("Images/normals/normals (7).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (8).bmp");
    depthTextureFiles.push_back("Images/depth/depth (8).bmp");
    normalTextureFiles.push_back("Images/normals/normals (8).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (8).bmp");
    depthTextureFiles.push_back("Images/depth/depth (8).bmp");
    normalTextureFiles.push_back("Images/normals/normals (8).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (9).bmp");
    depthTextureFiles.push_back("Images/depth/depth (9).bmp");
    normalTextureFiles.push_back("Images/normals/normals (9).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (9).bmp");
    depthTextureFiles.push_back("Images/depth/depth (9).bmp");
    normalTextureFiles.push_back("Images/normals/normals (9).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (10).bmp");
    depthTextureFiles.push_back("Images/depth/depth (10).bmp");
    normalTextureFiles.push_back("Images/normals/normals (10).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (11).bmp");
    depthTextureFiles.push_back("Images/depth/depth (11).bmp");
    normalTextureFiles.push_back("Images/normals/normals (11).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (12).bmp");
    depthTextureFiles.push_back("Images/depth/depth (12).bmp");
    normalTextureFiles.push_back("Images/normals/normals (12).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (13).bmp");
    depthTextureFiles.push_back("Images/depth/depth (13).bmp");
    normalTextureFiles.push_back("Images/normals/normals (13).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (14).bmp");
    depthTextureFiles.push_back("Images/depth/depth (14).bmp");
    normalTextureFiles.push_back("Images/normals/normals (14).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (15).bmp");
    depthTextureFiles.push_back("Images/depth/depth (15).bmp");
    normalTextureFiles.push_back("Images/normals/normals (15).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (16).bmp");
    depthTextureFiles.push_back("Images/depth/depth (16).bmp");
    normalTextureFiles.push_back("Images/normals/normals (16).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (17).bmp");
    depthTextureFiles.push_back("Images/depth/depth (17).bmp");
    normalTextureFiles.push_back("Images/normals/normals (17).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (18).bmp");
    depthTextureFiles.push_back("Images/depth/depth (18).bmp");
    normalTextureFiles.push_back("Images/normals/normals (18).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (19).bmp");
    depthTextureFiles.push_back("Images/depth/depth (19).bmp");
    normalTextureFiles.push_back("Images/normals/normals (19).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (20).bmp");
    depthTextureFiles.push_back("Images/depth/depth (20).bmp");
    normalTextureFiles.push_back("Images/normals/normals (20).bmp");

    rgbTextureFiles.push_back("Images/rgb/colour (21).bmp");
    depthTextureFiles.push_back("Images/depth/depth (21).bmp");
    normalTextureFiles.push_back("Images/normals/normals (21).bmp");


    loadBMP(rgbTextureFiles[selectedTextureFile].c_str(), colourTexture);
    loadBMP(depthTextureFiles[selectedTextureFile].c_str(), depthTexture);
    loadBMP(normalTextureFiles[selectedTextureFile].c_str(), normalTexture);
}

