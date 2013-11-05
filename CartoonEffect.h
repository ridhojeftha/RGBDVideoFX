

#ifndef CARTOONEFFECT_H
#define	CARTOONEFFECT_H


#include "Effect.h"

class CartoonEffect : public Effect {
public:
    CartoonEffect();
    virtual ~CartoonEffect();
    void display();
    void resize();

    int bilateralFilterPasses;
    int bilateralKernelSize;
    float bilateralSigma;

    int kuwaharaFilterPasses;
    int kuwaharaKernelSize;



    int edgeMethod;
    int filteringMethod;
    int quantize;

    float edgeThreshold;


private:
    Shader* HBilateralShader;
    Shader* VBilateralShader;

    Shader* edgeShader;
    Shader* sobelShader;
    Shader* radialValleyShader;
    Shader* kuwaharaShader;
    
    Shader* textureShader;
    Shader* quantizationShader;

    FrameBuffer* intermdiateBuffer;

};

#endif	/* CARTOONEFFECT_H */

