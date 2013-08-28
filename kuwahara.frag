#version 330 core

uniform int imageWidth;
uniform int imageHeight;

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;

out vec3 colour;
in vec2 UV;

uniform int radius;

uniform int kernelSize;
uniform float sigma;

 void main (void) 
 {

    float xstep = 1.0/float(imageWidth);
    float ystep = 1.0/float(imageHeight);
  
    float n = float((kernelSize + 1) * (kernelSize + 1));

    vec3 m[4];
    vec3 s[4];
    for (int k = 0; k < 4; ++k) {
        m[k] = vec3(0.0);
        s[k] = vec3(0.0);
    }

    for (int j = -kernelSize; j <= 0; ++j)  {
        for (int i = -kernelSize; i <= 0; ++i)  {
            vec3 c = texture2D(colourTexture, UV + vec2(j*xstep,i*ystep)).rgb;
            m[0] += c;
            s[0] += c * c;
        }
    }

    for (int j = -kernelSize; j <= 0; ++j)  {
        for (int i = 0; i <= kernelSize; ++i)  {
            vec3 c = texture2D(colourTexture, UV + vec2(j*xstep,i*ystep)).rgb;
            m[1] += c;
            s[1] += c * c;
        }
    }

    for (int j = 0; j <= kernelSize; ++j)  {
        for (int i = 0; i <= kernelSize; ++i)  {
            vec3 c = texture2D(colourTexture, UV + vec2(j*xstep,i*ystep)).rgb;
            m[2] += c;
            s[2] += c * c;
        }
    }

    for (int j = 0; j <= kernelSize; ++j)  {
        for (int i = -kernelSize; i <= 0; ++i)  {
            vec3 c = texture2D(colourTexture, UV + vec2(j*xstep,i*ystep)).rgb;
            m[3] += c;
            s[3] += c * c;
        }
    }


    float min_sigma2 = sigma;


    for (int k = 0; k < 4; ++k) {
        m[k] /= n;
        s[k] = abs(s[k] / n - m[k] * m[k]);

        float sigma2 = s[k].r + s[k].g + s[k].b;
        if (sigma2 < min_sigma2) {
            min_sigma2 = sigma2;
            colour = vec4(m[k], 1.0).rgb;
        }
    }
 }