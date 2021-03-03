#version 330 core

//in  vec2 texCoord;

out vec4 fColor;

//uniform sampler2D texture;

void main() 
{ 
   //fColor = textureLod( texture, texCoord, 0 );
   fColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 

