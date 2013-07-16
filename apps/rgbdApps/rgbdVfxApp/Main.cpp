#include "Main.h"
#include "Renderer.h"

int main(int argc, char** argv) {

    //Get ticks per second
    QueryPerformanceFrequency(&frequency);

    //Intialise window and effects
    initRenderer(argc, argv,800,600);

    //Load textures from files
    fillTextureFromBMP("teapot_colour.bmp",colourTexture);
    fillTextureFromBMP("teapot_depth.bmp",depthTexture);
    fillTextureFromBMP("teapot_normals.bmp",normalTexture);

    //Bind window callback functions
    glutCloseFunc(&close);
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&update);
  
    //Begin looping
    glutMainLoop();

    return EXIT_SUCCESS;
}

void close() {
    
    //Dispose of openGL resources
    cleanupRenderer();

    //Print performance
    std::cout << "Frames: " << frameCounter << "\n";
    std::cout << "Time: " << totalTime << "ms\n";
    std::cout << "FPS: " << frameCounter / totalTime * 1000.0 << "\n";

    exit(EXIT_SUCCESS);
}

void keyboard(unsigned char key, int x, int y) {

    rendererKeyboard(key);
    
    switch (key) {
        case '\x1B':
            close();
            break;
    }
}

void update() {

    //start timer
    QueryPerformanceCounter(&startTime);


    
    displayEffects();

    //stop timer
    QueryPerformanceCounter(&endTime);

    // compute elapsed time in milliseconds
    totalTime += (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

    frameCounter++;

    fflush(stderr);
    fflush(stdout);

    //Display as often as possible
    glutPostRedisplay();
}