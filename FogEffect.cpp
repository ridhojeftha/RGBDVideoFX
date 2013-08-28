
#include "FogEffect.h"
#include "Renderer.h"

FogEffect::FogEffect() {

    fogDensity = 0.5;
    fogColourRed = 1;
    fogColourGreen = 1;
    fogColourBlue = 1;

    fogShader = new Shader("fog.frag");
}

void FogEffect::display() {

    // bind the program (the shaders)
    glUseProgram(fogShader->id());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(fogShader->uniform("depthTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glUniform1i(fogShader->uniform("colourTexture"), 1);

    glUniform1f(fogShader->uniform("fogDensity"), fogDensity);
    glUniform3f(fogShader->uniform("fogColour"), fogColourRed, fogColourGreen, fogColourBlue);


    renderQuad(0);
}



FogEffect::~FogEffect() {
    delete fogShader;
}

