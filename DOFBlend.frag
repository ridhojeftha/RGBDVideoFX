#version 330 core

uniform sampler2D colourTexture; // Colour texture
uniform sampler2D depthTexture; // Depth texture
uniform sampler2D blurredTexture; // Blurred texture

uniform float lensDiameter;         //A
uniform float focalLength;          //f
uniform float focalPlaneDepth;      //I

out vec3 colour;
in vec2 UV;

void main ()
{		

    float depth = 1.0-texture2D(depthTexture, UV).r;
    float blurAmount = clamp(abs(lensDiameter * (depth - focalPlaneDepth) /(depth * (focalLength))),0.0,1.0);

    colour = (texture2D(colourTexture, UV).rgb * (1.0 - blurAmount)) + (texture2D(blurredTexture, UV).rgb * blurAmount);

}
							
	