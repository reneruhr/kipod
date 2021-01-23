#version 450 core

//in  vec2 texCoord;


in vec2 tCoord;
in vec4 vColor;
out vec4 fColor;

uniform sampler2D tex;

void main() 
{ 
   //fColor = textureLod( texture, texCoord, 0 );
   //fColor = vColor;

   fColor = texture(tex, tCoord); 
   fColor.w = 1.0f;
} 

