#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in mat4 instance_matrix;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;

out vec3 instanceColor;

void main()
{
    instanceColor = color;
    gl_Position = proj_matrix * view_matrix * instance_matrix * vec4(position, 1.0f);
}
