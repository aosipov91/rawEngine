#version 330 core

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

//uniform float Size2;   // Half the width of the quad

uniform mat4 ProjectionMatrix;

in vData
{
    float size;
}vertices[];

out vec2 TexCoord;

out vec3 fcolor;


void main()
{
    mat4 m = ProjectionMatrix;

    gl_Position = m * (vec4(-vertices[0].size,-vertices[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(0.0,0.0);
    EmitVertex();

    gl_Position = m * (vec4(vertices[0].size,-vertices[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(1.0,0.0);
    EmitVertex();

    gl_Position = m * (vec4(-vertices[0].size,vertices[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(0.0,1.0);
    EmitVertex();

    gl_Position = m * (vec4(vertices[0].size,vertices[0].size,0.0,0.0) + gl_in[0].gl_Position);
    TexCoord = vec2(1.0,1.0);
    EmitVertex();

    EndPrimitive();
}
