#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <set>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */



Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}



#define randF() ((float)rand()/RAND_MAX) 
#define randV() glm::vec3(randF()-.5, randF()-.5, randF()-.5)
#define mylerp(a,b,t) (a + (b-a) * t)

void Lab4::Init()
{
    //srand(1);
    polygonMode = GL_FILL;
    secondCamera = new gfxc::Camera(*GetSceneCamera());
    

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;


    {
        Shader* shader = new Shader("PlanetShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab4", "shaders", "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab4", "shaders", "VertexColor.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
        carPosLoc = shaders["PlanetShader"]->GetUniformLocation("CarPos");
    }
    {
        Shader* shader = new Shader("DustShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab4", "shaders", "MVP.Dust.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab4", "shaders", "VertexColor.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
        carPosLoc2 = shaders["DustShader"]->GetUniformLocation("CarPos");
        timeLoc = shaders["DustShader"]->GetUniformLocation("Time");
        carVelLoc2 = shaders["DustShader"]->GetUniformLocation("CarVel");
        extraTimeLoc = shaders["DustShader"]->GetUniformLocation("ExtraTime");


    }

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x - 50 - resolution.y / 5.f, 50, resolution.y / 5.f, resolution.x / 5.f);

    CreateCar();
    CreateEngine();
    CreateRock();
    CreateCube("box", { 1,1,1 });

    car.playerCar = true;

 

    //trackPoints.push_back({ 0,0,0 });
    trackPoints = {
           {0,0,0},
           {5.03751,0,0},
           {10.6909,0,-0.00085943},
           {17.7038,0,-0.156845},
           {25.2942,0,-0.853971},
           {32.9671,0,-2.19162},
           {40.5141,0,-3.9482},
           {47.9673,0,-5.4593},
           {55.3573,0,-5.92706},
           {62.5926,0,-4.88902},
           {69.5756,0,-2.50482},
           {76.2499,0,0.867329},
           {82.6292,0,4.88933},
           {88.7722,0,9.29341},
           {94.7661,0,13.8933},
           {100.682,0,18.5602},
           {106.584,0,23.229},
           {112.473,0,27.8938},
           {117.818,0,32.9619},
           {121.453,0,38.9609},
           {122.671,0,45.6647},
           {121.338,0,52.44},
           {117.774,0,58.6798},
           {112.567,0,64},
           {106.232,0,68.192},
           {99.1829,0,71.1472},
           {91.7805,0,73.1528},
           {84.3356,0,74.7976},
           {76.9338,0,76.2977},
           {69.5436,0,77.734},
           {62.2365,0,79.3401},
           {55.3553,0,81.8935},
           {49.3148,0,85.8772},
           {44.1729,0,91.1043},
           {39.8276,0,97.1837},
           {35.8372,0,103.586},
           {31.4274,0,109.562},
           {26.1337,0,114.659},
           {20.0499,0,118.836},
           {13.4109,0,122.232},
           {6.43662,0,125.04},
           {-0.702325,0,127.452},
           {-7.93925,0,129.607},
           {-15.206,0,130.962},
           {-22.2359,0,130.36},
           {-28.399,0,127.404},
           {-32.7942,0,122.286},
           {-34.778,0,115.742},
           {-34.3524,0,108.603},
           {-32.2787,0,101.344},
           {-29.2022,0,94.2324},
           {-25.3737,0,87.4701},
           {-21.4602,0,80.8817},
           {-18.6463,0,74.0845},
           {-18.0517,0,67.1848},
           {-19.9581,0,60.572},
           {-23.9902,0,54.741},
           {-29.6306,0,49.9888},
           {-36.3078,0,46.4681},
           {-43.5906,0,44.2019},
           {-51.1456,0,43.1381},
           {-58.7376,0,43.1406},
           {-66.2545,0,44.0144},
           {-73.6336,0,45.5362},
           {-80.9034,0,47.4943},
           {-88.1002,0,49.6947},
           {-95.3115,0,51.7709},
           {-102.608,0,53.0093},
           {-109.895,0,52.7368},
           {-117.004,0,50.9459},
           {-123.993,0,48.3107},
           {-131.054,0,45.9374},
           {-138.286,0,44.6012},
           {-145.618,0,44.6168},
           {-152.941,0,45.8115},
           {-160.184,0,47.7758},
           {-167.431,0,49.5548},
           {-174.738,0,50.258},
           {-181.842,0,49.2412},
           {-188.314,0,46.2544},
           {-193.691,0,41.5219},
           {-197.132,0,35.3563},
           {-198.327,0,28.3251},
           {-197.324,0,21.1732},
           {-193.926,0,15.0562},
           {-188.443,0,10.8439},
           {-181.604,0,8.68468},
           {-174.139,0,8.37616},
           {-166.443,0,9.02026},
           {-158.742,0,9.48046},
           {-151.316,0,8.67294},
           {-144.754,0,5.86006},
           {-139.699,0,1.06112},
           {-136.938,0,-5.21667},
           {-136.736,0,-12.2186},
           {-138.743,0,-19.2327},
           {-142.511,0,-25.78},
           {-147.354,0,-31.7925},
           {-152.432,0,-37.6329},
           {-157.073,0,-43.678},
           {-161.044,0,-50.0581},
           {-163.729,0,-56.871},
           {-164.558,0,-64.0321},
           {-163.467,0,-71.1752},
           {-160.352,0,-77.6641},
           {-155.421,0,-82.9292},
           {-149.096,0,-86.3754},
           {-141.929,0,-87.8658},
           {-134.409,0,-87.7478},
           {-126.83,0,-86.6706},
           {-119.381,0,-84.9138},
           {-112.134,0,-82.5871},
           {-105.031,0,-80.0776},
           {-97.9186,0,-78.5875},
           {-91.101,0,-79.2954},
           {-85.3655,0,-82.5755},
           {-81.5813,0,-88.0252},
           {-80.1702,0,-94.8069},
           {-80.9412,0,-102.089},
           {-83.5418,0,-109.221},
           {-87.267,0,-116.033},
           {-90.9557,0,-122.809},
           {-93.6089,0,-129.84},
           {-94.5884,0,-137.094},
           {-93.8507,0,-144.395},
           {-91.7103,0,-151.506},
           {-88.0958,0,-157.873},
           {-82.8452,0,-162.647},
           {-76.4125,0,-165.069},
           {-69.8544,0,-164.443},
           {-64.5478,0,-160.777},
           {-61.0191,0,-154.876},
           {-58.6518,0,-147.763},
           {-57.0474,0,-140.111},
           {-55.704,0,-132.374},
           {-54.4042,0,-124.739},
           {-52.6997,0,-117.373},
           {-50.1666,0,-110.341},
           {-47.5662,0,-103.415},
           {-46.084,0,-96.3515},
           {-46.1927,0,-89.1473},
           {-47.3568,0,-81.8737},
           {-49.0914,0,-74.6029},
           {-50.6897,0,-67.3041},
           {-51.1137,0,-60.0041},
           {-49.8112,0,-52.9166},
           {-47.1114,0,-46.0742},
           {-44.1035,0,-39.2597},
           {-40.9947,0,-32.4947},
           {-37.7652,0,-26.4747},
           {-35.1974,0,-22.1739},
           {-32.3192,0,-18.0835},
           {-29.1258,0,-14.1877},
           {-25.939,0,-10.258},
           {-22.8236,0,-6.34641},
           {-15.6964,0,-2.42189},
           { -10.6964,0,-1.42189 },
           { -5.6964,0,-0.42189 },


    };
    //trackGeneration
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

        //line & rocks
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


                    float height = rand() % 10;
                    float colorM = (height / 10.f) / 2.f + .5f;



                    ver.push_back({ {x,height,y} ,glm::vec3{.75,.4,.2} *colorM });
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
                        v.color = glm::vec3{ .75,.4,.2 } *(.5f + randF() / 8);
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
                    if (rand() % 4 == 0) {
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
                    if (cross < 0) {
                        sideDir *= -1;

                        
                    }
                    ver.push_back({ trackPoints[i] + sideDir + glm::vec3(0, 0, 0), { 1,.5,0 } });
                    ver.push_back({ trackPoints[i] + sideDir + glm::vec3(0, 1.5, 0), { 1,.5,0 } });

                    ver.push_back({ trackPoints[i] + sideDir - toNext*.4f + glm::vec3(0, 2, 0), { 1,.5,0 } });
                    ver.push_back({ trackPoints[i] + sideDir + toNext*.6f + glm::vec3(0, 2.5, 0), { 1,.5,0 } });
                    ver.push_back({ trackPoints[i] + sideDir - toNext*.4f + glm::vec3(0, 3, 0), { 1,.5,0 } });
                    pver.push_back({ trackPoints[i] + sideDir + glm::vec3(0, 3, 0), { 1,.5,0 } });

                    indx.push_back(ver.size() - 5);
                    indx.push_back(ver.size() - 4);
                    indx.push_back(ver.size() - 3);
                    indx.push_back(ver.size() - 2);
                    indx.push_back(ver.size()-2);
                    indx.push_back(ver.size()-1);
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
                glm::vec3 pos = glm::vec3{cos(angle),0,sin(angle)} * .1f * randF();
                glm::vec3 col = glm::vec3{ .5,.95,1 } * (randF() * .25f + .75f);
                glm::vec3 norm = glm::normalize(glm::vec3{0,1,0} + pos * randF() * 2.f);
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
        }
    }

    std::set<int> usedpoints = { 0 };

    // enemy cars
    for (int i = 0; i < 20; i++)
    {
        Car newCar = Car{ this };
        newCar.cpOffset = 1 + rand() % 7;
        newCar.autoPilotspeed = 15 + randF() * 25;
        int nextCpIndex = rand() % trackPoints.size();
        while (usedpoints.count(nextCpIndex))
            nextCpIndex = rand() % trackPoints.size();
        usedpoints.insert(nextCpIndex);
        glm::vec3 startPoint = trackPoints[nextCpIndex];
        newCar.pos = startPoint;
        newCar.userPos = startPoint;
        newCar.nextCp = nextCpIndex;
        newCar.sideOffset = randF() * 5.f - 2.5f;
        newCar.desiredHeight = 4.f - randF() + randF();
        enemyCars.push_back(newCar);

        
    }

    for (Car& car : enemyCars)
    {
        allCars.push_back(&car);
    }
    allCars.push_back(&car);

    GetSceneCamera()->SetPerspective(60, 16.f / 9.f, 0.1, 1000);
    secondCamera->SetOrthographic(9 * 16, 16 * 16, 0, 500);
}


