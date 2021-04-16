#version 330 core


in vec2 texture_coords;
out vec4 frag_color;

uniform sampler2D tex;

in mat3 kernel;
in vec2 offsets[9];

void main() 
{ 
    vec3 sample[9];
    vec3 color = vec3(0.0);

    mat3 kernel2 = mat3(-1,-1,-1,
		     		   -1,8,-1,
		      		   -1,-1,-1);

    for(int i = 0; i < 9; i++)	sample[i] = vec3(texture( tex, texture_coords.st + offsets[i] ));    
    for(int i = 0; i < 9; i++)	color += sample[i] * kernel[i % 3][ i / 3];
    
    frag_color = vec4(color, 1.0);
    //frag_color = vec4(texture( tex, texture_coords.st).rgb, 1.0);
} 

