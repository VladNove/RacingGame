#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform bool secondTexture;
uniform bool earth;
uniform float time;
// Output
layout(location = 0) out vec4 out_color;


void main()
{


    
    vec2 texc = texcoord;


    if (earth) {
        texc.x = mod(texc.x + time,1.0);
    }

    vec4 color = texture2D(texture_1, texc);
    vec4 outColor = color;
    if (secondTexture) {
        vec4 color2 = texture2D(texture_2, texc);
        outColor = mix(color, color2, 0.5);
    }
    if (outColor.a < 0.9)
        discard;


    out_color = outColor;

}