void Lab4::FrameStart()
{
    
    GetSceneCamera()->SetPerspective(60 + glm::length(car.vel), 16.f / 9.f, 0.1, 1000);



    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(.7, .6, .6, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    
    
}

static float t1, t2, t3, t4;

void Lab4::Update(float deltaTimeSeconds)
{
    using vec3 = glm::vec3;
    glLineWidth(3);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    deltaTimeSeconds = glm::clamp(deltaTimeSeconds, .0f, .1f);

    //autopilot (work in progress)
    /*
    if (Engine::GetElapsedTime() > 1.f)
    {



        glm::vec3 planeUserPos = { car.userPos.x, 0, car.userPos.z };
        glm::vec3 cpHeading = glm::normalize(trackPoints[(car.nextCp + 1) % trackPoints.size()] - trackPoints[car.nextCp]);

        if (glm::dot(cpHeading, planeUserPos - trackPoints[car.nextCp]) > 0) {
            car.nextCp++;
            car.nextCp %= trackPoints.size();
        }
        int tpOffset = 4;

        glm::vec3 nextCp = trackPoints[(car.nextCp + tpOffset) % trackPoints.size()];
        glm::vec3 toNextCp = glm::normalize(nextCp - planeUserPos);
        glm::vec3 nextHeading = trackPoints[(car.nextCp + tpOffset + 1) % trackPoints.size()] - trackPoints[(car.nextCp + tpOffset) % trackPoints.size()];
        car.userPos += toNextCp * deltaTimeSeconds * 20.f;
        car.heading = -atan2(nextHeading.z, nextHeading.x);
        DebugLine(car.pos, car.userPos, { 1,0,0 });

       
    }
    */
    
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
            {
                static vec3 oldCameraPos{};
                static vec3 oldLookAtPoint{};

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


                GetSceneCamera()->SetPositionAndRotation(newCameraPos, cameraRot);

                oldCameraPos = newCameraPos;
            }

            // physics update
            {
                if (!grounded)
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
            
            // enviorment rendering
            {
                glUseProgram(shaders["PlanetShader"]->program);
             
                glUniform3fv(carPosLoc, 1, glm::value_ptr(car.pos));
              
                glUseProgram(shaders["DustShader"]->program);
                glUniform3fv(carPosLoc2, 1, glm::value_ptr(car.pos));
                

                RenderMesh(meshes["trackLine"], shaders["PlanetShader"], glm::mat3(1));

                RenderMesh(meshes["ground"], shaders["PlanetShader"], glm::mat3(1));

                RenderMesh(meshes["track"], shaders["PlanetShader"], glm::mat3(1));

                glPointSize(2);
                glm::mat4 model = glm::translate(glm::mat4(1), car.pos);
                model *= glm::toMat4(car.rot);

                for (Rock& r : rocks) {
                    glm::mat4 rmodel = glm::translate(glm::mat4(1), r.pos);
                    rmodel = glm::scale(rmodel, vec3(r.scale) );
                    rmodel = glm::rotate(rmodel, r.orient, { 0,1,0 });
                    RenderMesh(meshes["rock"], shaders["PlanetShader"], rmodel);

                }
                if (miniMap)

                RenderMesh(meshes["warningLines"], shaders["VertexColor"], glm::mat3(1));


            }
        }

        // car rendering
        {
            glm::mat4 model = glm::translate(glm::mat4(1), car.pos);

            model *= glm::toMat4(car.rot);
            RenderMesh(meshes["car"], shaders["PlanetShader"], glm::scale(model, vec3{ 1,1,1 }));

            glm::mat4 shadowModel = glm::translate(glm::mat4(1), { car.pos.x,.25,car.pos.z });
            shadowModel = glm::scale(shadowModel, { 1,.001f,1 });
            shadowModel *= glm::toMat4(car.rot);

            RenderMesh(meshes["carS"], shaders["PlanetShader"], shadowModel);

            //engine rendering and indicator
            {
                // engine positions
                {
                    car.eBl.actualOrient = glm::rotate(car.rot, car.eBl.orient);
                    car.eBl.actualRelPos = glm::rotate(car.rot, car.eBl.relPos);
                    car.eBr.actualOrient = glm::rotate(car.rot, car.eBr.orient);
                    car.eBr.actualRelPos = glm::rotate(car.rot, car.eBr.relPos);
                    car.eFl.actualOrient = glm::rotate(car.rot, car.eFl.orient);
                    car.eFl.actualRelPos = glm::rotate(car.rot, car.eFl.relPos);
                    car.eFr.actualOrient = glm::rotate(car.rot, car.eFr.orient);
                    car.eFr.actualRelPos = glm::rotate(car.rot, car.eFr.relPos);
                    car.eYl.actualOrient = glm::rotate(car.rot, car.eYl.orient);
                    car.eYl.actualRelPos = glm::rotate(car.rot, car.eYl.relPos);
                    car.eYr.actualOrient = glm::rotate(car.rot, car.eYr.orient);
                    car.eYr.actualRelPos = glm::rotate(car.rot, car.eYr.relPos);
                }

                //indicator
                {
                    const vec3 lp1c = { 1.95666,-0.297439,1.3395 + .01 };
                    const vec3 lp2c = { -0.252516,-0.32545,1.00192 + .01 };
                    const vec3 lp3c = { -1.49389, -0.305302,0.470492 + .01 };


                    const float len1 = glm::length(lp3c - lp2c);
                    const float len2 = glm::length(lp2c - lp1c);
                    const float totLen = len1 + len2;

                    for (int i = -1; i <= 1; i += 2) {

                        vec3 lp1 = lp1c, lp2 = lp2c, lp3 = lp3c;
                        lp1.z *= i;
                        lp2.z *= i;
                        lp3.z *= i;


                        lp1 = model * glm::vec4(lp1, 1);
                        lp2 = model * glm::vec4(lp2, 1);
                        lp3 = model * glm::vec4(lp3, 1);

                        float fillLen = totLen * timeOut / 2.f;
                        glLineWidth(6);

                        if (fillLen > totLen - .1f) {
                            DebugLine(lp1, lp2, { .5,.95,1 });
                            DebugLine(lp2, lp3, { .5,.95,1 });
                        }

                        else if (fillLen > len1) {
                            float t = (fillLen - len1) / len2;

                            vec3 splitP = mylerp(lp2, lp1, t);

                            DebugLine(splitP, lp1, { 0,0,0 });
                            DebugLine(lp2, splitP, { .5,.95,1 });
                            DebugLine(lp2, lp3, { .5,.95,1 });
                        }

                        else {
                            float t = fillLen / len1;

                            vec3 splitP = mylerp(lp3, lp2, t);

                            DebugLine(lp2, lp1, { 0,0,0 });
                            DebugLine(lp2, splitP, { 0,0,0 });
                            DebugLine(splitP, lp3, { .5,.95,1 });
                        }
                    }






                }


                glLineWidth(10);

                for (Car::Engine& engine : std::vector<Car::Engine>{ car.eBl,car.eBr,car.eFr,car.eFl }) {
                    glm::mat4 eModel = glm::translate(model, engine.relPos + engine.orient * .1f);
                    eModel *= glm::toMat4(glm::rotation(glm::vec3{ 0,-1,0 }, engine.orient));
                    eModel *= glm::toMat4(glm::angleAxis(3.14f / 2, vec3{ 0,1,0 }));
                    RenderMesh(meshes["engine"], shaders["VertexColor"], eModel);

                   
                    

                    

                }







                glLineWidth(1);
            }
        }
        
        // translation PID
        if(enablePID)  
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

    vec3 carPlanePos = { car.pos.x, 0, car.pos.z };

    //distance to track
    {
        float minDist = 1000.f;

        for (int i = 0; i < trackPoints.size(); i++) {
            auto p1 = trackPoints[i];
            auto p2 = trackPoints[(i + 1) % trackPoints.size()];
            vec3 segment = p2 - p1;
            float segmentL = glm::length(segment);
            vec3 segmentN = segment / segmentL;

            vec3 ltoPos = carPlanePos - p1;
            float projectLen = glm::dot(segmentN, ltoPos);
            vec3 projectPoint = segmentN * projectLen;
            float dist = 0;

            if (projectLen < 0)
                dist = glm::length(carPlanePos - p1);
            else if (projectLen > glm::length(segment))
                dist = glm::length(carPlanePos - p2);
            else
                dist = glm::length(carPlanePos - projectPoint);

            if (dist < minDist)
                minDist = dist;

        }



        if (minDist > 10.5f)
            timeOut -= deltaTimeSeconds * sqrt(minDist - 10.5f);
        else if (enablePID)
            timeOut += deltaTimeSeconds;

        if (timeOut > 2.f)
            timeOut = 2.f;
        if (timeOut < 0.f)
        {
            timeOut = 0.f;
            enablePID = false;
        }
    }

    //ground collision
    {
        float i = (car.pos.x + 300.f) / 600.f * 50.f;
        float j = (car.pos.z + 300.f) / 600.f * 50.f;
        std::vector<int> closeGroundIndices;

        for (int a = -1; a <= 1; a++)
            for (int b = -1; b <= 1; b++)
            {
                int index = (int)(i + a) * 50 + (int)(b + j);
                if (index < 0 || index >= meshes["ground"]->vertices.size())
                    break;
                closeGroundIndices.push_back(index);

            }

        grounded = false;

        for (int i : closeGroundIndices) {
            auto pos = meshes["ground"]->vertices[i].position;
            if (glm::length2(pos - car.pos) < 100)
            {
                int line = i / 50;
                int col = i % 50;
                if (line == 0 || col == 0 || line == 49 || col == 49)
                    break;

                vector<glm::vec3> closePoints;
                for (int j = line - 1; j <= line + 1; j++)
                    for (int k = col - 1; k <= col + 1; k++)
                        closePoints.push_back(meshes["ground"]->vertices[j * 50 + k].position);

                std::sort(closePoints.begin(), closePoints.end(), [&](glm::vec3 a, vec3 b) {
                    return (glm::length2(a - car.pos) < glm::length2(b - car.pos));
                    });

                vec3 norm = glm::normalize(glm::cross(closePoints[1] - closePoints[0], closePoints[2] - closePoints[0]));
                if (glm::dot(norm, { 0,1,0 }) < 0)
                    norm *= -1;
                
              /*
                DebugLine(closePoints[1], closePoints[0], { 1,1,1 });
                DebugLine(closePoints[2], closePoints[1], { 1,1,1 });
                DebugLine(closePoints[0], closePoints[2], { 1,1,1 });
                DebugLine((closePoints[1]+closePoints[2]+closePoints[0])/3.f, (closePoints[1] + closePoints[2] + closePoints[0]) / 3.f+norm, { 1,1,1 });
                */
               
                if (closePoints[0].y < 0.1f && closePoints[1].y < 0.1f && closePoints[2].y < 0.3f)
                    continue;

                if (glm::dot(car.pos - closePoints[0], norm) <= 0)
                {
                    car.pos += norm * -glm::dot(car.pos - closePoints[0], norm) * .5f;
                    auto ricoVel = -norm * glm::dot(car.vel, norm) * 1.5f;
                    car.userPos += ricoVel;
                    car.vel += ricoVel;
                    car.vel -= car.vel * deltaTimeSeconds * .5f;
                    grounded = true;
                }
            }
        }
    }
 
    //enemy car
    for (Car& car : enemyCars)
    {
        


         if (Engine::GetElapsedTime() > 2.f )
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
             car.heading = -atan2(nextHeading.z, nextHeading.x);
           //  DebugLine(car.pos, car.userPos, { 1,0,0 });


         }


      
        glLineWidth(3);



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
                

                // physics update
                {
                    if (!grounded)
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
                RenderMesh(meshes["carE"], shaders["PlanetShader"], glm::scale(model, vec3{ 1,1,1 }));

                glm::mat4 shadowModel = glm::translate(glm::mat4(1), { car.pos.x,.25,car.pos.z });
                shadowModel = glm::scale(shadowModel, { 1,.001f,1 });
                shadowModel *= glm::toMat4(car.rot);

                RenderMesh(meshes["carS"], shaders["PlanetShader"], shadowModel);

                //engine rendering and indicator
                {
                    // engine positions
                    {
                        car.eBl.actualOrient = glm::rotate(car.rot, car.eBl.orient);
                        car.eBl.actualRelPos = glm::rotate(car.rot, car.eBl.relPos);
                        car.eBr.actualOrient = glm::rotate(car.rot, car.eBr.orient);
                        car.eBr.actualRelPos = glm::rotate(car.rot, car.eBr.relPos);
                        car.eFl.actualOrient = glm::rotate(car.rot, car.eFl.orient);
                        car.eFl.actualRelPos = glm::rotate(car.rot, car.eFl.relPos);
                        car.eFr.actualOrient = glm::rotate(car.rot, car.eFr.orient);
                        car.eFr.actualRelPos = glm::rotate(car.rot, car.eFr.relPos);
                        car.eYl.actualOrient = glm::rotate(car.rot, car.eYl.orient);
                        car.eYl.actualRelPos = glm::rotate(car.rot, car.eYl.relPos);
                        car.eYr.actualOrient = glm::rotate(car.rot, car.eYr.orient);
                        car.eYr.actualRelPos = glm::rotate(car.rot, car.eYr.relPos);
                    }

                

                    glLineWidth(10);

                    for (Car::Engine& engine : std::vector<Car::Engine>{ car.eBl,car.eBr,car.eFr,car.eFl }) {
                        glm::mat4 eModel = glm::translate(model, engine.relPos + engine.orient * .1f);
                        eModel *= glm::toMat4(glm::rotation(glm::vec3{ 0,-1,0 }, engine.orient));
                        eModel *= glm::toMat4(glm::angleAxis(3.14f / 2, vec3{ 0,1,0 }));
                        RenderMesh(meshes["engineE"], shaders["PlanetShader"], eModel);






                    }







                    glLineWidth(1);
                }
            }

            // translation PID
            if (true)
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

                if (car2.playerCar)
                    timeOut -= abs(glm::dot(norm, -relVel))/15.f;

            }
        }
    }
}


