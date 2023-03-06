#include "Tema2.h"

#include <vector>
#include <set>
#include <string>
#include <iostream>


using namespace std;
using namespace m1;
#include "objects.hpp"
#include "levels.hpp"

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */



Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}



#define randF() ((float)rand()/RAND_MAX) 
#define randV() glm::vec3(randF()-.5, randF()-.5, randF()-.5)
#define mylerp(a,b,t) (a + (b-a) * t)

void Tema2::Init()
{
    //srand(1);
    polygonMode = GL_FILL;
  //  secondCamera = new gfxc::Camera(*GetSceneCamera());

    tr = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x - window->GetResolution().x/4, window->GetResolution().y);

    //std::cout << "C:\\Users\\vladn\\Desktop\\EGC\\gfx-framework-master\\assets\\fonts\\Hack-Bold.ttf" << endl;
    tr->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);


    {
        Shader* shader = new Shader("PlanetShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexColor.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
        carPosLoc = shaders["PlanetShader"]->GetUniformLocation("CarPos");
        bgColorLoc = shaders["PlanetShader"]->GetUniformLocation("bgColor");

    }
    {
        Shader* shader = new Shader("DustShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "MVP.Dust.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexColor.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
        carPosLoc2 = shaders["DustShader"]->GetUniformLocation("CarPos");
        timeLoc = shaders["DustShader"]->GetUniformLocation("Time");
        carVelLoc2 = shaders["DustShader"]->GetUniformLocation("CarVel");
        extraTimeLoc = shaders["DustShader"]->GetUniformLocation("ExtraTime");
        bgColorLoc2 = shaders["DustShader"]->GetUniformLocation("bgColor");



    }

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x - 50 - resolution.y / 5.f, 50, resolution.y / 5.f, resolution.x / 5.f);



    //trackPoints.push_back({ 0,0,0 });

    if (!generatingTrack) {
        GetTrackPoints();
    }
    else {
        trackPoints = { {0,0,0} };
        bgColor = { .7,.6,.6 };
    }


    CreateCar();
    CreateEngine();
    CreateRock();



    //trackGeneration
    if (!generatingTrack)
    {
        /*{
            static float timeOfLastPoint = 1;
            timeOfLastPoint -= deltaTimeSeconds;

            glm::vec3 lastPoint = trackPoints[trackPoints.size() - 1];
            glm::vec3 carPlanePos = car.pos - glm::vec3{ 0, car.pos.y, 0 };
            //DebugLine(car.pos, carPlanePos, { 1,1,1 });

            if (glm::length(lastPoint - carPlanePos) > 5 && timeOfLastPoint < 0)
            {
                timeOfLastPoint = .5f;

                trackPoints.push_back(carPlanePos);



            }
        }*/

        //line
        {
            vector<VertexFormat> ver;
            vector<unsigned int > indx;

            glm::vec3 prevBez = { -1,0,0 };

            for (int i = 0; i < trackPoints.size(); i++) {



                auto np = trackPoints[(i + 1) % trackPoints.size()];
                auto np2 = trackPoints[(i + 2) % trackPoints.size()];

                auto thisP = trackPoints[i];
                glm::vec3 bezP1 = 2.f * thisP - prevBez;
                glm::vec3 bezP2 = np + (np - np2) * .25f;
                bezP2 += (thisP - bezP2) * .2f;
                prevBez = bezP2;


                for (float t = 0; t < 1; t += .35) {


                    auto a1 = mylerp(thisP, bezP1, t);
                    auto a2 = mylerp(bezP1, bezP2, t);
                    auto a3 = mylerp(bezP2, np, t);
                    auto b1 = mylerp(a1, a2, t);
                    auto b2 = mylerp(a2, a3, t);
                    auto p = mylerp(b1, b2, t);




                    ver.push_back({ p + glm::vec3{0,.15,0}, {.6,.6,.6} });
                    indx.push_back(ver.size() - 1);

                }
            }

            Mesh* ok = new Mesh("track");
            ok->SetDrawMode(GL_LINE_LOOP);
            ok->InitFromData(ver, indx);
            meshes["trackLine"] = ok;

        }

        //terrain
        {


            vector<VertexFormat> ver;
            vector<unsigned int > indx;

            for (int i = 0; i < 50; i++)
            {
                for (int j = 0; j < 50; j++) {

                    float x = i / 50.f * 600.f - 300.f;
                    float y = j / 50.f * 600.f - 300.f;

                    float height;
                    glm::vec3 color;
                    float colorM;
                    switch(track){
                    case 0:
                         height = rand() % 10;
                         color = glm::vec3{ .75,.4,.2 };
                         colorM = (height / 10.f) / 2.f + .5f;
                         break;
                    case 1:
                        height = randF() * 5;
                        color = glm::vec3{ .3,.75,.2 };
                        colorM = (height / 5.f) / 2.f + .5f;
                        break;
                    case 2:
                        height = randF() * randF() * 15.f;
                        color = glm::vec3{ 1,1,1 };
                        colorM = (height / 15.f) *.4f + .6f;
                        break;
                    }



                    ver.push_back({ {x,height,y} ,color *colorM });
                    if (i > 0 && j > 0)
                    {
                        indx.push_back((i - 1) * 50 + (j - 1));
                        indx.push_back((i - 1) * 50 + (j));
                        indx.push_back((i) * 50 + (j - 1));
                        indx.push_back((i) * 50 + (j - 1));
                        indx.push_back((i - 1) * 50 + (j));
                        indx.push_back((i) * 50 + (j));
                    }
                }
            }


            for (auto& v : ver)
                for (auto& t : trackPoints) {
                    float dist2 = glm::length2(glm::vec3{ v.position.x,0, v.position.z } - t);
                    if (dist2 < 1000.f) {
                        v.position.y = 0 - (1 - dist2 / 1000.f);
                        switch (track) {
                        case 0:
                            v.color = glm::vec3{ .75,.4,.2 } *(.5f + randF() / 8);
                            break;
                        case 1:
                            v.color = glm::vec3{ .4,.37,.2 } *(.5f + randF() / 8);
                            break;
                        case 2:
                            v.color = glm::vec3{ 1,1,1 } *(.5f + randF() / 8);
                            break;
                        }
                    }
                }



            /*
            vector<VertexFormat> ver = {
            {   {-300,0,-300},{.75,.4,.2}},
            {   {300,0,-300},{.75,.4,.2} },
            {   {300,0,300}, {.75,.4,.2}},
            {   {-300,0,300}, {.75,.4,.2}}


            };
            vector<unsigned int > indx{ 0,1,2,2, 3, 0 };
            */

            Mesh* ok = new Mesh("track");
            ok->SetDrawMode(GL_TRIANGLES);
            ok->InitFromData(ver, indx);
            meshes["ground"] = ok;


        }

        //track
        {
            {
                vector<VertexFormat> ver;
                vector<unsigned int > indx;


                for (int i = 0; i < trackPoints.size(); i++) {

                    glm::vec3 toNext;
                    if (i != trackPoints.size() - 1)
                        toNext = trackPoints[i + 1] - trackPoints[i];
                    else
                        toNext = trackPoints[0] - trackPoints[i];

                    glm::vec3 sideDir = glm::cross(glm::normalize(toNext), glm::vec3{ 0,1,0 }) * 10.f;



                    ver.push_back({ trackPoints[i] + glm::vec3{0,.05,0} + sideDir, glm::vec3(.1 + randF() / 20.f) });
                    ver.push_back({ trackPoints[i] + glm::vec3{0,.05,0} - sideDir, glm::vec3(.1 + randF() / 20.f) });

                    indx.push_back(i * 2);
                    indx.push_back(i * 2 + 1);



                    //rock placement
                    if ((track == 0 && rand() % 8 == 0)
                        || (track == 1 && rand() % 16 == 0)
                        || (track == 2 && rand() % 24 == 0)) {
                        sideDir *= 1.5 + randF();

                        if (rand() % 2)
                            sideDir *= -1;


                        rocks.push_back({ trackPoints[i] + glm::vec3{0,randF() - .5,0} + sideDir, randF() * 3.14f, randF() + .5f });
                    }




                }
                indx.push_back(0);
                indx.push_back(1);

                Mesh* ok = new Mesh("track");
                ok->SetDrawMode(GL_TRIANGLE_STRIP);
                ok->InitFromData(ver, indx);
                meshes["track"] = ok;


            }
        }

        //warningLines
        {
            vector<VertexFormat> ver;
            vector<unsigned int > indx;
            vector<VertexFormat> pver;
            vector<unsigned int > pindx;


            for (int i = 0; i < trackPoints.size(); i++) {

                glm::vec3 toNext;
                if (i != trackPoints.size() - 1)
                    toNext = trackPoints[i + 1] - trackPoints[i];
                else
                    toNext = trackPoints[0] - trackPoints[i];
                toNext = glm::normalize(toNext);

                glm::vec3 toPrev;
                if (i != 0)
                    toPrev = trackPoints[i - 1] - trackPoints[i];
                else
                    toPrev = trackPoints[trackPoints.size() - 1] - trackPoints[i];
                toPrev = glm::normalize(toPrev);

                glm::vec3 sideDir = glm::cross(toNext, glm::vec3{ 0,1,0 }) * 10.f;

                float cross = glm::dot(glm::cross(toNext, toPrev), { 0,1,0 });

                if (abs(cross) > .15f)
                {
                    if (cross < 0)
                        sideDir *= -1;

                    ver.push_back({ trackPoints[i] + sideDir + glm::vec3(0, 0, 0), { 1,.5,0 } });
                    ver.push_back({ trackPoints[i] + sideDir + glm::vec3(0, 1.5, 0), { 1,.5,0 } });

                    ver.push_back({ trackPoints[i] + sideDir - toNext * .4f + glm::vec3(0, 2, 0), { 1,.5,0 } });
                    ver.push_back({ trackPoints[i] + sideDir + toNext * .6f + glm::vec3(0, 2.5, 0), { 1,.5,0 } });
                    ver.push_back({ trackPoints[i] + sideDir - toNext * .4f + glm::vec3(0, 3, 0), { 1,.5,0 } });
                    pver.push_back({ trackPoints[i] + sideDir + glm::vec3(0, 3, 0), { 1,.5,0 } });

                    indx.push_back(ver.size() - 5);
                    indx.push_back(ver.size() - 4);
                    indx.push_back(ver.size() - 3);
                    indx.push_back(ver.size() - 2);
                    indx.push_back(ver.size() - 2);
                    indx.push_back(ver.size() - 1);
                    pindx.push_back(pver.size() - 1);
                }
            }
            Mesh* ok = new Mesh("warningLines");
            ok->SetDrawMode(GL_LINES);
            ok->InitFromData(ver, indx);
            meshes["warningLines"] = ok;

            ok = new Mesh("warningPoints");
            ok->SetDrawMode(GL_POINTS);
            ok->InitFromData(pver, pindx);
            meshes["warningPoints"] = ok;
        }


        //dust
        {
            vector<VertexFormat> ver;
            vector<unsigned int > indx;

            for (int i = 0; i < 50; i++) {
                float angle = randF() * TO_RADIANS * 360.f;
                glm::vec3 pos = glm::vec3{ cos(angle),0,sin(angle) } *.1f * randF();
                glm::vec3 col = glm::vec3{ .5,.95,1 } *(randF() * .25f + .75f);
                glm::vec3 norm = glm::normalize(glm::vec3{ 0,1,0 } + pos * randF() * 2.f);
                float timeOffs = randF();
                float arg2 = randF();


                for (int i = 0; i < 200; i++) {
                    ver.push_back({ pos,col,norm,{timeOffs + i / 4000.,arg2} });
                    indx.push_back(ver.size() - 1);
                }

            }

            Mesh* ok = new Mesh("dust");
            ok->SetDrawMode(GL_POINTS);
            ok->InitFromData(ver, indx);
            meshes["dust"] = ok;

            for (auto& v : ver) {

                swap(v.color.x, v.color.z);


            }

            ok = new Mesh("dustE");
            ok->SetDrawMode(GL_POINTS);
            ok->InitFromData(ver, indx);
            meshes["dustE"] = ok;

            for (auto& v : ver) {
                swap(v.color.x, v.color.z);
                swap(v.color.y, v.color.z);
            }

            ok = new Mesh("dust2");
            ok->SetDrawMode(GL_POINTS);
            ok->InitFromData(ver, indx);
            meshes["dust2"] = ok;

        }

        //snow
        if (track == 2)
         {
            
            vector<VertexFormat> ver;
            vector<unsigned int > indx;
            for (int i = 0; i < 50000; i++) {
                
                    ver.push_back({ { randF() * 600.f - 300.f ,randF() * 50.f, randF() * 600.f - 300.f },{1,1,1} });
                    indx.push_back(ver.size() - 1);
                
            }


            Mesh* ok = new Mesh("snow");
            ok->SetDrawMode(GL_POINTS);
            ok->InitFromData(ver, indx);
            meshes["snow"] = ok;
        }
    }

    std::set<int> usedpoints = { 0 };

    // enemy cars
    if (!generatingTrack)
    for (int i = 0; i < 20; i++)
    {
        Car newCar = Car{ this };
        newCar.cpOffset = 1 + rand() % 7;
        newCar.autoPilotspeed = 15 + randF() * 20;
        int nextCpIndex = rand() % trackPoints.size();
        while (usedpoints.count(nextCpIndex))
            nextCpIndex = rand() % trackPoints.size();
        usedpoints.insert(nextCpIndex);
        glm::vec3 startPoint = trackPoints[nextCpIndex];
        newCar.pos = startPoint + glm::vec3(randF() - .5, 0, randF() - .5) * 4.f;
        newCar.userPos = newCar.pos;
        newCar.nextCp = nextCpIndex;
        newCar.sideOffset = randF() * 5.f - 2.5f;
        newCar.desiredHeight = 4.f - randF() + randF();


        newCar.playerCar = false;

        enemyCars.push_back(newCar);


    }

    for (Car& car : enemyCars)
    {
        allCars.push_back(&car);
    }
    allCars.push_back(&car);
    
    if (!generatingTrack) {
        car.pos.z -= 5;
        car.userPos.z -= 5;
    }
    

    GetSceneCamera()->SetPerspective(60, 16.f / 9.f, 0.1, 1000);
    secondCamera.SetOrthographic(9 * 16, 16 * 16, 0, 500);
}


