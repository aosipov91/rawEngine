#version 330 core

uniform sampler2D Sampler0;

in vec2 uv;
in vec3 p;
//layout(location = 0) out vec4 fragColor;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 depth;

void main(){
	//fragColor = texture(Sampler0, uv);
	position = p;

}
