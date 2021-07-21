#version 330 core

in vec2 uv;

layout(location = 0) out float AoData;

uniform mat4 ProjectionMatrix;
uniform sampler2D PositionTex;
uniform sampler2D NormalTex;
uniform sampler2D DepthTex;
uniform sampler2D RandTex;

const int kernelSize = 32;
uniform vec3 SampleKernel[kernelSize];
uniform float Radius = .55f;
const vec2 randScale = vec2( 800.0 / 4.0, 600.0 / 4.0 );

int kernel_size = 32;
float radius = 0.8;
float bias = 0.2;

void main (void)
{
    // Create the random tangent space matrix
    vec3 randDir = normalize( texture(RandTex, uv.xy * randScale).xyz );
    vec3 n = normalize( texture(NormalTex, uv).xyz );
    vec3 biTang = cross( n, randDir );
    if( length(biTang) < 0.0001 )  // If n and randDir are parallel, n is in x-y plane
        biTang = cross( n, vec3(0,0,1));
    biTang = normalize(biTang);
    vec3 tang = cross(biTang, n);
    mat3 toCamSpace = mat3(tang, biTang, n);

    float occlusionSum = 0.0;
    vec3 camPos = texture(PositionTex, uv).xyz;
    for( int i = 0; i < kernelSize; i++ ) {
        vec3 samplePos = camPos + Radius * (toCamSpace * SampleKernel[i]);

        // Project point
        vec4 p = ProjectionMatrix * vec4(samplePos,1);
        p *= 1.0 / p.w;
        p.xyz = p.xyz * 0.5 + 0.5;

        // Access camera space z-coordinate at that point
        float surfaceZ = texture(PositionTex, p.xy).z;
        float zDist = surfaceZ - camPos.z;
        
        // Count points that ARE occluded
        if( zDist >= 0.0 && zDist <= Radius && surfaceZ > samplePos.z ) occlusionSum += 1.0;
    }

    float occ = occlusionSum / kernelSize;
    AoData = 1.0 - occ;
}