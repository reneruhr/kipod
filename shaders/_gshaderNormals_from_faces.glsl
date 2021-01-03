#version 330 core


layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 projection;

in vec3 normals[];

void main()
{
	vec4 mid = (gl_in[0].gl_Position+gl_in[1].gl_Position+gl_in[2].gl_Position)/3;
    gl_Position = projection*mid;
    EmitVertex();
    gl_Position = projection*(mid + vec4(normals[0],0));
    EmitVertex();

    EndPrimitive();
}