void Tema2::FrameStart()
{
    glm::ivec2 resolution = window->GetResolution();

 
    if (fourthPlayer) {
        car.playerCamera.SetPerspective(60 + glm::length(car.vel), (float)resolution.x / resolution.y, 0.1, 1000);
        player2car.playerCamera.SetPerspective(60 + glm::length(player2car.vel), (float)resolution.x / resolution.y, 0.1, 1000);
        player3car.playerCamera.SetPerspective(60 + glm::length(player2car.vel), (float)resolution.x / resolution.y, 0.1, 1000);
        player4car.playerCamera.SetPerspective(60 + glm::length(player2car.vel), (float)resolution.x / resolution.y, 0.1, 1000);

        glViewport(0, resolution.y / 2, resolution.x / 2, resolution.y / 2);
    }
    else if (thirdPlayer){
        car.playerCamera.SetPerspective(60 + glm::length(car.vel), (float)resolution.x / resolution.y , 0.1, 1000);
        player2car.playerCamera.SetPerspective(60 + glm::length(player2car.vel), (float)resolution.x / resolution.y , 0.1, 1000);
        player3car.playerCamera.SetPerspective(60 + glm::length(player2car.vel), (float)resolution.x / resolution.y * 1.75f, 0.1, 1000);
        glViewport(0, resolution.y / 2, resolution.x / 2, resolution.y/2);
    }
    else if (secondPlayer){
        car.playerCamera.SetPerspective(60 + glm::length(car.vel), (float)resolution.x / resolution.y / 1.75f, 0.1, 1000);
        player2car.playerCamera.SetPerspective(60 + glm::length(player2car.vel), (float)resolution.x / resolution.y / 1.75f, 0.1, 1000);
        glViewport(0, 0, resolution.x / 2, resolution.y);
    }

    else {

        car.playerCamera.SetPerspective(60 + glm::length(car.vel), (float)resolution.x / resolution.y, 0.1, 1000);
        glViewport(0, 0, resolution.x, resolution.y);

    }

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw

   

    

}

