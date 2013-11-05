#include "DOFEffect.h"
#include "Renderer.h"

DOFEffect::DOFEffect() {

    lensDiameter = 0.5;
    focalLength = 0.25;
    focalPlaneDepth = 0.5;

    preblurFilterPasses = 0;
    preblurKernelSize = 5;
    preblurSigma = 0.02;

    preblurHShader = new Shader("preblurH.frag");
    preblurVShader = new Shader("preblurV.frag");

    intermdiateBuffer = new FrameBuffer(screenWidth, screenHeight);

    distributionSigma = 0.02;
    sampleRadius = 20;

    VBlurShader = new Shader("VBlur.frag");
    HBlurShader = new Shader("HBlur.frag");
    blendShader = new Shader("DOFBlend.frag");

    testShader = new Shader("CoC.frag");


    blurredBuffer = new FrameBuffer(screenWidth, screenHeight);


}

void DOFEffect::resize() {
    intermdiateBuffer->resize();
    blurredBuffer->resize();
}

void DOFEffect::display() {



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
        renderQuad(blurredBuffer->fbo());
        depthTexInput = blurredBuffer->texture();

        //VERTICAL
        glUseProgram(preblurVShader->id());
        glUniform1i(preblurVShader->uniform("imageHeight"), inputHeight);
        glUniform1i(preblurVShader->uniform("sampleRadius"), preblurKernelSize);
        glUniform1f(preblurVShader->uniform("distributionSigma"), preblurSigma);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexInput);
        glUniform1i(preblurVShader->uniform("colourTexture"), 0);


        glUniform1i(preblurVShader->uniform("flipCoords"), false);
        renderQuad(blurredBuffer->fbo());

    }



    glUseProgram(HBlurShader->id());


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glUniform1i(HBlurShader->uniform("colourTexture"), 0);


    glUniform1i(HBlurShader->uniform("imageWidth"), inputWidth);
    glUniform1f(HBlurShader->uniform("distributionSigma"), distributionSigma);
    glUniform1i(HBlurShader->uniform("sampleRadius"), sampleRadius);

    glUniform1i(HBlurShader->uniform("flipCoords"), false);
    renderQuad(intermdiateBuffer->fbo());

    glUseProgram(VBlurShader->id());



    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glUniform1i(VBlurShader->uniform("colourTexture"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexInput);
    glUniform1i(VBlurShader->uniform("depthTexture"), 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, intermdiateBuffer->texture());
    glUniform1i(VBlurShader->uniform("blurredTexture"), 2);

    glUniform1i(VBlurShader->uniform("imageHeight"), inputHeight);
    glUniform1f(VBlurShader->uniform("distributionSigma"), distributionSigma);
    glUniform1i(VBlurShader->uniform("sampleRadius"), sampleRadius);
    glUniform1f(VBlurShader->uniform("focalLength"), focalLength);
    glUniform1f(VBlurShader->uniform("focalPlaneDepth"), focalPlaneDepth);
    glUniform1f(VBlurShader ->uniform("lensDiameter"), lensDiameter);


    glUniform1i(VBlurShader->uniform("flipCoords"), useKinect);
    renderQuad(0);




}

DOFEffect::~DOFEffect() {
    delete intermdiateBuffer;
    delete HBlurShader;
    delete VBlurShader;
}

