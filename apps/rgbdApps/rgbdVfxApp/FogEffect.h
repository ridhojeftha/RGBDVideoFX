#ifndef FOGEFFECT_H
#define	FOGEFFECT_H

#include "Effect.h"


class FogEffect : public Effect {
public:
    FogEffect();
    virtual ~FogEffect();
    void keyboard(unsigned char key);
    void display();
private:

    Shader* fogShader;

};

#endif	

