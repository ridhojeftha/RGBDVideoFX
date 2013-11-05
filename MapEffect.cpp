
#include "MapEffect.h"
#include "Renderer.h"

MapEffect::MapEffect() {

    selectedMap = 1;
    selectedPreFilter = 0;

    textureShader = new Shader("texture.frag");

    preblurFilterPasses = 0;
    preblurKernelSize = 5;
    preblurSigma = 0.02;

    preblurHShader = new Shader("preblurH.frag");
    preblurVShader = new Shader("preblurV.frag");
    HBilateralShader = new Shader("HBilateral.frag");
    VBilateralShader = new Shader("VBilateral.frag");

    intermdiateBuffer = new FrameBuffer(screenWidth, screenHeight);
}
void MapEffect::resize() {
    intermdiateBuffer->resize();
 
}

void MapEffect::display() {

    GLuint colourTexInput;

    switch (selectedMap) {
        case 0:
            colourTexInput = colourTexture;
            break;
        case 1:
            colourTexInput = depthTexture;
            break;
        case 2:
            colourTexInput = normalTexture;
            break;
    }



    for (int i = 0; i < preblurFilterPasses; i++) {

        if (selectedPreFilter == 1) {
            //HORIZONTAL
            glUseProgram(preblurHShader->id());
            glUniform1i(preblurHShader->uniform("imageWidth"), inputWidth);
            glUniform1i(preblurHShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(preblurHShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colourTexInput);
            glUniform1i(preblurHShader->uniform("colourTexture"), 0);


            glUniform1i(preblurHShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
            colourTexInput = intermdiateBuffer->texture();

            //VERTICAL
            glUseProgram(preblurVShader->id());
            glUniform1i(preblurVShader->uniform("imageHeight"), inputHeight);
            glUniform1i(preblurVShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(preblurVShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colourTexInput);
            glUniform1i(preblurVShader->uniform("colourTexture"), 0);


            glUniform1i(preblurVShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());

        } else if (selectedPreFilter == 2) {
            //HORIZONTAL
            glUseProgram(HBilateralShader->id());
            glUniform1i(HBilateralShader->uniform("imageWidth"), inputWidth);
            glUniform1i(HBilateralShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(HBilateralShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            glUniform1i(HBilateralShader->uniform("depthTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, colourTexInput);
            glUniform1i(HBilateralShader->uniform("colourTexture"), 1);

            glUniform1i(HBilateralShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
            colourTexInput = intermdiateBuffer->texture();

            //VERTICAL
            glUseProgram(VBilateralShader->id());
            glUniform1i(VBilateralShader->uniform("imageHeight"), inputHeight);
            glUniform1i(VBilateralShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(VBilateralShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            glUniform1i(VBilateralShader->uniform("depthTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, colourTexInput);
            glUniform1i(VBilateralShader->uniform("colourTexture"), 1);

            glUniform1i(VBilateralShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());

        }

    }





    glUseProgram(textureShader->id());

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, colourTexInput);

    glUniform1i(textureShader->uniform("colourTexture"), 0);

    glUniform1i(textureShader->uniform("flipCoords"), useKinect);
    renderQuad(0);
}

MapEffect::~MapEffect() {
    delete textureShader;
}


