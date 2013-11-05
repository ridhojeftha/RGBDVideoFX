#version 330 core

out vec3 colour;
in vec2 UV;

uniform sampler2D colourTexture; // Colour texture
uniform sampler2D depthTexture; // Depth texture
uniform sampler2D blurredTexture; // Blurred texture

uniform float lensDiameter;         //A
uniform float focalLength;          //f
uniform float focalPlaneDepth;      //I

uniform int imageHeight;
uniform int sampleRadius;
uniform float distributionSigma;

float totalWeight=1.0;
float depth;

void samplePixel(vec2 coord, float dist)
{

    float intensity=exp(-(dist*dist)/(2.0*distributionSigma*distributionSigma));

    colour+=texture2D(blurredTexture, coord).rgb*intensity;
    totalWeight+=intensity;

}

void main()
{

    depth = texture2D(depthTexture, UV).r;
    colour = texture2D(colourTexture, UV).rgb;

    for (int i=1;i<=sampleRadius;i++){
        float step = float(i)/float(imageHeight);
        samplePixel(UV+vec2(0.0,step),step);
        samplePixel(UV-vec2(0.0,step),step);
    }

    colour/=totalWeight;

    float blurAmount = clamp(abs(lensDiameter * (depth - focalPlaneDepth) /(depth * (focalLength))),0.0,1.0);
    colour = (texture2D(colourTexture, UV).rgb * (1.0 - blurAmount)) + (colour * blurAmount);



}