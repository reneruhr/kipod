#version 330 core

layout (location = 0) in  vec3 vPosition;

uniform mat4 mvp;
uniform vec4 color;

out vec4 vcolor;

void main()
{
    gl_Position = mvp * vec4(vPosition,1);
    vcolor = color;
}
