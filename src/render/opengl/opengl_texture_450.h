#pragma once
#include "../../core.h"
class TextureOpenGL;
class TextureIPOpenGL;
namespace kipod
{
	void LoadTextureIP450(TextureIPOpenGL* texture_implementation);
	void LoadTexture450(TextureOpenGL* texture_implementation, int texture_option_filter, int texture_option_wrap);
	void RenderToTexture450(GLuint& frame_buffer, TextureOpenGL* texture_implementation);
}
