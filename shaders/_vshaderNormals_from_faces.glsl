#version 330 core



layout (location = 0) in  vec3 vPosition;
layout (location = 1) in  vec3 vNormal;
uniform mat4 mv;
uniform mat4 mv_normal;
uniform float normal_length;

out vec3 normals;

void main()
{

    gl_Position = mv * vec4(vPosition,1);

    normals = normalize(vec3(mv_normal*vec4(vNormal,0)));
    normals *= normal_length;

}
