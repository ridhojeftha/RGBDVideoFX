#version 330 core

uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

void main(void)
{
    colour = texture2D(colourTexture, UV).rgb;
}