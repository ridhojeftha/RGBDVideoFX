#ifndef RELIGHTINGEFFECT_H
#define	RELIGHTINGEFFECT_H

#include "Effect.h"

class RelightingEffect : public Effect {
public:
    RelightingEffect();
    virtual ~RelightingEffect();
    void display();
    void resize();

    float * lightDirection;
    float * materialAmbient;
    float * materialDiffuse;
    float * materialSpecular;
    float * lightAmbient;
    float * lightDiffuse;
    float * lightSpecular;
    float shininess;


     int selectedPreFilter;
    int preblurFilterPasses;
    int preblurKernelSize;
    float preblurSigma;

private:
    Shader* HBilateralShader;
     Shader* VBilateralShader;
    Shader* preblurHShader;
    Shader* preblurVShader;
    Shader* phongShader;
    
      FrameBuffer* intermdiateBuffer;

};

#endif	/* RELIGHTINGEFFECT_H */