void Lab4::FrameEnd()
{
    glLineWidth(1);

    if (miniMap) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);



        glm::vec3 currentCarFwd = glm::rotate(car.rot, { 1,0,0 });
        glm::vec3 minimapOrient = glm::normalize(currentCarFwd + car.vel);

        glm::quat cameraRot = glm::quatLookAt(glm::vec3{ 0,-1,0 }, minimapOrient);
        //glm::quat cameraRot = glm::quatLookAt(glm::vec3{ 0,-1,0 }, carFwd);

        secondCamera->SetPositionAndRotation(car.pos + glm::vec3{ 0,50,0 } + minimapOrient * 20.f, cameraRot);
        gfxc::Camera fc = *GetSceneCamera();

        *GetSceneCamera() = *secondCamera;

        {

            RenderMesh(meshes["trackLine"], shaders["PlanetShader"], glm::mat3(1));

            RenderMesh(meshes["ground"], shaders["PlanetShader"], glm::mat3(1));

            RenderMesh(meshes["track"], shaders["PlanetShader"], glm::mat3(1));

            glPointSize(3);

            RenderMesh(meshes["warningPoints"], shaders["VertexColor"], glm::mat3(1));


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
        *GetSceneCamera() = fc;
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    
    

    /*if (window->KeyHold(GLFW_KEY_A))
        car.heading +=deltaTime * 2;
    else if (window->KeyHold(GLFW_KEY_D))
        car.heading -=deltaTime * 2;*/
    
    float desiredSpeed = 0;
    if (window->KeyHold(GLFW_KEY_W)) {

        //car.eBl.applyForce(100, deltaTime, car);

       
        desiredSpeed = 20;
        if (glm::length(car.vel) > 19)
            desiredSpeed = glm::length(car.vel) + 1;
        if (desiredSpeed > 30)
            desiredSpeed = 30;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        desiredSpeed -= 5;
    }
    
    deltaTime = glm::clamp(deltaTime, 0.f, 1.f);

    if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
        desiredSpeed *= 1.5f;
    }



    float speedErr = (desiredSpeed - car.fwdSpeed) * deltaTime * 20.f;
    if (speedErr < 0)
        speedErr /= 20.f;
    car.fwdSpeed += speedErr;
    
    float turnSpeed = 2;

    turnSpeed += 1 /(1+glm::length(car.vel));

    if (window->KeyHold(GLFW_KEY_A)) {
        car.heading += deltaTime * turnSpeed;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        car.heading -= deltaTime * turnSpeed;
    }
    
    car.lrSpeed = 0;
    if (window->KeyHold(GLFW_KEY_Q)) {
        car.lrSpeed += 5;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        car.lrSpeed -= 5;
    }
    
    if (window->KeyHold(GLFW_KEY_UP)) {
        t1 += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        t1 -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        t2 += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        t2-= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_I)) {
        t3 += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_K)) {
        t3 -= deltaTime;
    }

}