static float t1, t2, t3, t4;
void Tema2::Update(float deltaTimeSeconds)
{
    using vec3 = glm::vec3;
    glLineWidth(3);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    deltaTimeSeconds = glm::clamp(deltaTimeSeconds, .0f, .1f);

    

    if (generatingTrack) {

        static float timeOfLastPoint = 1;
        timeOfLastPoint -= deltaTimeSeconds;

        glm::vec3 lastPoint = trackPoints[trackPoints.size() - 1];
        glm::vec3 carPlanePos = car.pos - glm::vec3{ 0, car.pos.y, 0 };
        //DebugLine(car.pos, carPlanePos, { 1,1,1 });

        if (glm::length(lastPoint - carPlanePos) > 5 && timeOfLastPoint < 0)
        {
            timeOfLastPoint = .2f;

            trackPoints.push_back(carPlanePos);



        }


        //line
        if (trackPoints.size() > 1)
        {
            vector<VertexFormat> ver;
            vector<unsigned int > indx;

            glm::vec3 prevBez = { -1,0,0 };

            for (int i = 0; i < trackPoints.size(); i++) {



                auto np = trackPoints[(i + 1) % trackPoints.size()];
                auto np2 = trackPoints[(i + 2) % trackPoints.size()];

                auto thisP = trackPoints[i];
                glm::vec3 bezP1 = 2.f * thisP - prevBez;
                glm::vec3 bezP2 = np + (np - np2) * .25f;
                bezP2 += (thisP - bezP2) * .2f;
                prevBez = bezP2;


                for (float t = 0; t < 1; t += 1) {


                    auto a1 = mylerp(thisP, bezP1, t);
                    auto a2 = mylerp(bezP1, bezP2, t);
                    auto a3 = mylerp(bezP2, np, t);
                    auto b1 = mylerp(a1, a2, t);
                    auto b2 = mylerp(a2, a3, t);
                    auto p = mylerp(b1, b2, t);




                    ver.push_back({ p + glm::vec3{0,.15,0}, {1,1,1} });
                    indx.push_back(ver.size() - 1);

                }
            }

            Mesh* ok = new Mesh("track");
            ok->SetDrawMode(GL_LINE_LOOP);
            ok->InitFromData(ver, indx);
            meshes["trackLine"] = ok;

        }



        for (float x = -300; x < 300; x+=10)
            ri_l.push_back({vec3{ x,0,-300 }, vec3{ x+10,0,-300 }, vec3{ 1,1,1 }, car.pos, 100.f});
        for (float x = -300; x < 300; x += 10)
            ri_l.push_back({ vec3{ x,0,300 }, vec3{ x + 10,0,300 }, vec3{ 1,1,1 }, car.pos, 100.f });
        for (float x = -300; x < 300; x += 10)
            ri_l.push_back({ vec3{ 300,0,x }, vec3{ 300,0,x+10 }, vec3{ 1,1,1 }, car.pos, 100.f });
        for (float x = -300; x < 300; x += 10)
            ri_l.push_back({ vec3{ -300,0,x }, vec3{ -300,0,x + 10 }, vec3{ 1,1,1 }, car.pos, 100.f });

        ri_l.push_back({ vec3{ -10,0,0 }, vec3{ 10,0,0 }, vec3{ 0,1,0 }, car.pos, 100.f });
    }
    
    // finish line
    for (float z = -12; z < 11.9; z+=4) {
        ri_l.push_back({ { 0,.2,z }, { 0,.2,z + 2 },{1,.5,0},{0,1,0},1.f });
        ri_l.push_back({ { 0,.2,z+2 }, { 0,.2,z + 4 },{.5,.25,0},{0,1,0},1.f });

    }

    // fisrt player Update;
    {
        CarUpdateWithTimer(car, deltaTimeSeconds);
    }
    
    if (secondPlayer) {
        CarUpdateWithTimer(player2car, deltaTimeSeconds);
    }
    if (thirdPlayer) {
        CarUpdateWithTimer(player3car, deltaTimeSeconds);
    }
    if (fourthPlayer) {
        CarUpdateWithTimer(player4car, deltaTimeSeconds);
    }
    //enemy car
    for (Car& car : enemyCars)
    {
        UpdateCar(car, deltaTimeSeconds);
    }


    //collisions
    for (int i = 0; i < allCars.size() - 1; i++) {
        Car& car1 = *allCars[i];

        for (int j = i + 1; j < allCars.size(); j++)
        {
            Car& car2 = *allCars[j];
            glm::vec3 car1to2 = car2.pos - car1.pos;
            glm::vec3 relVel = car2.vel - car1.vel;
            float penLenght = (2 * 1.6f) * (2 * 1.6f) - glm::length2(car1to2);

            if (penLenght > 0)
            {
                glm::vec3 norm = glm::normalize(car1to2);
                car1.pos += norm * -penLenght / 2.f;
                car2.pos += norm * penLenght / 2.f;
                auto ricoVel = norm * glm::dot(norm, relVel) / 1.5f;
                car1.vel += ricoVel;
                car2.vel += -ricoVel;
                car1.userPos += ricoVel;
                car2.userPos += -ricoVel;

                if (car1.playerCar)
                    car1.timeOut -= abs(glm::dot(norm, -relVel)) / 15.f;
                if (car2.playerCar)
                    car2.timeOut -= abs(glm::dot(norm, -relVel)) / 15.f;

            }
        }
    }

    //enviorment
    {
        ri_m.push_back({ "trackLine",glm::mat3(1) });
        ri_m.push_back({ "ground",glm::mat3(1) });
        ri_m.push_back({ "track",glm::mat3(1) });

        if (track == 2) {
            static float snowTime = 0;
            snowTime += deltaTimeSeconds / 10.f;
            snowTime = snowTime - (long)snowTime;

            glm::mat4 snowModel = glm::translate(glm::mat4(1), { 0,-snowTime * 50.f,0 });
            glm::mat4 snowModel2 = glm::translate(glm::mat4(1), { 0,-snowTime * 50 + 50.f,0 });
            glm::mat4 snowModel3 = glm::translate(glm::mat4(1), { 0,-snowTime * 50 + 100.f,0 });
            glm::mat4 snowModel4 = glm::translate(glm::mat4(1), { 0,-snowTime * 50 + 150.f,0 });
            ri_m.push_back({ "snow",snowModel });
            ri_m.push_back({ "snow",snowModel2 });
            ri_m.push_back({ "snow",snowModel3 });
            ri_m.push_back({ "snow",snowModel4 });
        }
        for (Rock& r : rocks) {
            glm::mat4 rmodel = glm::translate(glm::mat4(1), r.pos);
            rmodel = glm::scale(rmodel, vec3(r.scale));
            rmodel = glm::rotate(rmodel, r.orient, { 0,1,0 });
            // RenderMesh(meshes["rock"], shaders["PlanetShader"], rmodel);
            ri_m.push_back({ "rock",rmodel });

        }
    }
   
    //render
    {
        RenderScene(car);
    }
    // renderSecondPlayer
    glm::ivec2 resolution = window->GetResolution();
    if (secondPlayer) {
        //glClear(GL_DEPTH_BUFFER_BIT);
        if (thirdPlayer)
            glViewport(resolution.x / 2, resolution.y / 2, resolution.x / 2, resolution.y/2);
        else
            glViewport(resolution.x / 2, 0, resolution.x / 2, resolution.y);

        RenderScene(player2car);
    }
    if (thirdPlayer) {
       // glClear(GL_DEPTH_BUFFER_BIT);
        if (fourthPlayer)
            glViewport(0, 0, resolution.x/2, resolution.y / 2);
        else
            glViewport(0, 0, resolution.x , resolution.y/2);
        RenderScene(player3car);
    }
    if (fourthPlayer) {
      //  glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(resolution.x / 2, 0, resolution.x / 2, resolution.y / 2);
        RenderScene(player4car);
    }


    ri_m.clear();
    ri_d.clear();
    ri_l.clear();

    if (showFps) {
        string text = "fps: " + std::to_string(1 / deltaTimeSeconds);
        tr->RenderText(text, 0, 150, .3);
    }

}


