#version 330 core


uniform sampler2D edgeTexture;
uniform sampler2D colourTexture;

uniform int imageWidth;
uniform int imageHeight;

out vec3 colour;
in vec2 UV;

uniform float threshold;

void main()
{

float total=0;

float center=texture2D(edgeTexture, UV).r;

float pw=1.0/imageWidth;
float ph=1.0/imageHeight;

total += abs(center-texture2D(edgeTexture, UV+vec2(pw,ph)).r);
total += abs(center-texture2D(edgeTexture, UV+vec2(-pw,ph)).r);
total += abs(center-texture2D(edgeTexture, UV+vec2(-pw,-ph)).r);
total += abs(center-texture2D(edgeTexture, UV+vec2(pw,-ph)).r);

total += abs(center-texture2D(edgeTexture, UV+vec2(pw,0)).r);
total += abs(center-texture2D(edgeTexture, UV+vec2(0,ph)).r);
total += abs(center-texture2D(edgeTexture, UV+vec2(0,-ph)).r);
total += abs(center-texture2D(edgeTexture, UV+vec2(-pw,0)).r);

if (total>=threshold){
    colour=vec3(0,0,0);
//colour=vec3(1,1,1);
}else{
colour=vec3(1,1,1);
    colour=texture2D(colourTexture, UV).rgb;
}


}