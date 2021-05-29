#pragma once
#include "../../render/render_shader.h"
namespace kipod {
	class ComputeShader : public Shader {
	public:
        ComputeShader() = default;
        ComputeShader(std::string comp) : Shader({}, {}, {}, comp)
        {
         
        }
	};



}