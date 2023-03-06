#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;
// Uniform properties
uniform sampler2D texture;

struct light_source
{
   int  type;
   vec3 position;
   vec3 color;
   vec3 direction;
};
 
uniform light_source lights[20];
uniform int N_lights;

uniform vec3 eye_position;


// TODO(student): Declare various other uniforms
// Output
layout(location = 0) out vec4 out_color;


vec3 compute_light (int li) {

    int  type = lights[li].type;
    vec3 light_position = lights[li].position;
    vec3 light_color = lights[li].color;
    vec3 light_direction= lights[li].direction;

    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);

    float material_kd = 1;
    float material_ks = 1;
    float material_shininess = 1;

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0;
    float diffuse_light = material_kd * abs(dot(N, L));
    float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(abs(dot(N, H)), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    float light = 0;

    if (type == 1)
    {
        float cut_off_rad = 0.524;
        float spot_light = dot(-L, light_direction);
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
        light = ambient_light + (diffuse_light + specular_light) * 1/(1 + length(light_position - world_position));
    }

    return light * light_color;

}




void main()
{  
    
    vec2 texc = texcoord;

    vec3 light = vec3(0);

    for (int i = 0; i < N_lights; i++)
        light += compute_light(i);

    vec3 N = normalize(world_normal);
    vec3 L = normalize(vec3(1,2,0.5));
    light += vec3(1) * max(dot(N, L), 0) * 0.2;
    
    out_color = vec4(vec3(texture2D(texture, texc)) * light,1);
}
