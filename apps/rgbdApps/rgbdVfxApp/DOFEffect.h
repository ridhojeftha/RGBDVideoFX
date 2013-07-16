
#ifndef DOFEFFECT_H
#define	DOFEFFECT_H

#include "Effect.h"

class DOFEffect : public Effect {
public:
    DOFEffect();
    virtual ~DOFEffect();
    void keyboard(unsigned char key);
    void display();
private:

    float lensDiameter;
    float focalLength;
    float focalPlaneDepth;
    
    Shader* HBlurShader;
    Shader* VBlurShader;
    FrameBuffer* intermdiateBuffer;

};

#endif	

