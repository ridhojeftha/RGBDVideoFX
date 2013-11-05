#version 330 core

uniform sampler2D depthTexture;
uniform sampler2D colourTexture;
uniform sampler2D normalTexture;

uniform vec3 ambientMat;
uniform vec3 diffuseMat;
uniform vec3 specularMat;

uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 specularLight;

uniform vec3 lightPosition;
uniform float shininess;

out vec3 colour;
in vec2 UV;

void main (void)
{

    vec3 E = normalize(vec3(1.0,1.0,-1.0));
    vec3 N = normalize(-(texture2D(normalTexture,UV).rgb - 0.5));
    vec3 L = normalize(lightPosition);
    vec3 R = normalize(-reflect(L,N));  

    vec3 ambient = ambientMat * ambientLight;
    vec3 diffuse = diffuseMat * diffuseLight * dot(N,L);
    vec3 specular = dot(R,E) > 0 ? specularMat * specularLight * pow(max(dot(R,E),0.0),shininess): vec3(0);

    colour = (clamp(ambient, 0.0, 1.0)+clamp(diffuse, 0.0, 1.0)+clamp(specular, 0.0, 1.0))*texture2D(colourTexture, UV).rgb;
 
}