void Lab4::OnKeyPress(int key, int mods)
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


    if (key==GLFW_KEY_O){
        cout << endl << endl << endl;
        for (int i = 0; i < trackPoints.size(); i++) {
            cout << "{" << trackPoints[i].x << "," << trackPoints[i].y << "," << trackPoints[i].z << "}" << endl;
        }
    }
    if (key == GLFW_KEY_X) {
        miniMap = !miniMap;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}

void m1::Lab4::CreateCube(std::string name, glm::vec3 color)
{
    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0, 1) + glm::vec3(-.5), color * .9f),
            // TODO(student): Complete the vertices data for the cube mesh
            VertexFormat(glm::vec3(1, 0, 1) + glm::vec3(-.5), color * .8f),
            VertexFormat(glm::vec3(0, 1, 1) + glm::vec3(-.5), color),
            VertexFormat(glm::vec3(1, 1, 1) + glm::vec3(-.5), color * .8f),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(-.5), color * .9f),
            VertexFormat(glm::vec3(1, 0, 0) + glm::vec3(-.5), color),
            VertexFormat(glm::vec3(0, 1, 0) + glm::vec3(-.5), color),
            VertexFormat(glm::vec3(1, 1, 0) + glm::vec3(-.5), color),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            // TODO(student): Complete indices data for the cube mesh
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        meshes[name] = new Mesh(name);
        meshes[name]->SetDrawMode(GL_LINES_ADJACENCY);
        meshes[name]->InitFromData(vertices, indices);

        // Actually create the mesh from the data
    }
}

