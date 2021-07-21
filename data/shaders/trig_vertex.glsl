#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 proj;
uniform mat4 view;

out vec2 uv;
void main()
{
	uv = texcoord;
	gl_Position = proj * view * vec4(position, 1.0); 

}

