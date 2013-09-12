#include "DOFEffect.h"
#include "Renderer.h"

DOFEffect::DOFEffect() {

    lensDiameter = 5;
    focalLength = 1;
    focalPlaneDepth = 0.5;
    intensity=1;

    VBlurShader = new Shader("VBlur.frag");
    HBlurShader = new Shader("HBlur.frag");

    intermdiateBuffer = new FrameBuffer(screenWidth, screenHeight);
}

void DOFEffect::display() {
    



       glUseProgram(HBlurShader->id());
       
       glUniform1i(HBlurShader->uniform("imageWidth"), inputWidth);

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, depthTexture);
       glUniform1i(HBlurShader->uniform("depthTexture"), 0);

       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D, colourTexture);
       glUniform1i(HBlurShader->uniform("colourTexture"), 1);

       glUniform1f(HBlurShader->uniform("focalLength"), focalLength);
       glUniform1f(HBlurShader->uniform("focalPlaneDepth"), focalPlaneDepth);
       glUniform1f(HBlurShader->uniform("lensDiameter"), lensDiameter);
       glUniform1f(HBlurShader->uniform("intensityConstant"), intensity);

       renderQuad(intermdiateBuffer->fbo());

       glUseProgram(VBlurShader->id());
       glUniform1i(VBlurShader->uniform("imageHeight"), inputHeight);
       

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, depthTexture);
       glUniform1i(VBlurShader->uniform("depthTexture"), 0);

       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D, intermdiateBuffer->texture());
       glUniform1i(VBlurShader->uniform("colourTexture"), 1);

       glUniform1f(VBlurShader->uniform("focalLength"), focalLength);
       glUniform1f(VBlurShader->uniform("focalPlaneDepth"), focalPlaneDepth);
       glUniform1f(VBlurShader ->uniform("lensDiameter"), lensDiameter);
       glUniform1f(VBlurShader->uniform("intensityConstant"), intensity);

       renderQuad(0);
   
     

}



DOFEffect::~DOFEffect() {
    delete intermdiateBuffer;
    delete HBlurShader;
    delete VBlurShader;
}

