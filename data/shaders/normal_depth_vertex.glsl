#version 330 core

layout(location = 0 ) in vec3 position;
layout(location = 1 ) in vec3 normal;

uniform mat4 uViewProjM;
uniform mat4 modelView;
uniform mat4 uView;
uniform mat4 uModelM;
uniform mat4 uNormalMatrix;


out vec3 n;
out vec3 p;
out float depth;

void main()
{
    p = vec3(uModelM * vec4(position, 1.0));
    // p = position;
    n = normalize(vec3(uNormalMatrix * vec4(normal, 0.0f)));
    // n = normalize(n);
    gl_Position = uViewProjM * uModelM * vec4(position, 1.0f);
    depth = length((uView * vec4(position, 1.0)).xyz);
    v_normal = normal;
}
