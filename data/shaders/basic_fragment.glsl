#version 330 core

layout (location = 0) out vec4 fragColor;
uniform sampler2D Sampler0;

in vec3 n;
in vec3 p;

in vec2 uv;
void main()
{
    //fragColor = vec4(n, 1.0);
    fragColor = texture(Sampler0, uv);
}
