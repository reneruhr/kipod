#version 330 core


//in  vec2 vTexCoord;
layout (location = 0) in  vec4 vPosition;

uniform mat4 mvp;
//out vec2 texCoord;

void main()
{
    // gl_Position.xyz = vPosition.xyz;
    // gl_Position.z = 0;
    // gl_Position.w=1;
    // texCoord = vTexCoord;


    gl_Position = mvp * vec4(vPosition.x,vPosition.y,vPosition.z,1);
    //gl_PointSize = 5.0;

}
