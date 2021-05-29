#version 430 core

in vec2 texture_coords;
out vec4 frag_color;

uniform layout(binding=3, rgba8) image2D texture_input;
uniform int width;
uniform int height;

uniform mat3 kernel_matrix;
in vec2 offsets[9];

uniform int algorithm;

void main() 
{
	    vec3 sample_points[9];
	    vec3 color = vec3(0.0);

        
	    for(int i = 0; i < 9; i++)	
        sample_points[i] 
            = vec3(imageLoad(texture_input, ivec2(float(width) * (texture_coords+offsets[i]).s, float(height) * (texture_coords+offsets[i]).t)));    
	    for(int i = 0; i < 9; i++)	
        color += sample_points[i] * kernel_matrix[i % 3][ i / 3];
	    
	    frag_color = vec4(color, 1.0);
} 