void Tema2::FrameEnd()
{
    glLineWidth(1);

    if (!fourthPlayer && !thirdPlayer) {
        if (miniMap) {
            RenderMiniMap(car);
        }

        //second playerMiniMap
        if (secondPlayer && p2miniMap) {
            RenderMiniMap(player2car);
        }
    }

    if (reInit) {
        enemyCars.clear();
        allCars.clear();
        rocks.clear();

        track++;
        track = track % 3;
        {
           // gfxc::Camera* pc = car.playerCamera;
            car = Car{ this };
          //  car.playerCamera = pc;
            
            secondPlayer = false;
            thirdPlayer = false;
            fourthPlayer = false;

        }
        Init();
        reInit = false;
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    deltaTime = glm::clamp(deltaTime, 0.f, 1.f);


    /*if (window->KeyHold(GLFW_KEY_A))
        car.heading +=deltaTime * 2;
    else if (window->KeyHold(GLFW_KEY_D))
        car.heading -=deltaTime * 2;*/

    

#define key(x) window->KeyHold(GLFW_KEY_##x##)

    int n;

    if (glfwJoystickPresent(0) && !thirdPlayer) {
        const float* f = glfwGetJoystickAxes(0, &n);



        const unsigned char* b = glfwGetJoystickButtons(0, &n);
        CarInputKeyboard(car, (f[5] + 1) / 2, (f[4] + 1) / 2, -f[0], f[0], b[0], deltaTime);
    } else
        CarInputKeyboard(car, key(W), key(S), key(A), key(D), key(LEFT_SHIFT), deltaTime);


    if (secondPlayer)
    {
        if (glfwJoystickPresent(1) && !fourthPlayer) {
            const float* f = glfwGetJoystickAxes(1, &n);
            const unsigned char* b = glfwGetJoystickButtons(1, &n);
            CarInputKeyboard(player2car, (f[5] + 1) / 2, (f[4] + 1) / 2, -f[0], f[0], b[0], deltaTime);
        } else 
            CarInputKeyboard(player2car, key(UP), key(DOWN), key(LEFT), key(RIGHT), key(RIGHT_SHIFT), deltaTime);
    }

    if (thirdPlayer) {
        const float* f = glfwGetJoystickAxes(0, &n);
        const unsigned char* b = glfwGetJoystickButtons(0, &n);
        CarInputKeyboard(player3car, (f[5] + 1) / 2, (f[4] + 1) / 2, -f[0], f[0], b[0], deltaTime);
    }

    if (fourthPlayer) {
        const float* f = glfwGetJoystickAxes(1, &n);
        const unsigned char* b = glfwGetJoystickButtons(1, &n);
        CarInputKeyboard(player4car, (f[5] + 1) / 2, (f[4] + 1) / 2, -f[0], f[0], b[0], deltaTime);
    }

    if (glfwJoystickPresent(0)) {
        static bool resetC0 = false;
        if (glfwGetJoystickButtons(0, &n)[3] == GLFW_PRESS) {
            if (!resetC0) {
                resetC0 = true;
                if (thirdPlayer)
                    ResetCar(player3car);
                else
                    ResetCar(car);
            }
        }
        else resetC0 = false;
    }
    if (glfwJoystickPresent(1)) {
        static bool resetC1 = false;
        if (glfwGetJoystickButtons(1, &n)[3] == GLFW_PRESS) {
            if (!resetC1) {
                resetC1 = true;
                if (fourthPlayer)
                    ResetCar(player4car);
                else {
                    ResetCar(player2car);
                    player2car.secondPlayer = true;
                }
            }
        }
        else
            resetC1 = false;
    }



}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }


    if (key == GLFW_KEY_O) {
        cout << endl << endl << endl;
        for (int i = 0; i < trackPoints.size(); i++) {
            cout << "{" << trackPoints[i].x << "," << trackPoints[i].y << "," << trackPoints[i].z << "}," << endl;
        }
    }
    if (key == GLFW_KEY_X) {
        miniMap = !miniMap;
    }
    if (key == GLFW_KEY_RIGHT_CONTROL) {
        p2miniMap = !p2miniMap;
    }

    if (key == GLFW_KEY_R) {
        ResetCar(car);
    } 
    if (key == GLFW_KEY_P) {
        ResetCar(player2car);
        player2car.secondPlayer = true;
    }

    int n;
    
   
    if (key == GLFW_KEY_V) {
        showTime = !showTime;
    }
    if (key == GLFW_KEY_N) {
        reInit = true;
    }
    if (key == GLFW_KEY_M) {
        showFps = !showFps;
    }
    if (key == GLFW_KEY_B) {
        permaBoost = true;
    }


    const auto createCar = [&](Car& car, float offs) {
        car = Car{ this };
        car.pos.z += offs;
        car.userPos.z += offs;
        car.secondPlayer = true;
        allCars.push_back(&car);
    };


    if (key == GLFW_KEY_RIGHT_SHIFT && !secondPlayer) {
        secondPlayer = true;
        createCar(player2car,5.f);
    }
    if (key == GLFW_KEY_3 && secondPlayer && !thirdPlayer && glfwJoystickPresent(0)) {
        thirdPlayer = true;
        createCar(player3car, 9.f);
    }
    if (key == GLFW_KEY_4 && secondPlayer && thirdPlayer && !fourthPlayer && glfwJoystickPresent(1)) {
        fourthPlayer = true;
        createCar(player4car, -9.f);
    }
    
}

