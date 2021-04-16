#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat3 kernel_matrix;

out vec2 texture_coords;
out mat3 kernel;

uniform int width;
uniform int height;

out vec2 offsets[9];



void main()
{
	gl_Position =  vec4(vPosition, 1.0f);
	texture_coords = (vPosition.xy+vec2(1.0f)) / 2.0f;
	
	kernel = kernel_matrix;
	float offset_x = 1.0f / width;
	float offset_y = 1.0f / height;


 	offsets = vec2[](
        vec2(-offset_x,  offset_y), // top-left
        vec2( 0.0f,    offset_y), // top-center
        vec2( offset_x,  offset_y), // top-right
        vec2(-offset_x,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset_x,  0.0f),   // center-right
        vec2(-offset_x, -offset_y), // bottom-left
        vec2( 0.0f,   -offset_y), // bottom-center
        vec2( offset_x, -offset_y)  // bottom-right    
    );

}