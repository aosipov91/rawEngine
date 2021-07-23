#version 330 core

in vec2 TexCoord;

uniform sampler2D SpriteTex;

layout( location = 0 ) out vec4 FragColor;
in vec3 vertex_color;


void main()
{
    //FragColor = vec4(vertexDebug[0].position, 1.0); 
    FragColor = vec4(vertex_color, 1.0) * texture(SpriteTex, TexCoord);
}
