#ifndef MAPEFFECT_H
#define	MAPEFFECT_H

#include "Effect.h"

class MapEffect : public Effect {
public:
    MapEffect();
    virtual ~MapEffect();

    void display();

    int selectedMap;

private:

    Shader* textureShader;

};


#endif

