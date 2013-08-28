#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;
uniform sampler2D normalTexture;

uniform vec3 lightPosition;

uniform vec3 ambientMat;
uniform vec3 diffuseMat;
uniform vec3 specMat;

uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 specLight;

uniform float specPow;


out vec3 colour;
in vec2 UV;

void main (void)

{
    vec3 diffuse;
    vec3 spec;
    vec3 ambient;

    vec3 N =normalize(-(texture2D(normalTexture,UV).rgb- 0.5));
    vec3 L = normalize(lightPosition);
    vec3 E = vec3(1.0,0.0,0.0);
    vec3 R = normalize(reflect(-L,N));  

    ambient = ambientMat * ambientLight;
    diffuse = diffuseMat * diffuseLight * max(dot(N,L) , 0.0);
    spec = specMat * specLight * pow(max(dot(R,E),0.0),0.3*specPow);
    spec = vec3(0,0,0);


   colour = (clamp(ambient, 0.0, 1.0 ) + clamp(diffuse, 0.0, 1.0 ) + clamp(spec, 0.0, 1.0 ))+texture2D(colourTexture, UV).rgb;

    if (texture2D(depthTexture,UV).r<0.01){
        colour = texture2D(colourTexture, UV).rgb;
    }

 
}

