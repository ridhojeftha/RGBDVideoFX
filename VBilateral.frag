#version 330 core

uniform int imageHeight;

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform int kernelSize;
uniform float sigma;

float gaussianCoef(int x, int y){
    return exp(-((x*x)+(y*y))/(2*sigma*sigma));
}

void main()
{


    vec4 color = vec4(0.0);
    vec2 sample;
    float sum = 0.0;
    float coefG,coefZ,finalCoef;
    float Zp = texture2D(depthTexture, UV).r;

   
 //const float epsilon = 0.01;

 const float epsilon = 0.01;


        for(int j = -(kernelSize-1)/2; j <= (kernelSize-1)/2; j++) {

            sample = UV + (vec2(0,j) / float(imageHeight));
            coefG = gaussianCoef(0,j);
            float zTmp = texture2D(depthTexture, sample).r;
            coefZ = epsilon + abs(Zp - zTmp);
            finalCoef = coefG / coefZ;
            sum += finalCoef;
            color += finalCoef * texture2D(colourTexture,sample);

        }
    

    colour = (color / sum).rgb;
} 

