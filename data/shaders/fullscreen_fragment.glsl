#version 330 core

in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D PositionTex;


void main()
{
    vec3 pos = texture(PositionTex, uv).xyz;
    fragColor =  vec4(pos, 1.);

}
