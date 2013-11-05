#ifndef MAPEFFECT_H
#define	MAPEFFECT_H

#include "Effect.h"

class MapEffect : public Effect {
public:
    MapEffect();
    virtual ~MapEffect();

    void display();
    void resize();

    int selectedMap;
    int selectedPreFilter;

    int preblurFilterPasses;
    int preblurKernelSize;
    float preblurSigma;

private:

    Shader* preblurHShader;
    Shader* preblurVShader;
    Shader* HBilateralShader;
    Shader* VBilateralShader;

    FrameBuffer* intermdiateBuffer;

    Shader* textureShader;

};


#endif

