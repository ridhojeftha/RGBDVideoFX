#version 330 core

uniform sampler2D colourTexture;


out vec3 colour;
in vec2 UV;

const float N = 0.1;

void main()
{


colour = texture2D(colourTexture, UV).rgb;
colour.r = round(colour.r / N)*N;
colour.g = round(colour.g / N)*N;
colour.b = round(colour.b / N)*N;

 
}