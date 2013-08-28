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
    float specularPower;
    int relightingBase;

private:

    Shader* phongShader;

};

#endif	/* RELIGHTINGEFFECT_H */

