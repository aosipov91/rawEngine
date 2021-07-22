#version 330 core

in vec2 TexCoord;

uniform sampler2D SpriteTex;

layout( location = 0 ) out vec4 FragColor;

in vec3 fcolor;


void main()
{
    //FragColor = vec4((1.0, 0.0, 0.0), 1.0); 
    FragColor = texture(SpriteTex, TexCoord);
}
