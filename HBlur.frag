#version 330 core

out vec3 colour;
in vec2 UV;

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

uniform int imageWidth;
uniform int sampleRadius;
uniform float distributionSigma;

float totalWeight=1.0;


void samplePixel(vec2 coord, float dist)
{

    float intensity=exp(-(dist*dist)/(2.0*distributionSigma*distributionSigma));

    colour+=texture2D(colourTexture, coord).rgb*intensity;
    totalWeight+=intensity;

}

void main()
{

    colour = texture2D(colourTexture, UV).rgb;
  
    for (int i=1;i<=sampleRadius;i++){
        float step = float(i)/float(imageWidth);
        samplePixel(UV+vec2(step,0.0),step);
        samplePixel(UV-vec2(step,0.0),step);
    }

    colour/=totalWeight;


}