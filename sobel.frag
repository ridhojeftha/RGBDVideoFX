#version 330 core

uniform int imageWidth;
uniform int imageHeight;

uniform sampler2D edgeTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform float threshold;


float intensity(in vec4 color)
{
	return sqrt((color.x*color.x)+(color.y*color.y)+(color.z*color.z));
}
 

 
void main(void)
{
    float xstep = 1.0/float(imageWidth);
    float ystep = 1.0/float(imageHeight);
  
	

    // get samples around pixel
    float tleft = intensity(texture2D(edgeTexture,UV + vec2(-xstep,ystep)));
    float left = intensity(texture2D(edgeTexture,UV + vec2(-xstep,0)));
    float bleft = intensity(texture2D(edgeTexture,UV + vec2(-xstep,-ystep)));
    float top = intensity(texture2D(edgeTexture,UV + vec2(0,ystep)));
    float bottom = intensity(texture2D(edgeTexture,UV + vec2(0,-ystep)));
    float tright = intensity(texture2D(edgeTexture,UV + vec2(xstep,ystep)));
    float right = intensity(texture2D(edgeTexture,UV + vec2(xstep,0)));
    float bright = intensity(texture2D(edgeTexture,UV + vec2(xstep,-ystep)));
 
	// Sobel masks (to estimate gradient)
	//        1 0 -1     -1 -2 -1
	//    X = 2 0 -2  Y = 0  0  0
	//        1 0 -1      1  2  1
 
    float x = tleft + 2.0*left + bleft - tright - 2.0*right - bright;
    float y = -tleft - 2.0*top - tright + bleft + 2.0 * bottom + bright;
    float magnitude = sqrt((x*x) + (y*y));

    if (magnitude > threshold){
        colour = vec3(0);
    }else{
        colour = texture2D(colourTexture,UV).rgb;
    }
 
}