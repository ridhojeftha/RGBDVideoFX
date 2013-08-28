#ifndef FOGEFFECT_H
#define	FOGEFFECT_H

#include "Effect.h"

class FogEffect : public Effect {
public:
    FogEffect();
    virtual ~FogEffect();

    void display();

    float fogDensity;
    float fogColourRed;
    float fogColourGreen;
    float fogColourBlue;

private:

    Shader* fogShader;

};

#endif	