void m1::Tema2::UpdateCar(Car& car, float deltaTimeSeconds)
{

    using vec3 = glm::vec3;

    {
        //autopilot
        if (Engine::GetElapsedTime() > 3.f && !car.playerCar)
        {



            glm::vec3 planeUserPos = { car.userPos.x, 0, car.userPos.z };
            glm::vec3 cpHeading = glm::normalize(trackPoints[(car.nextCp + 1) % trackPoints.size()] - trackPoints[car.nextCp]);

            if (glm::dot(cpHeading, planeUserPos - trackPoints[car.nextCp]) > 0 || car.timeSinceLastCp > 0.5f) {
                car.nextCp++;
                car.nextCp %= trackPoints.size();
                car.timeSinceLastCp = 0;
            }
            car.timeSinceLastCp += deltaTimeSeconds;

            int tpOffset = car.cpOffset;

            glm::vec3 nextCp = trackPoints[(car.nextCp + tpOffset) % trackPoints.size()];
            glm::vec3 nextHeading = trackPoints[(car.nextCp + tpOffset + 1) % trackPoints.size()] - trackPoints[(car.nextCp + tpOffset) % trackPoints.size()];
            glm::vec3 sideVec = glm::cross(glm::normalize(nextHeading), vec3{ 0,1,0 }) * car.sideOffset;
            glm::vec3 toNextCp = glm::normalize(nextCp + sideVec - planeUserPos);

            //DebugLine(car.pos, nextCp + sideVec + vec3{0, car.pos.y, 1}, { 0, 1, 0 });
            car.userPos += toNextCp * deltaTimeSeconds * car.autoPilotspeed;
            //cout << car.cpOffset <<" "<< car.autoPilotspeed << " " << endl;

            car.heading = -atan2(nextHeading.z, nextHeading.x);
            //  DebugLine(car.pos, car.userPos, { 1,0,0 });


        }

        //carControl
        {

            vec3 carFwd;
            vec3 currentCarFwd;

            //heading and target position
            {
                carFwd = glm::rotate(glm::angleAxis(car.heading, vec3{ 0,1,0 }), { 1,0,0 });
                currentCarFwd = glm::rotate(car.rot, { 1,0,0 });
                car.cfwd = currentCarFwd;
                car.userPos.y = car.desiredHeight;
                car.userPos += (car.pos - car.userPos) * 1.f * deltaTimeSeconds;
                if (car.playerCar)
                    car.userPos += carFwd * deltaTimeSeconds * car.fwdSpeed;


                vec3 carRight = { 0,0, -car.lrSpeed };
                carRight = glm::rotate(glm::angleAxis(car.heading, vec3{ 0,1,0 }), carRight);
                car.userPos += carRight * deltaTimeSeconds;

                float carHeadingErr = glm::dot({ 0,1,0 }, glm::cross(carFwd, currentCarFwd));
                if (glm::dot(carFwd, currentCarFwd) < 0)
                    carHeadingErr = (carHeadingErr < 0) ? -2 : 2 - carHeadingErr;
                car.heading += carHeadingErr * .5f * deltaTimeSeconds;
            }
            // camera, physics, env
            {
                //camera controll
                if (car.playerCar)
                {
                    vec3& oldCameraPos = car.oldCameraPos;
                    vec3& oldLookAtPoint = car.oldLookAtPoint;

                    float speedCorrect = (3 - 2 / (1 + glm::length2(car.vel)));

                    vec3 cameraOffs = glm::normalize(currentCarFwd * 10.f + car.vel);
                    cameraOffs.y /= 10.f;

                    vec3 desiredCameraPos = car.pos + car.vel / 5.f -
                        5.f * cameraOffs + vec3{ 0,2,0 };
                    vec3 newCameraPos = oldCameraPos + (desiredCameraPos - oldCameraPos) * deltaTimeSeconds * speedCorrect;


                    vec3 desiredLookAtPoint = car.pos + vec3{ currentCarFwd.x,min(currentCarFwd.y,0.f),currentCarFwd.z } *10.f + car.vel;

                    vec3 lookAtPoint = oldLookAtPoint + (desiredLookAtPoint - oldLookAtPoint) * deltaTimeSeconds * 2.f * speedCorrect;
                    oldLookAtPoint = lookAtPoint;

                    glm::quat cameraRot = glm::quatLookAt(glm::normalize(lookAtPoint - newCameraPos), { 0,1,0 });


                    car.playerCamera.SetPositionAndRotation(newCameraPos, cameraRot);

                    oldCameraPos = newCameraPos;
                }

                // physics update
                {
                    if (!car.grounded)
                        car.vel.y -= 9.8f * deltaTimeSeconds;

                    car.pos += car.vel * deltaTimeSeconds;

                    if (car.pos.y < 1)
                    {
                        car.pos.y = 1;
                        car.vel.y = 0;
                        car.vel -= car.vel * deltaTimeSeconds;
                        car.angularRot -= car.angularRot * deltaTimeSeconds * 10.f;
                    }

                    car.rot *= glm::angleAxis(car.angularRot.y * deltaTimeSeconds, vec3{ 0,1,0 });
                    car.rot *= glm::angleAxis(car.angularRot.z * deltaTimeSeconds, vec3{ 0,0,1 });
                    car.rot *= glm::angleAxis(car.angularRot.x * deltaTimeSeconds, vec3{ 1,0,0 });
                }

            }

            // car rendering
            {
                glm::mat4 model = glm::translate(glm::mat4(1), car.pos);

                model *= glm::toMat4(car.rot);
                if (car.secondPlayer)
                    ri_m.push_back({ "car2", glm::scale(model, vec3{ 1,1,1 }) });
                else if (car.playerCar)
                    ri_m.push_back({ "car", glm::scale(model, vec3{ 1,1,1 }) });
                    //RenderMesh(meshes["car"], shaders["PlanetShader"], glm::scale(model, vec3{ 1,1,1 }));
                else
                    ri_m.push_back({ "carE", glm::scale(model, vec3{ 1,1,1 }) });
                    //RenderMesh(meshes["carE"], shaders["PlanetShader"], glm::scale(model, vec3{ 1,1,1 }));
                glm::mat4 shadowModel = glm::translate(glm::mat4(1), { car.pos.x,.25,car.pos.z });
                shadowModel = glm::scale(shadowModel, { 1,.001f,1 });
                shadowModel *= glm::toMat4(car.rot);

                ri_m.push_back({ "carS", shadowModel });
                //RenderMesh(meshes["carS"], shaders["PlanetShader"], shadowModel);

                //engine rendering and indicator
                {
                    // engine positions
                    for (Car::Engine* ep : std::vector<Car::Engine*>{ &car.eBl, &car.eBr, &car.eFr, &car.eFl, &car.eYl, &car.eYr })
                    {

                        ep->actualOrient = glm::rotate(car.rot, ep->orient);
                        ep->actualRelPos = glm::rotate(car.rot, ep->relPos);
                        
                    }

                    //indicator
                    if (car.playerCar)
                    {
                        const vec3 lp1c = { 1.95666,-0.297439,1.3395 + .01 };
                        const vec3 lp2c = { -0.252516,-0.32545,1.00192 + .01 };
                        const vec3 lp3c = { -1.49389, -0.305302,0.470492 + .01 };


                        const float len1 = glm::length(lp3c - lp2c);
                        const float len2 = glm::length(lp2c - lp1c);
                        const float totLen = len1 + len2;

                        glm::vec3 col = { .5,.95,1 };
                        if (car.secondPlayer)
                            swap(col.y, col.z);

                        for (int i = -1; i <= 1; i += 2) {

                            vec3 lp1 = lp1c, lp2 = lp2c, lp3 = lp3c;
                            lp1.z *= i;
                            lp2.z *= i;
                            lp3.z *= i;


                            lp1 = model * glm::vec4(lp1, 1);
                            lp2 = model * glm::vec4(lp2, 1);
                            lp3 = model * glm::vec4(lp3, 1);

                            float fillLen = totLen * car.timeOut / 2.f;
                            //glLineWidth(6);

                            if (fillLen > totLen - .1f) {
                                ri_l.push_back({ lp1, lp2, col,car.pos });
                                ri_l.push_back({ lp2, lp3, col,car.pos });
                            }

                            else if (fillLen > len1) {
                                float t = (fillLen - len1) / len2;

                                vec3 splitP = mylerp(lp2, lp1, t);

                                ri_l.push_back({ splitP, lp1, { 0,0,0 },car.pos });
                                ri_l.push_back({ lp2, splitP, col,car.pos });
                                ri_l.push_back({ lp2, lp3, col,car.pos });
                            }

                            else {
                                float t = fillLen / len1;

                                vec3 splitP = mylerp(lp3, lp2, t);

                                ri_l.push_back({ lp2, lp1, { 0,0,0 },car.pos });
                                ri_l.push_back({ lp2, splitP, { 0,0,0 },car.pos });
                                ri_l.push_back({ splitP, lp3, col,car.pos });
                            }
                        }






                    }


                   // glLineWidth(10);

                    for (Car::Engine& engine : std::vector<Car::Engine>{ car.eBl,car.eBr,car.eFr,car.eFl }) {
                        glm::mat4 eModel = glm::translate(model, engine.relPos + engine.orient * .1f);
                        eModel *= glm::toMat4(glm::rotation(glm::vec3{ 0,-1,0 }, engine.orient));
                        eModel *= glm::toMat4(glm::angleAxis(3.14f / 2, vec3{ 0,1,0 }));
                        if (car.secondPlayer)
                            ri_m.push_back({ "engine2",eModel });
                        else if (car.playerCar)
                            ri_m.push_back({ "engine",eModel });
                          //  RenderMesh(meshes["engine"], shaders["PlanetShader"], eModel);
                        else
                            ri_m.push_back({ "engineE",eModel });
                         //  RenderMesh(meshes["engineE"], shaders["PlanetShader"], eModel);




                    }







                    //glLineWidth(1);
                }
            }

            // translation PID
            if (car.enablePID)
            {
                vec3 Err = car.userPos - car.pos;

                car.pidI += Err * deltaTimeSeconds;

                vec3 P = Err * 10.0f;
                vec3 D = car.vel * -10.0f;
                vec3 I = car.pidI * 10.0f;

                car.desiredAccel = P + D + I;

                for (Car::Engine* ep : std::vector<Car::Engine*>{ &car.eBl, &car.eBr, &car.eFr, &car.eFl, &car.eYl, &car.eYr })
                {
                    Car::Engine& engine = *ep;
                    float dot = glm::dot(car.desiredAccel, engine.actualOrient);
                    engine.queuedAccel += dot;
                    //engine.applyForce(dot, deltaTimeSeconds, car);
                }
            }

            // rotation PID
            {

                vec3 upDir = { 0,1,0 };
                upDir = glm::rotate(glm::inverse(car.rot), upDir);
                vec3 rotAx = glm::cross({ 0,1,0 }, upDir);


                if (glm::length2(rotAx) > 0)
                    if (glm::dot(upDir, { 0,1,0 }) < 0)
                        rotAx = glm::normalize(rotAx) * 2.f - rotAx;


                vec3 fwdDir = glm::rotate(glm::inverse(car.rot), carFwd);
                vec3 rotAxDir = glm::cross({ 1,0,0 }, fwdDir);

                if (glm::length2(rotAxDir) > 0) {
                    if (glm::dot(fwdDir, { 1,0,0 }) < 0)
                        rotAxDir = glm::normalize(rotAxDir) * 2.f - rotAxDir;
                }


                vec3 P = rotAx * 10.0f + rotAxDir * 25.0f;
                vec3 D = car.angularRot * -10.0f;


                car.desiredAngularAccel = P + D;
                vec3 currentAngularAccel = {};

                for (int i = 0; i < 10; i++) {
                    vec3 err = (car.desiredAngularAccel - currentAngularAccel);

                    for (Car::Engine* ep : std::vector<Car::Engine*>{ &car.eBl, &car.eBr, &car.eFr, &car.eFl, &car.eYl, &car.eYr })
                    {
                        Car::Engine& engine = *ep;
                        vec3 engineAngular = glm::cross(engine.relPos, engine.orient);
                        float dot = glm::dot(err, glm::normalize(engineAngular)) * .5f;
                        engine.queuedAccel += dot / glm::length(engineAngular);
                        vec3 angAdd = dot * glm::normalize(engineAngular);
                        currentAngularAccel += angAdd;
                    }

                }

            }

            //force application
            for (Car::Engine* ep : std::vector<Car::Engine*>{ &car.eBl, &car.eBr, &car.eFr, &car.eFl, &car.eYl, &car.eYr })
            {
                Car::Engine& engine = *ep;

                engine.applyForce(engine.queuedAccel, deltaTimeSeconds, car);
                //DebugLine(engine.relPos, engine.relPos + glm::normalize(glm::cross(engine.relPos, engine.orient)), { 0,1,1 });

            }
        }

        if (car.playerCar && !generatingTrack) {
            vec3 carPlanePos = { car.pos.x, 0, car.pos.z };
            //distance to track
            {
                float minDist = 1000.f;

                for (int i = 0; i < trackPoints.size(); i++) {
                    auto p1 = trackPoints[i];
                    auto p2 = trackPoints[(i + 1) % trackPoints.size()];
                    vec3 segment = p2 - p1;
                    float segmentLen2 = glm::length2(segment);
                    //vec3 segmentN = segment / segmentL;

                    vec3 ltoPos = carPlanePos - p1;
                    float projectLenS = glm::dot(segment, ltoPos);
                    
                    float dist = 0;

                    if (projectLenS < 0)
                        dist = glm::length(carPlanePos - p1);
                    else if (projectLenS > segmentLen2)
                        dist = glm::length(carPlanePos - p2);
                    else {
                        vec3 projectPoint = segment * projectLenS / segmentLen2;
                        dist = glm::length(carPlanePos - projectPoint);
                    }
                    if (dist < minDist)
                        minDist = dist;

                }



                if (minDist > 10.5f)
                    car.timeOut -= deltaTimeSeconds * sqrt(minDist - 10.5f);
                else if (car.enablePID)
                    car.timeOut += deltaTimeSeconds;

                if (car.timeOut > 2.f)
                    car.timeOut = 2.f;
                if (car.timeOut < 0.f)
                {
                    car.timeOut = 0.f;
                    car.enablePID = false;
                }
            }
        }

        //ground collision
        if (!generatingTrack)
        {
            float i = (car.pos.x + 300.f) / 600.f * 50.f;
            float j = (car.pos.z + 300.f) / 600.f * 50.f;

            vec3 tri[3] = {};

            car.grounded = false;
                
            if (i < 0 || j < 0 || (int)i >= 49 || (int)j >= 49)
                return;

            int index0;
            if (i - (int)i + j - (int)j > 1)
                index0 = (int)(i + 1) * 50 + (int)(j + 1);
            else
                index0 = (int)i * 50 + (int)j;


            int index1 = (int)(i + 1) * 50 + (int)j;
            int index2 = (int)i * 50 + (int)(j + 1);
            tri[0] = meshes["ground"]->vertices[index0].position;
            tri[1] = meshes["ground"]->vertices[index1].position;
            tri[2] = meshes["ground"]->vertices[index2].position;

            vec3 colPos = car.pos;
            //colPos.y -= .5f;

            if (tri[0].y < colPos.y && tri[1].y < colPos.y && tri[2].y < colPos.y)
                return;

            vec3 normal = glm::normalize(glm::cross(tri[1]-tri[0],tri[2]-tri[0]));
            if (glm::dot(normal, vec3{ 0,1,0 }) < 0)
                normal *= -1;

            float pen = -glm::dot(colPos - tri[0], normal);

            if (pen > 0) {
                car.pos += normal * pen;
                car.vel -= car.vel * deltaTimeSeconds * 10.f;
                auto ricoVel = -normal * glm::dot(normal, car.vel) * 1.1f;
                car.vel += ricoVel;
                car.userPos += ricoVel;
                car.grounded = true;
            }
                
                        

                

        }
        

    }
}