void m1::Lab4::CreateCar()
{
    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
            {{-1.4939,-0.305302,-0.470492},{0.3137,0.3137,0.3137}},
{{-0.795867,0.325735,-0.470492},{0.3333,0.3333,0.3333}},
{{1.95666,-0.297439,-1.33139},{0.2902,0.2902,0.2902}},
{{1.62623,0.161646,-1.25233},{0.3098,0.3098,0.3098}},
{{-1.4939,-0.305302,0.470492},{0.3882,0.3882,0.3882}},
{{-0.795867,0.325735,0.470492},{0.3176,0.3176,0.3176}},
{{1.95666,-0.297439,1.3395},{0.1373,0.1373,0.1373}},
{{1.62623,0.161646,1.24526},{0.3333,0.3333,0.3333}},
{{1.95666,-0.297439,1.10844},{0.1333,0.1333,0.1333}},
{{0.556449,-0.150698,0.292024},{0.3568,0.3568,0.3568}},
{{0.556449,-0.150698,-0.279215},{0.3804,0.3804,0.3804}},
{{1.95666,-0.297439,-1.09713},{0.2392,0.2392,0.2392}},
{{1.62623,0.161646,-1.03643},{0.1882,0.1882,0.1882}},
{{0.625759,0.225879,-0.151614},{0.3137,0.3137,0.3137}},
{{0.625759,0.225879,0.174199},{0.298,0.298,0.298}},
{{1.62623,0.161646,1.02408},{0.1373,0.1373,0.1373}},
{{-1.4939,-0.305302,-0.282295},{0.3804,0.3804,0.3804}},
{{-1.55314,-0.286509,-0.091747},{0.4039,0.4039,0.4039}},
{{-1.55314,-0.286509,0.09645},{0.4078,0.4078,0.4078}},
{{-1.4939,-0.305302,0.282295},{0.4078,0.4078,0.4078}},
{{-0.795867,0.325735,0.282295},{0.3255,0.3255,0.3255}},
{{-0.855115,0.344528,0.09645},{0.3294,0.3294,0.3294}},
{{-0.855115,0.344528,-0.091747},{0.3333,0.3333,0.3333}},
{{-0.795867,0.325735,-0.282295},{0.3333,0.3333,0.3333}},
{{-0.252516,-0.32545,-1.00192},{0.2039,0.2039,0.2039}},
{{-0.03133,0.107555,-0.68882},{0.3216,0.3216,0.3216}},
{{-0.252516,-0.32545,1.00192},{0.153,0.153,0.153}},
{{-0.03133,0.107555,0.68882},{0.2902,0.2902,0.2902}},
{{-0.03133,0.107555,0.470492},{0.2941,0.2941,0.2941}},
{{-0.493116,0.433466,0.143365},{0.3255,0.3255,0.3255}},
{{-0.493116,0.433466,-0.156545},{0.3333,0.3333,0.3333}},
{{-0.03133,0.107555,-0.470492},{0.3255,0.3255,0.3255}},
{{-0.252516,-0.32545,-0.654886},{0.3372,0.3372,0.3372}},
{{-0.731919,-0.247846,-0.22291},{0.3059,0.3059,0.3059}},
{{-0.731919,-0.247846,0.191292},{0.3216,0.3216,0.3216}},
{{-0.252516,-0.32545,0.58772},{0.2863,0.2863,0.2863}},
{{0.571508,0.282568,-0.13501},{0.3176,0.3176,0.3176}},
{{0.571508,0.282568,0.155644},{0.3294,0.3294,0.3294}},
{{-0.426629,0.467755,0.128138},{0.3255,0.3255,0.3255}},
{{-0.426629,0.467755,-0.139409},{0.3333,0.3333,0.3333}},
{{-0.014674,0.177013,-0.419478},{0.3294,0.3294,0.3294}},
{{-0.014674,0.177013,0.419964},{0.298,0.298,0.298}},
{{0.553562,0.273673,-0.130833},{0.3451,0.4039,0.5216}},
{{0.553562,0.273673,0.150907},{0.3451,0.3725,0.451}},
{{-0.413961,0.45318,0.124245},{0.2275,0.5176,0.6432}},
{{-0.413961,0.45318,-0.135097},{0.2706,0.4589,0.5412}},
{{-0.014642,0.171355,-0.406576},{0.3451,0.3725,0.451}},
{{-0.014642,0.171355,0.407121},{0.3255,0.3529,0.4314}},
        };

        vector<unsigned int> indices =
        {
           24,3,2,
8,7,6,
26,5,4,
23,0,16,
35,4,19,
31,1,23,
28,5,27,
29,20,28,
29,22,21,
30,23,22,
32,0,24,
33,16,32,
34,17,33,
34,19,18,
5,19,4,
19,21,18,
21,17,18,
22,16,17,
3,11,2,
12,10,11,
13,9,10,
9,15,8,
9,35,34,
10,34,33,
10,32,11,
11,24,2,
29,41,38,
15,27,7,
12,25,31,
8,26,35,
7,26,6,
1,24,0,
13,37,14,
13,40,36,
13,12,31,
15,14,28,
40,45,46,
41,44,38,
41,43,47,
30,40,31,
14,41,28,
29,39,30,
42,44,43,
44,47,43,
45,42,46,
40,42,36,
38,45,39,
36,43,37,
24,25,3,
8,15,7,
26,27,5,
23,1,0,
35,26,4,
31,25,1,
28,20,5,
29,21,20,
29,30,22,
30,31,23,
32,16,0,
33,17,16,
34,18,17,
34,35,19,
5,20,19,
19,20,21,
21,22,17,
22,23,16,
3,12,11,
12,13,10,
13,14,9,
9,14,15,
9,8,35,
10,9,34,
10,33,32,
11,32,24,
29,28,41,
15,28,27,
12,3,25,
8,6,26,
7,27,26,
1,25,24,
13,36,37,
13,31,40,
40,39,45,
41,47,44,
41,37,43,
30,39,40,
14,37,41,
29,38,39,
42,45,44,
40,46,42,
38,44,45,
36,42,43,
        };

        meshes["car"] = new Mesh("car");
        meshes["car"]->InitFromData(vertices, indices);

        for (auto& v : vertices)
            swap(v.color.x, v.color.z);

        meshes["carE"] = new Mesh("carE");
        meshes["carE"]->InitFromData(vertices, indices);


        for (auto& v : vertices)
            v.color = {0.05,0.05,0.05};

        meshes["carS"] = new Mesh("carS");
        meshes["carS"]->InitFromData(vertices, indices);
        // Actually create the mesh from the data
    }
}

