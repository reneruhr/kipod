#version 330 core


layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 projection;

in vec3 normals[];

void main()
{
    gl_Position = projection*gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projection*(gl_in[0].gl_Position + vec4(normals[0],0));
    //gl_Position = gl_in[0].gl_Position + vec4(1.0,1.0,0.0,0.0);
    EmitVertex();

    EndPrimitive();
}
