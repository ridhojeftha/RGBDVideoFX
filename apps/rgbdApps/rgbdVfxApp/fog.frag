#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;

in vec2 UV;

void main()
{

float density=0.5;
float d=length(texture2D(depthTexture, UV));
float fogAmount = 1/exp(pow(d*(1.0-density),2));
vec3 fogColour = vec3(1.0,1.0,1.0);

colour = (fogColour.rgb*fogAmount)+(texture2D(colourTexture, UV).rgb*(1-fogAmount));

 
}