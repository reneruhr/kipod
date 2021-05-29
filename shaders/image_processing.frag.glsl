#version 410 core


in vec2 texture_coords;
out vec4 frag_color;

uniform sampler2D tex;
uniform mat3 kernel_matrix;
in vec2 offsets[9];

uniform int algorithm;

// subroutine float Algorithm();
// subroutine uniform Algorithm algorithm;

// subroutine (Algorithm) 
// float None(){
// 	return 1.0f;
// }

// subroutine (Algorithm) 
// float Canny(){
// 	return 0.0f;
// }

in float[25] gaussian;
uniform float low_treshhold;
uniform float high_treshhold;


float RGBtoGray(in vec3 rgb)
{
    return 0.299*rgb.r + 0.587*rgb.g +0.114*rgb.b;
}


float x_offset(int i)
{
	int j = ( i % 5 ) - 2;
	return offsets[2].x * j;
}
float y_offset(int i)
{
	int j = 2 - ( i / 5 );
	return offsets[2].y * j;
}

vec2 offset(int i)
{
	return vec2(x_offset(i), y_offset(i));
}

float GreyGaussian(vec2 coords){
	float color = 0.0f;

	for(int i = 0; i < 25; i++)	color += RGBtoGray(vec3(texture( tex, coords + offset(i) ))) * gaussian[i];
	return color;
}

vec2 NormDirection()
{
    mat3 sobel_x = mat3(-1,0,1,
		     		   -2,0,2,
		      		   -1,0,1);
    mat3 sobel_y = mat3(-1,-2,-1,
		     		   0,0,0,
		      		   1,2,1);

    float sample_points[9];
    float color_x = 0.0f;
    float color_y = 0.0f;

    for(int i = 0; i < 9; i++)	sample_points[i] = GreyGaussian(texture_coords.st + offsets[i]);

    for(int i = 0; i < 9; i++)	color_x += sample_points[i] * sobel_x[i % 3][ i / 3];
    for(int i = 0; i < 9; i++)	color_y += sample_points[i] * sobel_y[i % 3][ i / 3];

    vec2 norm_direction;
    norm_direction.x = color_x*color_x+color_y*color_y;
    norm_direction.y = atan(color_y, color_x);

    return norm_direction;
}

mat3 MagnitudeSample()
{
    mat3 sobel_x = mat3(-1,0,1,
		     		   -2,0,2,
		      		   -1,0,1);
    mat3 sobel_y = mat3(-1,-2,-1,
		     		   0,0,0,
		      		   1,2,1);

    mat3 magnitude_sample;
    for(int j =0; j<9; ++j)
    {
		float sample_points[9];
		float color_x = 0.0f;
		float color_y = 0.0f;

		for(int i = 0; i < 9; i++)	sample_points[i] = GreyGaussian(texture_coords.st + offsets[i] + offsets[j] );

		for(int i = 0; i < 9; i++)	color_x += sample_points[i] * sobel_x[i % 3][ i / 3];
		for(int i = 0; i < 9; i++)	color_y += sample_points[i] * sobel_y[i % 3][ i / 3];

		magnitude_sample[j % 3][ j / 3] = color_x*color_x+color_y*color_y;
    }
    return magnitude_sample;
}

int Partition(in float theta)
{
	float M_PI = 3.141592f;
	float division = M_PI/16.0f;
	int partition_sector;

    if(theta <= division && theta > -division) partition_sector = 0;
    else if(theta+M_PI/8 <= division && theta+M_PI/8 > -division) partition_sector = 1;
    else if(theta-M_PI/8 <= division && theta-M_PI/8 > -division) partition_sector = 2;
    else if(theta+M_PI/4 <= division && theta+M_PI/4 > -division) partition_sector = 3;

    return partition_sector;
}

float Thinning(in mat3 magnitude_sample,in int partition_sector){
    float point = magnitude_sample[1][1];
    switch(partition_sector){
    case 0:
        if(point < magnitude_sample[0][1] || point < magnitude_sample[2][1]) point = 0.0f;
        break;
    case 1:
        if(point < magnitude_sample[0][0] || point < magnitude_sample[2][2]) point = 0.0f;
        break;
    case 2:
        if(point < magnitude_sample[2][0] || point < magnitude_sample[0][2]) point = 0.0f;
        break;
    case 3:
        if(point < magnitude_sample[1][0] || point < magnitude_sample[1][2]) point = 0.0f;
        break;
    }
    return point;
}

float Treshhold(in mat3 magnitude_sample,in float low,in float high){
    float point = magnitude_sample[1][1];
    if(point < low) return 0.0f;
    else if (point > high) return point;
    else{
        for(int i=0; i<3; i+=2)
            for(int j=0; j<3; j+=2)
                if(magnitude_sample[j][i] > high) return point;
    }
    return 0.0f;
}


void main() 
{ 
    if(algorithm == 0)
    {
	    vec3 sample_points[9];
	    vec3 color = vec3(0.0);

	    for(int i = 0; i < 9; i++)	sample_points[i] = vec3(texture( tex, texture_coords.st + offsets[i] ));    
	    for(int i = 0; i < 9; i++)	color += sample_points[i] * kernel_matrix[i % 3][ i / 3];
	    
	    frag_color = vec4(color, 1.0);
	}
    else if(algorithm == 1)
    {
    	vec2 norm_direction = NormDirection();
    	int partition_sector = Partition(norm_direction.y);
    	mat3 magnitude_sample = MagnitudeSample();
    	float color = Thinning(magnitude_sample, partition_sector);
    	magnitude_sample[1][1] = color;
    	color = Treshhold(magnitude_sample, low_treshhold, high_treshhold);
    	frag_color = vec4(vec3(color), 1.0);
    }
} 

