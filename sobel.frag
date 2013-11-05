#version 330 core

uniform int imageWidth;
uniform int imageHeight;

uniform sampler2D edgeTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform float threshold;

float intensity(in vec4 colour)
{
    return (colour.r*colour.r)+(colour.g*colour.g)+(colour.b*colour.b);
}
 
 
void main(void)
{
    float xstep = 1.0/float(imageWidth);
    float ystep = 1.0/float(imageHeight);

    float samples[8]; 
	
    // get samples around pixel
    samples[0] = intensity(texture2D(edgeTexture,UV + vec2(-xstep,ystep)));
    samples[1] = intensity(texture2D(edgeTexture,UV + vec2(-xstep,0)));
    samples[2] = intensity(texture2D(edgeTexture,UV + vec2(-xstep,-ystep)));

    samples[3] = intensity(texture2D(edgeTexture,UV + vec2(0,ystep)));
    samples[4] = intensity(texture2D(edgeTexture,UV + vec2(0,-ystep)));

    samples[5] = intensity(texture2D(edgeTexture,UV + vec2(xstep,ystep)));
    samples[6] = intensity(texture2D(edgeTexture,UV + vec2(xstep,0)));
    samples[7] = intensity(texture2D(edgeTexture,UV + vec2(xstep,-ystep)));
 
    //        1 0 -1     -1 -2 -1
    //    X = 2 0 -2  Y = 0  0  0
    //        1 0 -1      1  2  1
 
    float xGradient = samples[0] + 2.0*samples[1] + samples[2] - samples[5] - 2.0*samples[6] - samples[7];
    float yGradient = -samples[0] - 2.0*samples[3] - samples[5] + samples[2] + 2.0 * samples[4] + samples[7];

    colour = (xGradient*xGradient) + (yGradient*yGradient) <= threshold ? texture2D(colourTexture,UV).rgb : vec3(0);
 
}