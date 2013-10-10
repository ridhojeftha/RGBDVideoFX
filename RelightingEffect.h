#ifndef RELIGHTINGEFFECT_H
#define	RELIGHTINGEFFECT_H

#include "Effect.h"

class RelightingEffect : public Effect {
public:
    RelightingEffect();
    virtual ~RelightingEffect();
    void display();

    float * lightDirection;
    float * materialAmbient;
    float * materialDiffuse;
    float * materialSpecular;
    float * lightAmbient;
    float * lightDiffuse;
    float * lightSpecular;
    float shininess;
    int relightingBase;

    int bilateralFilterPasses;
    int bilateralKernelSize;
    float bilateralSigma;

private:
    Shader* HBilateralShader;
     Shader* VBilateralShader;

    Shader* phongShader;
    
      FrameBuffer* intermdiateBuffer;

};

#endif	/* RELIGHTINGEFFECT_H */

