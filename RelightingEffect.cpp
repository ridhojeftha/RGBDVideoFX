

#include "RelightingEffect.h"
#include "Renderer.h"

#include <Eigen/Core>
#include <Eigen/Geometry>

RelightingEffect::RelightingEffect() {

    preblurFilterPasses = 0;
    preblurKernelSize = 5;
    preblurSigma = 0.02;
    selectedPreFilter=0;

    lightDirection = new float[16] {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    materialAmbient = new float[3] {
        1, 1, 1
    };
    materialDiffuse = new float[3] {
        1, 1, 1
    };
    ;
    materialSpecular = new float[3] {
        1, 1, 1
    };
    ;
    lightAmbient = new float[3] {
        0.5, 0.5, 0.5
    };
    ;
    lightDiffuse = new float[3] {
        0.7, 0, 0
    };
    ;
    lightSpecular = new float[3] {
        0.6, 0.6, 0.6
    };
    ;
    shininess = 30.0;


    phongShader = new Shader("phong.frag");
        HBilateralShader = new Shader("HBilateral.frag");
    VBilateralShader = new Shader("VBilateral.frag");
      preblurHShader = new Shader("preblurH.frag");
    preblurVShader = new Shader("preblurV.frag");
    intermdiateBuffer = new FrameBuffer(screenWidth, screenHeight);

}
void RelightingEffect::resize() {
    intermdiateBuffer->resize();
 
}

__attribute__((force_align_arg_pointer)) void RelightingEffect::display() {
    
    GLuint normalTexInput = normalTexture;
    
     for (int i = 0; i < preblurFilterPasses; i++) {

        if (selectedPreFilter == 1) {
            //HORIZONTAL
            glUseProgram(preblurHShader->id());
            glUniform1i(preblurHShader->uniform("imageWidth"), inputWidth);
            glUniform1i(preblurHShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(preblurHShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, normalTexInput);
            glUniform1i(preblurHShader->uniform("colourTexture"), 0);


            glUniform1i(preblurHShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
            normalTexInput = intermdiateBuffer->texture();

            //VERTICAL
            glUseProgram(preblurVShader->id());
            glUniform1i(preblurVShader->uniform("imageHeight"), inputHeight);
            glUniform1i(preblurVShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(preblurVShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, normalTexInput);
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
            glBindTexture(GL_TEXTURE_2D, normalTexInput);
            glUniform1i(HBilateralShader->uniform("colourTexture"), 1);

            glUniform1i(HBilateralShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());
            normalTexInput = intermdiateBuffer->texture();

            //VERTICAL
            glUseProgram(VBilateralShader->id());
            glUniform1i(VBilateralShader->uniform("imageHeight"), inputHeight);
            glUniform1i(VBilateralShader->uniform("sampleRadius"), preblurKernelSize);
            glUniform1f(VBilateralShader->uniform("distributionSigma"), preblurSigma);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            glUniform1i(VBilateralShader->uniform("depthTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalTexInput);
            glUniform1i(VBilateralShader->uniform("colourTexture"), 1);

            glUniform1i(VBilateralShader->uniform("flipCoords"), false);
            renderQuad(intermdiateBuffer->fbo());

        }

    }
    

    // bind the program (the shaders)
    glUseProgram(phongShader->id());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(phongShader->uniform("depthTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colourTexture);

    glUniform1i(phongShader->uniform("colourTexture"), 1);
    

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalTexInput);
    glUniform1i(phongShader->uniform("normalTexture"), 2);



    Eigen::Quaternionf q(lightDirection);
    Eigen::AngleAxisf aa(q);
    Eigen::Vector3f v = aa.axis();
    glUniform3f(phongShader ->uniform("lightPosition"), v(0), v(1), v(2));

    glUniform3f(phongShader ->uniform("ambientMat"), materialAmbient[0], materialAmbient[1], materialAmbient[2]);
    glUniform3f(phongShader ->uniform("diffuseMat"), materialDiffuse[0], materialDiffuse[1], materialDiffuse[2]);
    glUniform3f(phongShader ->uniform("specularMat"), materialSpecular[0], materialSpecular[1], materialSpecular[2]);

    glUniform3f(phongShader ->uniform("ambientLight"), lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    glUniform3f(phongShader ->uniform("diffuseLight"), lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
    glUniform3f(phongShader ->uniform("specularLight"), lightSpecular[0], lightSpecular[1], lightSpecular[2]);

    glUniform1f(phongShader ->uniform("shininess"), shininess);
 

 glUniform1i(phongShader->uniform("flipCoords"), useKinect);
    renderQuad(0);
}



RelightingEffect::~RelightingEffect() {
    delete phongShader;
}