void m1::Lab4::CreateEngine()
{
    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
           {{-0.238005,0.424765,-0.150331},{0.3765,0.3686,0.2471}},
{{-0.238005,-0.424765,-0.150331},{0.2745,0.2745,0.2745}},
{{-0.238005,0.424765,0.150331},{0.3882,0.3843,0.2588}},
{{-0.238005,-0.424765,0.150331},{0.3804,0.3804,0.3725}},
{{0.238005,0.424765,-0.150331},{0.3882,0.3843,0.2588}},
{{0.238005,-0.424765,-0.150331},{0.2863,0.2863,0.2863}},
{{0.238005,0.424765,0.150331},{0.3647,0.3608,0.2353}},
{{0.238005,-0.424765,0.150331},{0.4941,0.4941,0.4823}},
{{-0.196324,-0.46918,-0.124005},{0.2392,0.2392,0.2392}},
{{-0.196324,-0.46918,0.124005},{0.3686,0.3686,0.3647}},
{{0.196324,-0.46918,0.124005},{0.4784,0.4784,0.4667}},
{{0.196324,-0.46918,-0.124005},{0.2823,0.2823,0.2823}},
{{-0.196952,0.47107,0.124401},{0.4706,0.4589,0.2432}},
{{-0.196952,0.47107,-0.124401},{0.4589,0.4471,0.2353}},
{{0.196952,0.47107,0.124401},{0.4628,0.451,0.2353}},
{{0.196952,0.47107,-0.124401},{0.4667,0.455,0.2392}},
{{-0.160823,-0.371261,-0.101581},{0.5059,0.4902,0.2039}},
{{-0.160823,-0.371261,0.101581},{0.6432,0.6276,0.3333}},
{{0.160823,-0.371261,0.101581},{0.6902,0.6745,0.3765}},
{{0.160823,-0.371261,-0.101581},{0.5216,0.5059,0.2196}},
{{-0.238005,0.500784,0.103367},{0.8588,0.8196,0.1333}},
{{-0.238005,0.500784,-0.103367},{0.7098,0.6824,0.1647}},
{{0.238005,0.500784,-0.103367},{0.8549,0.8196,0.1333}},
{{0.238005,0.500784,0.103367},{0.7059,0.6785,0.1608}},
{{-0.196952,0.547088,0.085537},{0.6079,0.5843,0.1569}},
{{-0.196952,0.547088,-0.085537},{0.8196,0.7843,0.1373}},
{{0.196952,0.547088,-0.085537},{0.6471,0.6236,0.1922}},
{{0.196952,0.547088,0.085537},{0.8196,0.7843,0.1412}},
{{-0.176314,0.43031,0.111366},{0.4157,0.4039,0.1882}},
{{-0.176314,0.43031,-0.111366},{0.5686,0.549,0.2039}},
{{0.176314,0.43031,0.111366},{0.5725,0.553,0.2118}},

{{0.176314,0.43031,-0.111366},{0.4706,0.4589,0.2471}},
        };

        for (auto& v : vertices) {
            auto& c = v.color;
            if ((c.r + c.g) / 2 > c.b) {
                float dif = (c.r + c.g) / 2.f - c.b;
                c.r -= dif;
                c.b += dif;
                c.r += .05f;
                c.b += .05f;
                c.g += .05f;
               
            }

        }

        vector<unsigned int> indices =
        {
          1,2,0,
3,6,2,
7,4,6,
5,0,4,
0,15,4,
3,10,7,
10,19,11,
7,11,5,
1,9,3,
5,8,1,
15,30,14,
6,12,2,
13,21,25,
14,23,27,
17,19,18,
8,17,9,
8,19,16,
10,17,18,
20,25,21,
22,27,23,
2,21,0,
14,26,15,
13,24,12,
4,23,6,
12,20,2,
15,22,4,
30,29,28,
12,29,13,
13,31,15,
14,28,12,
1,3,2,
3,7,6,
7,5,4,
5,1,0,
0,13,15,
3,9,10,
10,18,19,
7,10,11,
1,8,9,
5,11,8,
15,31,30,
6,14,12,
13,0,21,
14,6,23,
17,16,19,
8,16,17,
8,11,19,
10,9,17,
20,24,25,
22,26,27,
2,20,21,
14,27,26,
13,25,24,
4,22,23,
12,24,20,
15,26,22,
30,31,29,
12,28,29,
13,29,31,
14,30,28,


        };

        meshes["engine"] = new Mesh("engine");
        meshes["engine"]->InitFromData(vertices, indices);


        for (auto& v : vertices)
            swap(v.color.x, v.color.z);


        meshes["engineE"] = new Mesh("engineE");
        meshes["engineE"]->InitFromData(vertices, indices);
        // Actually create the mesh from the data
    }
}