void m1::Tema2::RenderScene(Car& car)
{
    *GetSceneCamera() = car.playerCamera;

    glUseProgram(shaders["PlanetShader"]->program);
    glUniform3fv(carPosLoc, 1, glm::value_ptr(car.pos));
    glUniform3fv(bgColorLoc, 1, glm::value_ptr(bgColor));

    glUseProgram(shaders["DustShader"]->program);
    glUniform3fv(carPosLoc2, 1, glm::value_ptr(car.pos));
    glUniform3fv(bgColorLoc2, 1, glm::value_ptr(bgColor));

    if (!generatingTrack)
        glLineWidth(2);
    else
        glLineWidth(10);

    glPointSize(1);
    for (const auto& r : ri_m) {
        RenderMesh(meshes[r.mesh], shaders["PlanetShader"], r.model);
    }

    glLineWidth(3);
    RenderMesh(meshes["warningLines"], shaders["VertexColor"], glm::mat3(1));


    glUseProgram(car.env->shaders["DustShader"]->program);

    for (const auto& r : ri_d) {
        float distToPlayer = glm::length(car.pos - r.carPos);
        int size = max(1.f, 5 - sqrt(distToPlayer / 10.f));
        glPointSize(size);
        glUniform1f(car.env->timeLoc, r.tss);
        glUniform1f(car.env->extraTimeLoc, r.tse);
        glUniform3fv(car.env->carVelLoc2, 1, glm::value_ptr(r.carVel));
        RenderMesh(meshes[r.mesh], shaders["DustShader"], r.dustMat);
    }

    for (const auto& r : ri_l) {
        float distToPlayer = glm::length(car.pos - r.carPos);
        int size = max(1.f, 5 - sqrt(distToPlayer / 10.f));
        glLineWidth((float)size * r.scale);

        DebugLine(r.orig, r.tip, r.col);
    }

    glLineWidth(3);

    if (showTime) {
        string text;
        text = "lap: " + std::to_string(car.lapTime);
        tr->RenderText(text, 0, 0, .3);
        text = "prv: " + std::to_string(car.prevLapTime);
        tr->RenderText(text, 0, 50, .3);
        text = "bst: " + std::to_string(car.bestLapTime);
        tr->RenderText(text, 0, 100, .3);

    }
}

