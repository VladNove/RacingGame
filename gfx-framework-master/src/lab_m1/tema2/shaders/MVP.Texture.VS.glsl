#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 CarPos;
uniform vec3 bgColor;
// Output
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;


void main()
{


    frag_normal = v_normal;
    //frag_color = v_color;
    tex_coord = v_texture_coord;

    
   
    vec3 pv = (Model * vec4(v_position,1.0)).xyz;
    vec3 distV = pv - CarPos;
    float dist2 = dot(distV,distV);
    float tcolor = dist2 / 100000;
    
    if (bgColor.z > .7){
        
        tcolor = smoothstep(0,1,pow(dist2 / 100000,.5));
       // tcolor +=
       // tcolor += pv.y/100.f;
    }
    tcolor = min(tcolor, 1);

    pv.y = pv.y + dist2 * 0.0005;

    frag_color = v_color + (bgColor - v_color) * tcolor;
    gl_Position = Projection * View * vec4(pv, 1.0);


}
