#version 330 core

layout (location = 0) out vec3 PositionData;
layout (location = 1) out vec3 NormalData;
layout (location = 2) out vec3 ColorData;

in vec3 n;
in vec3 p;

void main()
{
    PositionData = p;
    NormalData = normalize(n);
    ColorData = vec3(.3f);
}