void m1::Tema2::RenderMiniMap(Car& car)
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    if (!car.secondPlayer)
        glViewport(resolution.x - miniViewportArea.x - miniViewportArea.width, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    else
        glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    glLineWidth(1);

    glUseProgram(shaders["PlanetShader"]->program);
    glUniform3fv(carPosLoc, 1, glm::value_ptr(car.pos));

    glm::vec3 currentCarFwd = glm::rotate(car.rot, { 1,0,0 });
    glm::vec3 minimapOrient = glm::normalize(currentCarFwd + car.vel);

    glm::quat cameraRot = glm::quatLookAt(glm::vec3{ 0,-1,0 }, minimapOrient);
    //glm::quat cameraRot = glm::quatLookAt(glm::vec3{ 0,-1,0 }, carFwd);

    secondCamera.SetPositionAndRotation(car.pos + glm::vec3{ 0,50,0 } + minimapOrient * 20.f, cameraRot);
    //gfxc::Camera fc = *GetSceneCamera();

    *GetSceneCamera() = secondCamera;

    {

        RenderMesh(meshes["trackLine"], shaders["PlanetShader"], glm::mat3(1));

        RenderMesh(meshes["ground"], shaders["PlanetShader"], glm::mat3(1));

        RenderMesh(meshes["track"], shaders["PlanetShader"], glm::mat3(1));

        glPointSize(3);

        RenderMesh(meshes["warningPoints"], shaders["VertexColor"], glm::mat3(1));

        DebugLine({ 0,1,-12 }, { 0,1,12 }, { 1,.5,0 });


        for (auto carp : allCars) {
            Car& car = *carp;

            glm::mat4 model = glm::translate(glm::mat4(1), car.pos);

            model *= glm::toMat4(car.rot);
            // model *= glm::toMat4(glm::angleAxis(3.14f / 2, glm::vec3{ 0,0,0 }));
            // RenderMesh(meshes["car"], shaders["VertexColor"], glm::scale(model, glm::vec3{ 1,1,1 }));

            glLineWidth(2);
            float arrHeight = 1.5;
            if (car.playerCar) {
                DebugLine(model * glm::vec4{ -1,arrHeight,2,1 }, model * glm::vec4(3, arrHeight, 0, 1), { 1,1,1 });
                DebugLine(model * glm::vec4{ -1,arrHeight,-2,1 }, model * glm::vec4(3, arrHeight, 0, 1), { 1,1,1 });
            }
            else {
                arrHeight = glm::length2(this->car.pos - car.pos) * 0.0005;
                DebugLine(model * glm::vec4{ -1,arrHeight,2,1 }, model * glm::vec4(3, arrHeight, 0, 1), { 1,.5,0 });
                DebugLine(model * glm::vec4{ -1,arrHeight,-2,1 }, model * glm::vec4(3, arrHeight, 0, 1), { 1,.5,0 });
            }
        }



    }
    //*GetSceneCamera() = fc;
}

void m1::Tema2::ResetCar(Car& car)
{
    float prev = car.prevLapTime;
    float best = car.bestLapTime;
    car = Car{ this };
    // car.playerCamera = pc;
    car.prevLapTime = prev;
    car.bestLapTime = best;
}

