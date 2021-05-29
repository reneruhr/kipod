#pragma once
#include "../../core.h"
class TextureOpenGL;
namespace kipod
{
	void LoadTexture410(TextureOpenGL* texture_implementation, int texture_option_filter, int texture_option_wrap);
	void RenderToTexture410(GLuint& frame_buffer, TextureOpenGL* texture_implementation);
}
