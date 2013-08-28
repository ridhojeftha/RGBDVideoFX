#version 330 core

uniform int imageWidth;
uniform int imageHeight;

uniform sampler2D edgeTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

// radius for valley detection
//uniform int radius;
//uniform int renderwidth;

int radius =5;
uniform int renderwidth=3;

float intensity(in vec4 color)
{
	return sqrt((color.x*color.x)+(color.y*color.y)+(color.z*color.z));
}

 
 
void main(void){

    float xstep = 1.0/float(imageWidth);
    float ystep = 1.0/float(imageHeight);

    // let's learn more about our center pixel
    float center_intensity = intensity(texture2D(colourTexture, UV));
    // counters we need
    int darker_count = 0;
    float max_intensity = center_intensity;
    // let's look at our neighbouring points
    for(int i = -radius; i <= radius; i++)
    {
            for(int j = -radius; j<= radius; j++)
            {
                
                    float current_intensity = intensity(texture2D(colourTexture,UV + vec2(i*xstep, j*ystep)));
                    if(current_intensity < center_intensity)
                    {
                            darker_count++;
                    }
                    if(current_intensity > max_intensity)
                    {
                            max_intensity = current_intensity;
                    }
            }
    }
    // do we have a valley pixel?
    if((max_intensity - center_intensity) > 0.01*radius)
    {
            if(darker_count/(radius*radius) < (1-(1/radius)))
            {
                    colour= vec3(0.0,0.0,0.0); // yep, it's a valley pixel.
            }
    }
    colour= vec3(1.0,1.0,1.0); // no, it's not.


}
