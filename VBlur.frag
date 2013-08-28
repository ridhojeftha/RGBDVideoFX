#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;

in vec2 UV;

uniform float lensDiameter;         //D
uniform float focalLength;          //f
uniform float focalPlaneDepth;      //u

uniform int imageHeight;


const float intensityConstant=1.0;

float totalWeight=1;


void samplePixel(vec2 offset, int i){


    float overlap;
    float  sDepth =1.0-texture2D(depthTexture, UV+offset).r;
    float  sCoC = abs(lensDiameter * (focalLength * (sDepth - focalPlaneDepth)) /(sDepth * (focalPlaneDepth - focalLength)));
    vec3  sColour = texture2D(colourTexture, UV+offset).rgb;
   //  float  sIntensity = intensityConstant/pow(i,2.0);
     float sIntensity =1;


        if (sCoC<=i){
            overlap=0;
        } else if (sCoC<i+1){
            overlap=sCoC-i;
        } else {
            overlap=1;
        }



        colour+=sColour*sIntensity*overlap;
        totalWeight+=sIntensity*overlap;
}

void main()
{

   float depth =1.0-texture2D(depthTexture, UV).r;
    float CoC = abs(lensDiameter * (focalLength * (depth - focalPlaneDepth)) /(depth * (focalPlaneDepth - focalLength)));

    CoC=clamp(CoC,0.0,200.0);

    //float intensity = intensityConstant/pow(CoC,2);

    colour = texture2D(colourTexture, UV).rgb;



    for (int i=1;i<CoC;i++){
        samplePixel(vec2(0.0,i/float(imageHeight)),i);
        samplePixel(vec2(0.0,i/-float(imageHeight)),i);
    }


    colour/=totalWeight;
  

}




 