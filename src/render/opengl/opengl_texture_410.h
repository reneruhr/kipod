#pragma once
#include "../../core.h"
class TextureOpenGL;
namespace kipod
{
	void RenderToTexture410(GLuint& frame_buffer, TextureOpenGL* texture_implementation);
}
