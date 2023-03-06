#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction[2];
uniform vec3 light_position[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform int type_of_light[2];
uniform float cut_off_angle[2];

// Output
layout(location = 0) out vec4 out_color;



float compute_light (int li) {

    

    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position[li] - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);



    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    float diffuse_light = material_kd * max(dot(N, L), 0);
    float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    float light = 0;

    if (type_of_light[li] == 1)
    {
        float cut_off_rad = radians(cut_off_angle[li]);
        float spot_light = dot(-L, light_direction[li]);
        float spot_light_limit = cos(cut_off_rad);

        if (spot_light > spot_light_limit)
        {
            // Quadratic attenuation
            float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            light = ambient_light + light_att_factor * (diffuse_light + specular_light);
        }
        else
        {
            light = ambient_light;  // There is no spot light, but there is light from other objects
        }
    }
    else
    {
        light = ambient_light + diffuse_light + specular_light;
    }

    return light;

}

void main()
{

    

    float light = 0;


    for (int i = 0; i < 2; i++)
        light += compute_light(i);



    // Write pixel out color
    vec3 colour = object_color * light;

    out_color = vec4(colour, 1);

    // TODO(student): Write pixel out color
   // out_color = vec4(1);

}
