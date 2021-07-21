#version 330 core

in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D PositionTex;
uniform sampler2D NormalTex;
uniform sampler2D ColorTex;
uniform sampler2D ParticleColorTex;

void main()
{
    vec3 pos = texture(PositionTex, uv).xyz;
    vec3 norm = texture(NormalTex, uv).xyz;
    vec3 diffColor = texture(ColorTex, uv).rgb;
    vec3 particleColor = texture(ParticleColorTex, uv).rgb;


    //fragColor = vec4(particleColor, 1.0) + vec4((fract(pos + 0.5) / 2.0 ), 1.0);
    fragColor =  vec4(diffColor, 1.);

}
