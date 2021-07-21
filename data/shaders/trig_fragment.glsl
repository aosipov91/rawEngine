#version 330 core

uniform sampler2D Sampler0;

in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main(){
	fragColor = texture(Sampler0, uv); 

}
