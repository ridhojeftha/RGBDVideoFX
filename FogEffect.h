#ifndef FOGEFFECT_H
#define	FOGEFFECT_H

#include "Effect.h"

class FogEffect : public Effect {
public:
    FogEffect();
    virtual ~FogEffect();

    void display();
    void resize();

    float fogDensity;
    float fogAmount;
    float fogColourRed;
    float fogColourGreen;
    float fogColourBlue;
    
    int  preblurFilterPasses;
    int preblurKernelSize;
    float preblurSigma;

private:
    
Shader* preblurHShader;
Shader* preblurVShader;
    Shader* fogShader;
          FrameBuffer* intermdiateBuffer;

};

#endif	

