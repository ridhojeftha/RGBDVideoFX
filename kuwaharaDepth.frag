#version 330 core

uniform int imageWidth;
uniform int imageHeight;

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform int kernelSize;

vec3 regionMean[4];
vec3 regionVariance[4];

void samplePixel(vec2 coord, int region)
{

    vec3 sampleColour = texture2D(colourTexture, coord).rgb;
    float sampleDepth = texture2D(depthTexture, coord).r;
    regionMean[region] += sampleColour;
    regionVariance[region] += sampleDepth * sampleDepth;

}


 void main (void) 
 {

    float xstep = 1.0/float(imageWidth);
    float ystep = 1.0/float(imageHeight);
  
    float numSamples = float((kernelSize + 1) * (kernelSize + 1));


    for (int j = -kernelSize; j <= 0; ++j)  {
        for (int i = -kernelSize; i <= 0; ++i)  {
            samplePixel(UV + vec2(j*xstep,i*ystep),0);
        }
    }

    for (int j = -kernelSize; j <= 0; ++j)  {
        for (int i = 0; i <= kernelSize; ++i)  {
            samplePixel(UV + vec2(j*xstep,i*ystep),1);
        }
    }

    for (int j = 0; j <= kernelSize; ++j)  {
        for (int i = 0; i <= kernelSize; ++i)  {
            samplePixel(UV + vec2(j*xstep,i*ystep),2);
        }
    }

    for (int j = 0; j <= kernelSize; ++j)  {
        for (int i = -kernelSize; i <= 0; ++i)  {
            samplePixel(UV + vec2(j*xstep,i*ystep),3);
        }
    }


    float mininmumVariance = 10000;
 

    for (int k = 0; k < 4; ++k) {

        regionMean[k] /= numSamples;
        regionVariance[k] = abs((regionVariance[k] / numSamples) - (regionMean[k] * regionMean[k]));
        regionVariance[k].r = regionVariance[k].r + regionVariance[k].g + regionVariance[k].b;

        if (regionVariance[k].r < mininmumVariance) {
            mininmumVariance = regionVariance[k].r;
            colour = regionMean[k];
        }

    }
 }