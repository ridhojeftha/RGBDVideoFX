#include "CartoonEffect.h"
#include "Renderer.h"

CartoonEffect::CartoonEffect() {
    HBilateralShader = new Shader("HBilateral.frag");
    VBilateralShader = new Shader("VBilateral.frag");
    textureShader = new Shader("texture.frag");
    quantizationShader = new Shader("quantization.frag");

    kuwaharaShader = new Shader("kuwahara.frag");
    edgeShader = new Shader("depthdiscontinuity.frag");
    sobelShader = new Shader("sobel.frag");
    radialValleyShader = new Shader("radialvalley.frag");
    intermdiateBuffer = new FrameBuffer(screenWidth, screenHeight);

    bilateralFilterPasses = 2;
    bilateralKernelSize = 15;
    bilateralSigma = 0.02;

    kuwaharaFilterPasses = 2;
    kuwaharaKernelSize = 5;


    edgeThreshold = 0.1;

    edgeMethod = 2;
    filteringMethod = 1;
    quantize = 0;

}

void CartoonEffect::resize() {
    intermdiateBuffer->resize();
}

void CartoonEffect::display() {




    GLuint colourTexInput = colourTexture;

    //BILATERAL
    if (filteringMethod == 1) {
        for (int i = 0; i < bilateralFilterPasses; i++) {

            //HORIZONTAL
            glUseProgram(HBilateralShader->id());
            glUniform1i(HBilateralShader->uniform("imageWidth"), inputWidth);
            glUniform1i(HBilateralShader->uniform("sampleRadius"), bilateralKernelSize);
            glUniform1f(HBilateralShader->uniform("distributionSigma"), bilateralSigma);

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
            glUniform1i(VBilateralShader->uniform("sampleRadius"), bilateralKernelSize);
            glUniform1f(VBilateralShader->uniform("distributionSigma"), bilateralSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            glUniform1i(VBilateralShader->uniform("depthTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, colourTexInput);
            glUniform1i(VBilateralShader->uniform("colourTexture"), 1);

            if (edgeMethod == 0 && quantize == 0 && i == bilateralFilterPasses - 1) {
                glUniform1i(VBilateralShader->uniform("flipCoords"), useKinect);
                renderQuad(0);
            } else {
                glUniform1i(VBilateralShader->uniform("flipCoords"), false);
                renderQuad(intermdiateBuffer->fbo());
            }
        }
    }



    //KUWAHARA
    if (filteringMethod == 2) {
        colourTexInput = colourTexture;

        for (int i = 0; i < kuwaharaFilterPasses; i++) {

            glUseProgram(kuwaharaShader->id());

            glUniform1i(kuwaharaShader->uniform("imageHeight"), inputHeight);
            glUniform1i(kuwaharaShader->uniform("imageWidth"), inputWidth);
            glUniform1i(kuwaharaShader->uniform("kernelSize"), kuwaharaKernelSize);    

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            glUniform1i(kuwaharaShader->uniform("depthTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, colourTexInput);
            glUniform1i(kuwaharaShader->uniform("colourTexture"), 1);

            if (edgeMethod == 0 && i == kuwaharaFilterPasses - 1 && quantize == 0) {
                glUniform1i(kuwaharaShader->uniform("flipCoords"), useKinect);
                renderQuad(0);
            } else {
                glUniform1i(kuwaharaShader->uniform("flipCoords"), false);
                renderQuad(intermdiateBuffer->fbo());

            }

            colourTexInput = intermdiateBuffer->texture();


        }


    }


    //Depth discontiuity
    if (edgeMethod == 1) {

        glUseProgram(edgeShader->id());

        glUniform1i(edgeShader->uniform("imageHeight"), inputHeight);
        glUniform1i(edgeShader->uniform("imageWidth"), inputWidth);
        glUniform1f(edgeShader->uniform("threshold"), edgeThreshold);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(edgeShader->uniform("edgeTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colourTexInput);
        glUniform1i(edgeShader->uniform("colourTexture"), 1);

        if (quantize == 0) {
             glUniform1i(edgeShader->uniform("flipCoords"), useKinect);
            renderQuad(0);
        } else {
            glUniform1i(edgeShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
        }
    }


    //Sobel depth
    if (edgeMethod == 2) {

        glUseProgram(sobelShader->id());

        glUniform1i(sobelShader->uniform("imageHeight"), inputHeight);
        glUniform1i(sobelShader->uniform("imageWidth"), inputWidth);
        glUniform1f(sobelShader->uniform("threshold"), edgeThreshold);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(sobelShader->uniform("edgeTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colourTexInput);
        glUniform1i(sobelShader->uniform("colourTexture"), 1);

        if (quantize == 0) {
             glUniform1i(sobelShader->uniform("flipCoords"), useKinect);
            renderQuad(0);
        } else {
            glUniform1i(sobelShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
        }

    }

    //Sobel colour
    if (edgeMethod == 3) {

        glUseProgram(sobelShader->id());

        glUniform1i(sobelShader->uniform("imageHeight"), inputHeight);
        glUniform1i(sobelShader->uniform("imageWidth"), inputWidth);
        glUniform1f(sobelShader->uniform("threshold"), edgeThreshold);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colourTexInput);
        glUniform1i(sobelShader->uniform("edgeTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colourTexInput);
        glUniform1i(sobelShader->uniform("colourTexture"), 1);

        if (quantize == 0) {
            glUniform1i(sobelShader->uniform("flipCoords"), useKinect);
            renderQuad(0);
        } else {
            glUniform1i(sobelShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
        }

    }
    
     //Depth discontiuity
    if (edgeMethod == 4) {

        glUseProgram(radialValleyShader->id());

        glUniform1i(radialValleyShader->uniform("imageHeight"), inputHeight);
        glUniform1i(radialValleyShader->uniform("imageWidth"), inputWidth);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colourTexInput);
        glUniform1i(radialValleyShader->uniform("colourTexture"), 1);

        if (quantize == 0) {
            glUniform1i(radialValleyShader->uniform("flipCoords"), useKinect);
            renderQuad(0);
        } else {
            glUniform1i(radialValleyShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
        }
    }
    

    if (quantize == 1) {


        glUseProgram(quantizationShader->id());

        glActiveTexture(GL_TEXTURE0);
        if ((edgeMethod == 0) && (filteringMethod == 0)) {
            glBindTexture(GL_TEXTURE_2D, colourTexture);
        } else {
            glBindTexture(GL_TEXTURE_2D, intermdiateBuffer->texture());
        }
        glUniform1i(quantizationShader->uniform("colourTexture"), 0);

         glUniform1i(quantizationShader->uniform("flipCoords"), useKinect);
        renderQuad(0);
    } else if ((edgeMethod == 0) && (filteringMethod == 0)) {

        glUseProgram(textureShader->id());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colourTexture);
        glUniform1i(textureShader->uniform("colourTexture"), 0);

         glUniform1i(textureShader->uniform("flipCoords"), useKinect);
        renderQuad(0);
    }




}

CartoonEffect::~CartoonEffect() {
    delete intermdiateBuffer;
    delete HBilateralShader;
    delete VBilateralShader;
}