void m1::Lab4::CreateRock()
{
    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
           {{-0.839095,0.828003,0.468759},{0.749,0.4,0.2}},
{{-0.564719,0.405277,-0.882475},{0.749,0.4,0.2}},
{{0.913787,0.920671,-0.29845},{0.8078,0.4353,0.2157}},
{{-0.384438,1.15811,0.231631},{0.7764,0.4157,0.2078}},
{{1.17419,-0.325498,-0.616616},{0.5059,0.2706,0.1333}},
{{-0.809592,-0.633597,0.298507},{0.5176,0.2784,0.1373}},
{{1.0019,-0.415748,0.510267},{0.5451,0.2902,0.1451}},
{{-0.065337,-0.542822,0.938244},{0.5059,0.2706,0.1333}},
{{-0.535555,0.626968,1.00844},{0.749,0.4,0.2}},
{{-1.00042,0.246214,0.222268},{0.749,0.4,0.2}},
{{0.604827,-0.071261,-1.08892},{0.6745,0.3608,0.1765}},
{{0.436478,0.692092,0.829795},{0.8117,0.4353,0.2157}},
{{0.605194,0.704988,-0.845698},{0.749,0.4,0.2}},

        };

        for (auto& v : vertices) {
            
            v.color *= .5f;
            v.color = v.color * .5f + glm::vec3(glm::length(v.color)) * .5f;
            
        }

        vector<unsigned int> indices =
        {
   7,5,10,
7,10,4,
10,5,1,
0,8,3,
1,0,3,
2,12,3,
12,1,3,
8,11,3,
3,11,2,
4,12,2,
4,10,12,
10,1,12,
9,0,1,
7,11,8,
6,2,11,
1,5,9,
9,8,0,
9,5,8,
5,7,8,
7,6,11,
6,4,2,

4,6,7,

        };

        meshes["rock"] = new Mesh("rock");
        meshes["rock"]->InitFromData(vertices, indices);

        // Actually create the mesh from the data
    }
}

