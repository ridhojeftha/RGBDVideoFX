
#include "MapEffect.h"
#include "Renderer.h"

MapEffect::MapEffect() {

    selectedMap=1;

    textureShader = new Shader("texture.frag");
}

void MapEffect::display() {

        glUseProgram(textureShader->id());

        glActiveTexture(GL_TEXTURE0);
        switch (selectedMap){
            case 0:glBindTexture(GL_TEXTURE_2D, colourTexture);break;
            case 1:glBindTexture(GL_TEXTURE_2D, depthTexture);break;
            case 2:glBindTexture(GL_TEXTURE_2D, normalTexture);break;
        }
        glUniform1i(textureShader->uniform("colourTexture"), 0);

        glUniform1i(textureShader->uniform("flipCoords"), useKinect);
        renderQuad(0);
}



MapEffect::~MapEffect() {
    delete textureShader;
}


