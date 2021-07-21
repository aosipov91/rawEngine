#version 330 core
in vec2 uv;
uniform sampler2D Sampler0;

layout( location = 0 ) out vec4 FragColor;


uniform float PixOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[10];
uniform bool horizontal;

vec4 blur()
{

    vec4 sum = texture(Sampler0, uv) * Weight[0];
    float dx = 1.0 / (textureSize(Sampler0,0)).x;
    float dy = 1.0 / (textureSize(Sampler0,0)).y;

    if (horizontal) {
    for( int i = 1; i < 10; i++ )
    {
        // go to the right towards of texel
       sum += texture( Sampler0, uv + vec2(PixOffset[i],0.0) * dx ) * Weight[i];
         // go the lefth towards of texe
       sum += texture( Sampler0, uv - vec2(PixOffset[i],0.0) * dx ) * Weight[i];
    }

    }
    else {
        for( int i = 1; i < 10; i++ )
        {
            // up towards
             sum += texture( Sampler0, uv + vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
             // down
             sum += texture( Sampler0, uv - vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
        }

    }

    return sum;
}

void main()
{
    FragColor = blur();
}