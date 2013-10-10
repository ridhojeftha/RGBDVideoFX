#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform float lensDiameter;         //D
uniform float focalLength;          //f
uniform float focalPlaneDepth;      //u
uniform float intensityConstant;
uniform int imageWidth;


const float leakageConstant=1.0;
float totalWeight=0;

void samplePixel(vec2 coord, float depth){

    float  sDepth =1.0-texture2D(depthTexture, coord).r;
  //  float sCoC = abs(lensDiameter * focalLength * (sDepth - focalPlaneDepth) /(sDepth * (focalPlaneDepth - focalLength)));

float sCoC = abs(sDepth - focalPlaneDepth) * lensDiameter;
    vec3  sColour = texture2D(colourTexture, coord).rgb;

   const float sigma=0.01;
    float xdist=UV.x-coord.x;
    float ydist=UV.y-coord.y;
  //  float dist=sqrt((xdist*xdist)+(ydist*ydist));

    float sIntensity=exp(-((xdist*xdist)+(ydist*ydist))/(2*sigma*sigma));



//float sIntensity=0.01;

    float  sLeakage = 1.0;
     if (sDepth>=depth-0.1){
         sLeakage=0.0;
     }

    float overlap=1.0;

  //  if (sCoC<=dist){
     //   overlap=0.0;

   // } else {
      //  overlap=1.0;
  //  }

    colour+=sColour*sIntensity*sLeakage*overlap*sCoC;
    totalWeight+=sIntensity*sLeakage*overlap*sCoC;
}


void main()
{

    float depth =1.0-texture2D(depthTexture, UV).r;

  // float CoC = abs(lensDiameter * focalLength * (depth - focalPlaneDepth) /(depth * (focalPlaneDepth - focalLength)));


float CoC = abs(depth - focalPlaneDepth);// * lensDiameter;
//    CoC = lensDiameter*abs(focalLength/(focalPlaneDepth-focalLength))*abs(1.0-(focalPlaneDepth/depth));

//CoC=abs((lensDiameter*focalLength*(zFocus-depth))/(zFocus*(depth-focalLength)));

    //float intensity=intensityConstant/pow(CoC,2.0);
//float intensity=CoC/100.0;

float intensity=1;

colour = texture2D(colourTexture, UV).rgb*intensity;
    totalWeight=intensity;
  
if (intensityConstant>=1.0){

    for (int i=0;i<=imageWidth;i++){
        for (int j=0;j<=imageWidth;j++){
            vec2 coord =vec2(float(i)/imageWidth,float(j)/imageWidth);
            if (coord!=UV){
                samplePixel(coord, depth);
            }
        }
    }


    colour/=totalWeight;

}else{
colour=vec3(CoC,CoC,CoC);
}



}

