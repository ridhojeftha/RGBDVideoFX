#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform float lensDiameter;         //D
uniform float focalLength;          //f
uniform float focalPlaneDepth;      //u
uniform float intensityConstant;

uniform int imageHeight;


const float leakageConstant=1.0;
float totalWeight=0;


void samplePixel(vec2 offset, float dist, float depth, float CoC){

    float overlap;
    float  sDepth =1.0-texture2D(depthTexture, UV+offset).r;
    float sCoC = abs(lensDiameter * focalLength * (sDepth - focalPlaneDepth) /(sDepth * (focalPlaneDepth - focalLength)));
    vec3  sColour = texture2D(colourTexture, UV+offset).rgb;
    float  sLeakage = 1.0;
   float  sIntensity = intensityConstant/pow(sCoC,2.0);
float sigma=sCoC;
//sIntensity=exp(-dist*dist/(2*sigma*sigma));

//sIntensity=sCoC;
    if (sCoC<=dist){
        overlap=0;
    } else if (sCoC<dist+1){
        overlap=sCoC-dist;

    } else {
        overlap=1;
    }


    if (sDepth<focalPlaneDepth){
       sLeakage=leakageConstant*CoC;
    }

    colour+=sColour*sIntensity*overlap*sLeakage;
    totalWeight+=sIntensity*overlap*sLeakage;
}

void main()
{

    float depth =1.0-texture2D(depthTexture, UV).r;
    float CoC = abs(lensDiameter * (focalLength * (depth - focalPlaneDepth)) /(depth * (focalPlaneDepth - focalLength)));
    float intensity=intensityConstant/pow(CoC,2.0);
//intensity=1.0;
  

    
    colour = texture2D(colourTexture, UV).rgb*intensity;
    totalWeight=intensity;


    for (int i=1;i<50;i++){
        samplePixel(vec2(0.0,float(i)/float(imageHeight)),i,depth,CoC);
        samplePixel(vec2(0.0,float(i)/-float(imageHeight)),i,depth,CoC);
    }


    colour/=totalWeight;

//colour = vec3(CoC/10,0,0);
  

}




 