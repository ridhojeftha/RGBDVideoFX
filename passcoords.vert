#version 330
uniform bool flipCoords;

in vec3 pos;
in vec2 coord;
out vec2 UV;

void main()
{

    UV= flipCoords ? vec2(coord.s, 1.0 - coord.t) : coord.st;
    gl_Position=vec4(pos, 1.0);

}

