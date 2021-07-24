#version 330 core

layout (location = 0) out vec4 fragColor;

in vec3 n;
in vec3 p;

void main()
{
    fragColor = vec4(n, 1.0);
}