void m1::Lab4::DebugLine(glm::vec3 orig, glm::vec3 tip, glm::vec3 color)
{
    
    vector<VertexFormat> ver = { {orig, color}, {tip, color} };
    vector<unsigned int > indx = { 0,1 };

    Mesh* ok = new Mesh("generatedLine");
    ok->SetDrawMode(GL_LINES);
    ok->InitFromData(ver, indx);
    RenderMesh2D(ok, shaders["VertexColor"], glm::mat3(1));
    delete ok;
}

void m1::Lab4::Car::Engine::applyForce(float t, float deltaTime, Car& car)
{
    car.vel += deltaTime * actualOrient * t / 10.f;
    car.angularRot += deltaTime * t * glm::cross(relPos, orient);




    

        t = .25f;
        if (queuedAccel > 0)
        t += queuedAccel / 100.f;
       
        if (queuedAccel - prevAccel > 0)
        t += (queuedAccel - prevAccel) /5.f;

        if (car.env->window->KeyHold(GLFW_KEY_W)) {
            float fwdM = glm::dot({ 1,0,0 }, orient);
            if (fwdM > 0)
                t += fwdM;
        }
        
        if (car.env->window->KeyHold(GLFW_KEY_D)) {
            float fwdM = glm::dot({ 0,0,1 }, orient);
            if (fwdM > 0)
                t += fwdM;
        }

        if (car.env->window->KeyHold(GLFW_KEY_A)) {
            float fwdM = glm::dot({ 0,0,-1 }, orient);
            if (fwdM > 0)
                t += fwdM;
        }

        t = glm::clamp(t, 0.f, 3.f);

    if (t > 0) {
        if (str > 5 && (car.env->enablePID || !car.playerCar)) {

            if (car.playerCar) {
                car.env->glLineWidth(10);


                car.env->DebugLine(car.pos + actualRelPos, car.pos + randV() * t / 10.f + actualRelPos - (actualOrient)*t * randF(), { .5,.95,1 });
                car.env->DebugLine(car.pos + actualRelPos, car.pos + randV() * t / 10.f + actualRelPos - (actualOrient)*t * randF(), { .5,.95,1 });
                car.env->DebugLine(car.pos + actualRelPos, car.pos + randV() * t / 10.f + actualRelPos - (actualOrient)*t * randF(), { .5,.95,1 });
                car.env->DebugLine(car.pos + actualRelPos, car.pos + randV() * t / 10.f + actualRelPos - (actualOrient)*t * randF(), { .5,.95,1 });
            }
            // engine particles
            {
                float distToPlayer = glm::length(car.env->car.pos - car.pos);
                glPointSize(max(1.f,5 - sqrt(distToPlayer /10.f)));

                timeSinceStart += deltaTime;

                if ((t > 2.9 ||(t > 2 &&!car.playerCar)) && (timeSinceEnd > 1 || timeSinceEnd == 0)) {
                    timeSinceEnd = 0;
                }
                else {
                    timeSinceEnd += deltaTime;
                    if (timeSinceEnd > 1)
                        timeSinceStart = 0;

                    // timeSinceStart = 0;

                }

                if (timeSinceStart > 0) {
                    glUseProgram(car.env->shaders["DustShader"]->program);
                    glUniform1f(car.env->timeLoc, timeSinceStart);
                    glUniform1f(car.env->extraTimeLoc, timeSinceEnd);
                    glUniform3fv(car.env->carVelLoc2, 1, glm::value_ptr(glm::vec3{ car.vel.x, 0, car.vel.z }));
                    glm::mat4 dustMat = glm::translate(glm::mat4(1), car.pos + actualRelPos);
                    dustMat *= glm::toMat4(glm::rotation({ 0,1,0 }, actualOrient));

                    if (car.playerCar)
                    car.env->RenderMesh(car.env->meshes["dust"], car.env->shaders["DustShader"], dustMat);
                    else
                    car.env->RenderMesh(car.env->meshes["dustE"], car.env->shaders["DustShader"], dustMat);
                }
            }
        }

        prevAccel += (queuedAccel - prevAccel) * deltaTime ;
       
    }
    queuedAccel = 0;
}
