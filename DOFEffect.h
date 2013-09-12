
#ifndef DOFEFFECT_H
#define	DOFEFFECT_H

#include "Effect.h"

class DOFEffect : public Effect {
public:
    DOFEffect();
    virtual ~DOFEffect();
    
    void display();
    
    float lensDiameter;
    float focalLength;
    float focalPlaneDepth;
    float intensity;
    
private:

    Shader* HBlurShader;
    Shader* VBlurShader;
    FrameBuffer* intermdiateBuffer;

};

#endif	

