#version 330 core


//in  vec2 vTexCoord;
layout (location = 0) in  vec4 vPosition;

uniform mat4 mvp;
uniform WindowBlock {
	vec2 x;
	vec2 y;
	vec2 z;
	vec2 w;
};

out vec4 vColor;
//out vec2 texCoord;

void main()
{
    // gl_Position.xyz = vPosition.xyz;
    // gl_Position.z = 0;
    // gl_Position.w=1;
    // texCoord = vTexCoord;

    vColor = vec4(0,1,0,1);
    //if(( z[0] <= vPosition.z ) && ( vPosition.z <= z[1] ) ) vColor.z=0.0f;
   
    vColor.x= abs(vPosition.w)/10.0f;
	vColor.z= 1-abs(vPosition.z)/10.0f;

    gl_Position = mvp * vec4(vPosition.x,vPosition.y,vPosition.z,1);
    //gl_PointSize = 5.0;

}
