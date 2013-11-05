
#ifndef DOFEFFECT_H
#define	DOFEFFECT_H

#include "Effect.h"

class DOFEffect : public Effect {
public:
    DOFEffect();
    virtual ~DOFEffect();
    
    void display();
    void resize();
    
    float distributionSigma;
    int sampleRadius;
    float lensDiameter;
    float focalLength;
    float focalPlaneDepth;

    
           int  preblurFilterPasses;
    int preblurKernelSize;
    float preblurSigma;

private:
    
Shader* preblurHShader;
Shader* preblurVShader;

    Shader* HBlurShader;
    Shader* VBlurShader;
    Shader* testShader;
      Shader* blendShader;
    FrameBuffer* intermdiateBuffer;
        FrameBuffer* blurredBuffer;

};

#endif	

