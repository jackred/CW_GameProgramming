#version 430 core

out vec4 FragColor;

in vec3 instanceColor;

void main()
{
    FragColor = vec4(instanceColor, 1.0);
}