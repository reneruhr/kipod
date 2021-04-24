#version 410 core

layout(location = 0) in vec3 vPosition;

uniform mat3 kernel_matrix;

out vec2 texture_coords;
out mat3 kernel;

uniform int width;
uniform int height;

out vec2 offsets[9];


uniform float gaussian_sigma;
out float[25] gaussian;

float f(int i){
    return pow(i-2,2);
}

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

    const float M_PI = 3.141592f;
    float s2 = 1/ (2.0f*gaussian_sigma*gaussian_sigma);
    float spi2 = 1.0f/M_PI * s2;

    int size = 5;

    for(int i=0; i<size; ++i)
        for(int j=0; j<size; ++j)
            gaussian[j+i*size] = spi2*exp(-(f(i)+f(j))*s2);

}