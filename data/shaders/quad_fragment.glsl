#version 330 core

in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, uv);
	//fragColor = vec4(0.0, 0.0, 1.0, 1.0); 


}
