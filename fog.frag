#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;
uniform float fogDensity;
uniform vec3 fogColour;

out vec3 colour;
in vec2 UV;

void main()
{

float fogAmount = 1/exp(pow(length(texture2D(depthTexture, UV))*(1.0-fogDensity),2));
colour = (fogColour.rgb*fogAmount)+(texture2D(colourTexture, UV).rgb*(1-fogAmount));

 
}