#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in float size;
layout (location = 3) in float time;
layout (location = 4) in float lifeTime;
layout (location = 5) in float mass;
layout (location = 6) in vec3 color;



uniform vec3 eyePos;
uniform vec3 gAccel;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float gTime;

out vData
{
    float size;

}vertex;

out vec3 fcolor;

//out float Size2;

void main()
{
    float t = gTime - time;
    vec3 posL = position;

    // Rotate the particles about local space.
    float sine = sin(0.5*mass*t);
    float cosine = cos(0.5*mass*t);
    float x = posL.x*cosine + posL.y*-sine;
    float y = posL.x*sine + posL.y*cosine;
    
    // Oscillate particles up and down.
    float s = sin(6.0f*t);
    posL.x = x;
    posL.y = y + mass*s;
    
    // Constant acceleration.
    posL = posL + velocity*t + 0.5f * gAccel * t * t;

    vertex.size = size;
    vertex.size += 8.0*t*t;
    float d = distance(position, eyePos);
    vertex.size = 600.0*size/(1.0 + 8.0 * d);

    fcolor = color * (1.0f - (t / lifeTime));
    gl_Position = ModelViewMatrix * vec4(posL,1.0);
}
