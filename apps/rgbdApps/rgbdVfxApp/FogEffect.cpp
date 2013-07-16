
#include "FogEffect.h"
#include "Renderer.h"

FogEffect::FogEffect() {
      fogShader = new Shader("fog.frag");
}

void FogEffect::display(){
    
    // bind the program (the shaders)
    glUseProgram(fogShader->id());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(fogShader->uniform("depthTexture"), 0);
    
     glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glUniform1i(fogShader->uniform("colourTexture"), 1);

    renderQuad(0);
}

void FogEffect::keyboard(unsigned char){
    
}


FogEffect::~FogEffect() {
    delete fogShader;
}

