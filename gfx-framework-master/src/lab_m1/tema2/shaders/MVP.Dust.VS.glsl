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
uniform vec3 CarVel;
uniform float Time;
uniform float ExtraTime;
// Output
out vec3 frag_normal;
out vec3 frag_color;

out vec2 tex_coord;


void main()
{
    

    frag_normal = v_normal;
    //frag_color = v_color;
    tex_coord = v_texture_coord;

    float arg1 = v_texture_coord.x;
    float arg2 = v_texture_coord.y;

    float tphyis = mod(Time + arg1,1.0); // particleTime (0 - 1)
    bool dontDraw = false; //only draw particles that should have appearead

    if (tphyis > Time)
        dontDraw = true;
    if (tphyis < ExtraTime)
        dontDraw = true;

    vec3 pv;
    vec3 pcolor;
    bool keep = false;

    if (!dontDraw){
        pv = (Model * vec4(v_position,1.0)).xyz; // world position of particle
        vec3 initialPos = pv; // initial world position
        vec3 planeVel = v_normal; // side speed of particle
        planeVel.y = 0;
        
        vec3 vel = (Model * vec4(v_normal,1.0)).xyz; // speed of particle in world space
        vel -= pv; // negated, particle moves down when engine points up

        vel *= -100; // speed factor

        // time until particle collides with ground   
        float timeToGround = (0.15- pv.y) / (vel.y - CarVel.y *  1.5);

        // blend the color to bg if particle is old
        pcolor = v_color + (bgColor - v_color) * min(tphyis * 10,0.6);

        // if particle hasnt reached ground or never will draw in air
        if (tphyis < timeToGround || timeToGround < 0)
        {
            pv = pv + vel * tphyis - CarVel * tphyis* 1.5;

            // only draw if it is close to the engine
            if (distance(pv, initialPos) < 5 - arg2 * 4)
                keep = true;
        }
        else
        {
            // compute ground impact position
            vec3 impactPos = pv + vel * timeToGround - CarVel * timeToGround* 1.5;
            vel.y = 0;
            //time since ground collision
            float tsc = tphyis - timeToGround;
        
            pcolor = pcolor * .5;
            
            // particle will lose initial speed, and spread over the ground
            vel = vel/10 + planeVel * 200;
            pv = impactPos + vel * tsc - CarVel * tsc * 1.5;

            // if particle is old, speed will slow down
            if (tsc > 0.05){
                //pv -= vel * (tsc - 0.05) ;
                //vel /= 1 + 10 * tsc;
                //pv += vel * (tsc - 0.05) ;
                 pv -= vel * (1 - 1 / (1 + 10*tsc))*(tsc - 0.05);

            }
            // if ground impact position is too far dont draw particle
            if ( distance(impactPos, initialPos) +  distance(pv, initialPos) < 10 - 5 *arg2)
                keep = true;

        }
    }
    // if particle is too close or shouldnt be drawn, move it out of view
    if (tphyis < 0.005 + arg2 * 0.01 || (!keep) || dontDraw ){
        pv.y = -10000;
    }

   
    // planet shader effects
    vec3 distV = pv - CarPos;
    float dist2 = dot(distV,distV);
    float tcolor = dist2 / 100000;

    tcolor = min(tcolor, 1);

    pv.y = pv.y + dist2 * 0.0005;



    

    frag_color = pcolor + (bgColor - pcolor) * tcolor;
    gl_Position = Projection * View * vec4(pv, 1.0);


}
