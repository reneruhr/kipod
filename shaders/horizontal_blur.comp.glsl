//////////////////////////////////////
////////// average blur.glsl /////////
//////////////////////////////////////

#version 430 core

// each work group will be mapped onto a horizontal array of 64 pixels
layout(local_size_x=64, local_size_y=1, local_size_z=1) in;

// define our input and output images with a 4-component format (the standard
// for color textures)
layout(binding=0, rgba8) uniform readonly image2D inputImage;
layout(binding=1, rgba8) uniform writeonly image2D outputImage;

// This exact size of the images, such that we don't read or write beyond
// the boundaries
uniform ivec2 size;

// 64 pixels + 3 to the left + 3 to the right
shared vec4 sharedTexels[70];

void main(void)
{
	// get global and local invocation id for readability
	ivec2 gid = ivec2(gl_GlobalInvocationID.xy);
	uint lid = gl_LocalInvocationID.x;
	
	// Every thread reads one pixel, starting 3 pixels to the left
	// of the beginning of the work group
	// We clamp the x-coordinate to lie inside the image
	ivec2 coords = ivec2(clamp(gid.x-3,0,size.x-1), gid.y);
	// Load the texel and store it in shared memory
	sharedTexels[lid] = imageLoad(inputImage, coords);
	
	// We need to read 6 more texels, this can be done by the
	// threads with a local invocation id from 0...5
	if (lid < 6)
	{
		// 64 pixels further right than the first read: gid.x-3+64=gid.x+61
		coords.x = clamp(gid.x+61,0,size.x-1);
		sharedTexels[lid+64] = imageLoad(inputImage, coords);
	}
	
	// flush the writes to shared memory first
	memoryBarrierShared();
	// make sure that the threads of a work group wait here
	// until all writes to shared memory have been performed
	// and flushed
	barrier();
	
	// the average has to be computed only by threads that are inside
	// the image bounds. We don't have to check for the y-coordinate
	// because the work group height is 1 and therefore we will have
	// no unused threads in the y-direction.
	if (gid.x < size.x)
	{
		// Get the 7 texels from shared memory and average
		vec4 finalColor = vec4(0,0,0,0);
		for (uint i=0;i<7;i++)
			finalColor += sharedTexels[lid+i];
		finalColor *= (1.0/7.0);
		
		// Save the result to the outputImage at the pixel
		// given by the global invocation ID
		imageStore(outputImage, gid, finalColor);
	}
}