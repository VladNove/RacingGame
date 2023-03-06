#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_text_coord;
layout(location = 3) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View; 
uniform mat4 Projection;
uniform float TimeElapsed;
// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color;

    float avg = (frag_color.x + frag_color.y + frag_color.z)/3;

    frag_color = vec3(avg);
    frag_color.x = sin(TimeElapsed) / 2 + .5;
    frag_color.y = sin(TimeElapsed) / 2 + .5;
    
    
    
    //frag_color.x = sin(TimeElapsed + 3.14) * .5 + .5;
    //frag_color.z = sin(TimeElapsed) * .5 + .5;
    // TODO(student): Compute gl_Position

    float randOffs = v_position.x * 12 + v_position.y * 23 + v_position.z * 38;
    randOffs = sin(randOffs) + 2;

    gl_Position = Projection * View * Model * vec4(v_position  * (2 + sin(TimeElapsed * randOffs)), 1.0);
}
