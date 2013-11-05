#version 330 core

uniform int imageWidth;

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform int sampleRadius;
uniform float distributionSigma;

float totalWeight=1.0;
float depth;

void samplePixel(vec2 coord, float dist)
{

    float euclideanDifference = exp(-(dist*dist)/(2.0*distributionSigma*distributionSigma));
    float depthDifference = abs(depth - texture2D(depthTexture, coord).r);         
    float intensity=euclideanDifference / (depthDifference+0.01);

    colour+=texture2D(colourTexture, coord).rgb*intensity;
    totalWeight+=intensity;

}


void main()
{

    depth = texture2D(depthTexture, UV).r;
    colour = texture2D(colourTexture, UV).rgb;
  
    for (int i=1;i<=sampleRadius;i++){
        float step = float(i)/float(imageWidth);
        samplePixel(UV+vec2(step,0.0),step);
        samplePixel(UV-vec2(step,0.0),step);
    }

    colour/=totalWeight;
} 

