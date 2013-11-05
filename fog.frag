#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

uniform float fogDensity;
uniform vec3 fogColour;
uniform float fogAmount;

out vec3 colour;
in vec2 UV;

void main()
{

    float fogRatio = 1.0/exp(pow((1.0-texture2D(depthTexture, UV).r)*(1.0-fogAmount),fogDensity));
    colour = (fogColour.rgb*fogRatio)+(texture2D(colourTexture, UV).rgb*(1.0-fogRatio));

 
}