void m1::Tema2::CarInputKeyboard(Car &car, float keyCodeW, float keyCodeS, float keyCodeA, float keyCodeD, float keyCodeShift, float deltaTime)
{
    float desiredSpeed = 0;
    car.key_W = false;
    car.key_A = false;
    car.key_D = false;
    car.key_SHIFT = false;


    if (keyCodeW > 0.1 && Engine::GetElapsedTime() > .1f) {

        //car.eBl.applyForce(100, deltaTime, car);


        desiredSpeed = 20;
        if (glm::length(car.vel) > 19)
            desiredSpeed = glm::length(car.vel) + 1;
        if (desiredSpeed > 30)
            desiredSpeed = 30;
        desiredSpeed *= keyCodeW;
        car.key_W = true;
    }

    if (keyCodeS>0.1) {
        desiredSpeed -= 5 * keyCodeS;
    }


    if (permaBoost && keyCodeShift < 0.1) {
        desiredSpeed *= 1.5f;
        car.key_SHIFT = true;
    }

    if (keyCodeShift>0.1 && !permaBoost) {
        desiredSpeed *= 1.5f * keyCodeShift;
        car.key_SHIFT = true;
    }



    float speedErr = (desiredSpeed - car.fwdSpeed) * deltaTime * 20.f;
    if (speedErr < 0)
        speedErr /= 20.f;
    car.fwdSpeed += speedErr;

    float turnSpeed = 2;

    turnSpeed += 1 / (1 + glm::length(car.vel));

    if (keyCodeA>0.1) {
        car.heading += deltaTime * turnSpeed * keyCodeA;
        car.key_A = true;
    }
    if (keyCodeD>0.1) {
        car.heading -= deltaTime * turnSpeed * keyCodeD;
        car.key_D = true;
    }

}

void m1::Tema2::DebugLine(glm::vec3 orig, glm::vec3 tip, glm::vec3 color)
{
    Mesh* line = nullptr;
    float code = color.x * 100 + color.y * 10 + color.z;
    if (lines.count(code))
        line = lines[code];
    else
    {
        vector<VertexFormat> ver = { {{0,0,0}, color}, {{1,0,0}, color} };
        vector<unsigned int > indx = { 0,1 };

        line = new Mesh("generatedLine");
        line->SetDrawMode(GL_LINES);
        line->InitFromData(ver, indx);
        lines[code] = line;
    }

    glm::mat4 skew = glm::identity<glm::mat4>();
    skew[0][0] = tip.x - orig.x;
    skew[0][1] = tip.y - orig.y;
    skew[0][2] = tip.z - orig.z;

    glm::mat4 lineModel = glm::translate(glm::mat4(1), orig);
    lineModel *= skew;
    RenderMesh(line, shaders["PlanetShader"], lineModel);

    

  //  ok->GetBuffers()->ReleaseMemory();
    //delete ok;
}

void m1::Tema2::CarUpdateWithTimer(Car& car, float deltaTimeSeconds)
{
    bool closeToStart = (car.pos.z < 15 && car.pos.z > -15 && car.pos.x > -5 && car.pos.x < 5);
    if (!closeToStart)
        car.lapTime += deltaTimeSeconds;

    bool wasBehindStart = generatingTrack || glm::dot(car.pos - trackPoints[0], trackPoints[1] - trackPoints[0]) <= 0;
    UpdateCar(car, deltaTimeSeconds);
    bool isAfterStart = generatingTrack || glm::dot(car.pos - trackPoints[0], trackPoints[1] - trackPoints[0]) >= 0;

    if (wasBehindStart && isAfterStart && closeToStart && car.lapTime > 5.f) {
        car.prevLapTime = car.lapTime;
        car.lapTime = 0;
        if (car.prevLapTime < car.bestLapTime)
            car.bestLapTime = car.prevLapTime;
    }


}

void m1::Tema2::Car::Engine::applyForce(float t, float deltaTime, Car& car)
{
    car.vel += deltaTime * actualOrient * t / 10.f;
    car.angularRot += deltaTime * t * glm::cross(relPos, orient);






    t = .25f;
    if (queuedAccel > 0)
        t += queuedAccel / 100.f;

    if (queuedAccel - prevAccel > 0)
        t += (queuedAccel - prevAccel) / 5.f;

    if (car.key_W) {
        float fwdM = glm::dot({ 1,0,0 }, orient);
        if (fwdM > 0) {
            t += fwdM/2;
            if (car.key_SHIFT)
                t += fwdM/2;
        }
    }

    if (car.key_D) {
        float fwdM = glm::dot({ 0,0,1 }, orient);
        if (fwdM > 0)
            t += fwdM;
    }

    if (car.key_A) {
        float fwdM = glm::dot({ 0,0,-1 }, orient);
        if (fwdM > 0)
            t += fwdM;
    }

    t = glm::clamp(t, 0.f, 3.f);

    if (t > 0) {
        if (str > 5 && (car.enablePID || !car.playerCar)) {
            float distToPlayer = glm::length(car.env->car.pos - car.pos);
            int size = max(1.f, 5 - sqrt(distToPlayer / 10.f));

            if (car.playerCar) {
                for (int i = 0; i < 4; i++) {


                    float colorM = .85f;

                    if (car.secondPlayer)
                        car.env->ri_l.push_back({ car.pos + actualRelPos, car.pos + randV() * t / 10.f + actualRelPos - (actualOrient)*t * randF(), glm::vec3{ .5,1,.95 } *colorM,car.pos,2 });

                    else
                        car.env->ri_l.push_back({ car.pos + actualRelPos, car.pos + randV() * t / 10.f + actualRelPos - (actualOrient)*t * randF(), glm::vec3{ .5,.95,1 }*colorM,car.pos,2 });
                }
            }
            // engine particles
            {
                   // glPointSize(size);

                timeSinceStart += deltaTime;

                if ((t > 2.9 || (t > 2 && !car.playerCar)) && (timeSinceEnd > 1 || timeSinceEnd == 0)) {
                    timeSinceEnd = 0;
                }
                else {
                    timeSinceEnd += deltaTime;
                    if (timeSinceEnd > 1)
                        timeSinceStart = 0;

                    // timeSinceStart = 0;

                }

                if (timeSinceStart > 0) {
                
                    glm::mat4 dustMat = glm::translate(glm::mat4(1), car.pos + actualRelPos);
                    dustMat *= glm::toMat4(glm::rotation({ 0,1,0 }, actualOrient));

                    if (car.secondPlayer)
                        car.env->ri_d.push_back({ timeSinceStart, timeSinceEnd, glm::vec3{ car.vel.x, 0, car.vel.z }, dustMat, "dust2", car.pos });

                    else if (car.playerCar)
                        //car.env->RenderMesh(car.env->meshes["dust"], car.env->shaders["DustShader"], dustMat);
                        car.env->ri_d.push_back({ timeSinceStart, timeSinceEnd, glm::vec3{ car.vel.x, 0, car.vel.z }, dustMat, "dust", car.pos });
                    else
                        car.env->ri_d.push_back({ timeSinceStart, timeSinceEnd, glm::vec3{ car.vel.x, 0, car.vel.z }, dustMat, "dustE", car.pos });

                }

                
            }
        }

        prevAccel += (queuedAccel - prevAccel) * deltaTime;

    }
    queuedAccel = 0;
}
