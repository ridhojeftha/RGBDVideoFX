
#include "FogEffect.h"
#include "Renderer.h"

FogEffect::FogEffect() {

    preblurFilterPasses = 0;
    preblurKernelSize = 5;
    preblurSigma = 0.02;

    fogAmount = 0.5;
    fogDensity = 2.0;
    fogColourRed = 1;
    fogColourGreen = 1;
    fogColourBlue = 1;

    preblurVShader = new Shader("preblurV.frag");
    preblurHShader = new Shader("preblurH.frag");
    fogShader = new Shader("fog.frag");
    intermdiateBuffer = new FrameBuffer(screenWidth, screenHeight, GL_RGB);
}

void FogEffect::display() {

    GLuint depthTexInput = depthTexture;

    for (int i = 0; i < preblurFilterPasses; i++) {


        //HORIZONTAL
        glUseProgram(preblurHShader->id());
        glUniform1i(preblurHShader->uniform("imageWidth"), inputWidth);
        glUniform1i(preblurHShader->uniform("sampleRadius"), preblurKernelSize);
        glUniform1f(preblurHShader->uniform("distributionSigma"), preblurSigma);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexInput);
        glUniform1i(preblurHShader->uniform("colourTexture"), 0);


        glUniform1i(preblurHShader->uniform("flipCoords"), false);
        renderQuad(intermdiateBuffer->fbo());
        depthTexInput = intermdiateBuffer->texture();

        //VERTICAL
        glUseProgram(preblurVShader->id());
        glUniform1i(preblurVShader->uniform("imageHeight"), inputHeight);
        glUniform1i(preblurVShader->uniform("sampleRadius"), preblurKernelSize);
        glUniform1f(preblurVShader->uniform("distributionSigma"), preblurSigma);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexInput);
        glUniform1i(preblurVShader->uniform("colourTexture"), 0);


        glUniform1i(preblurVShader->uniform("flipCoords"), false);
        renderQuad(intermdiateBuffer->fbo());


    }


    // bind the program (the shaders)
    glUseProgram(fogShader->id());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexInput);
    glUniform1i(fogShader->uniform("depthTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glUniform1i(fogShader->uniform("colourTexture"), 1);

    glUniform1f(fogShader->uniform("fogAmount"), fogAmount);
    glUniform1f(fogShader->uniform("fogDensity"), fogDensity);
    glUniform3f(fogShader->uniform("fogColour"), fogColourRed, fogColourGreen, fogColourBlue);

    glUniform1i(fogShader->uniform("flipCoords"), useKinect);
    renderQuad(0);

}

FogEffect::~FogEffect() {
    delete fogShader;
}

