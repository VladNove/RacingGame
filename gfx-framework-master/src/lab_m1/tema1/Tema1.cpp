#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/objects2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    initRes = resolution;


    AddMeshToList(objects2D::CreateWing1("wing1", 1.05, 20,-2));
    AddMeshToList(objects2D::CreateWing2("wing2", 1.05, 20,-1));
    AddMeshToList(objects2D::CreateWing1("wing1B", .9, 20,-3));
    AddMeshToList(objects2D::CreateWing2("wing2B", .9, 20,-4));
    AddMeshToList(objects2D::CreateBody("body", 10, -2.5));
    AddMeshToList(objects2D::CreateHead("head", 7, -2.1));
    AddMeshToList(objects2D::CreateCircle("crossC", 15, 0, 8));
    AddMeshToList(objects2D::CreateCircle("crossH", 5, 0, 4));
    AddMeshToList(objects2D::CreateCircle("rifleSight", 5, 0, 4, {.55,.55,.55},true));
    AddMeshToList(objects2D::CreateSquare("rifleTop", { -10,0,0 }, 20, { .6,.6,.6 },true));
    AddMeshToList(objects2D::CreateSquare("rifleSide", { 0,0,0 }, 20, { .6,.3,.3 }, true));
    AddMeshToList(objects2D::CreateSquare("colBox", { 0,0,0 }, 50, { 0,1,0 }, false));

    AddMeshToList(objects2D::CreateGrass("grass",150,-.5,5,window->GetResolution().x/5));
    AddMeshToList(objects2D::CreateGrass("grass2", 170, -100, 1.25, window->GetResolution().x / 1.25));
    AddMeshToList(objects2D::CreateSquare("grassBG", { 0,0,-101}, 1, { 0,.7,0 }, true));
    AddMeshToList(objects2D::CreateBullet("bullet", 0));
    AddMeshToList(objects2D::CreateHeart("heart", 0));
    AddMeshToList(objects2D::CreateMuzzleFlash("mflash", -.1));
    AddMeshToList(objects2D::CreateFeather("feather", -3.5));

    AddMeshToList(objects2D::CreateSquare("scoreBox", { 0,0,0 }, 50, { 1,1,1 }, false));
    AddMeshToList(objects2D::CreateSquare("score", { 0,0,0 }, 50, { 1,1,1 }, true));



    AddMeshToList(objects2D::CreateBody("Kbody", 10, -2.5));
    AddMeshToList(objects2D::CreateHead("Khead", 7, -2.1));
    AddMeshToList(objects2D::CreateNeck("Kneck", 7, -2.2));
    AddMeshToList(objects2D::CreateWing1("Kwing1", 1.05, 20, -2));
    AddMeshToList(objects2D::CreateWing2("Kwing2", 1.05, 20, -1));
    target.pos = { 0,0 };
    target.vel = { 0,0 };

    tr = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);

    std::cout << "C:\\Users\\vladn\\Desktop\\EGC\\gfx-framework-master\\assets\\fonts\\Hack-Bold.ttf" << endl;
   // std::cout << window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf";
    tr->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);

    ducks.push_back(Duck{ this });



    bossFight = false;

   // bossFight = true;


    kraken = Kraken(this);
}





void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    {
        if (slowMoTimeLeft <= 0)
            glClearColor(.8, .8, .85, 1);

        else if (slowMoTimeLeft < 1)
            glClearColor(.8 + slowMoTimeLeft * .1, .8 + slowMoTimeLeft * .1, .85 + slowMoTimeLeft * .05, 1);

        else
            glClearColor(.9, .9, .9, 1);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glm::ivec2 resolution = window->GetResolution(true);
    GetSceneCamera()->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

/*
float Time = 0;
bool expand = true;


bool rightM = true;
*/

float randF() {
    return ((float)rand() / RAND_MAX);
}


void Tema1::RenderWings(Duck& duck, glm::mat3 modelMatrix, float deltaTimeSeconds){
    modelMatrix *= transform2D::Translate(35, 10);

        duck.Fwing2Offs += randF() * deltaTimeSeconds;
        duck.Bwing2Offs += randF() * deltaTimeSeconds;
        float baseHeight = sin(duck.flapSpeed * Time + duck.Fwing2Offs) * .75 + .25;
        float tipHeight = sin(duck.flapSpeed * Time - AI_MATH_HALF_PI + duck.Fwing2Offs) * .25 - .3;
        float baseHeightB = sin(duck.flapSpeed * Time + duck.Bwing2Offs) * .75 + .25;
        float tipHeightB = sin(duck.flapSpeed * Time - AI_MATH_HALF_PI + duck.Bwing2Offs) * .25 - .3;


        auto wingBase = modelMatrix * transform2D::Scale(1, baseHeight);
        RenderMesh2D(meshes["wing1" + duck.extraMeshId], shaders["VertexColor"], wingBase);
        duck.wingBox = wingBase * duck.wingBox;

        auto wingTip = modelMatrix * transform2D::Translate(0, baseHeight * 20 * 2) * transform2D::Scale(1, tipHeight);
        RenderMesh2D(meshes["wing2" + duck.extraMeshId], shaders["VertexColor"], wingTip);

        auto wingBaseB = modelMatrix * transform2D::Scale(1, baseHeightB);
        RenderMesh2D(meshes["wing1B"], shaders["VertexColor"], wingBaseB);

        auto wingTipB = modelMatrix * transform2D::Translate(0, baseHeightB * 20 * 2) * transform2D::Scale(1, tipHeightB);
        RenderMesh2D(meshes["wing2B"], shaders["VertexColor"], wingTipB);
    

}

void Tema1::RenderDuck(glm::mat3 modelMatrix, float deltaTimeSeconds) {
    
    for (Duck& duck : ducks) {


        modelMatrix = transform2D::Translate(duck.pos.x, duck.pos.y) * transform2D::Scale(1,1)
            * transform2D::Scale(duck.flipped, 1);



        modelMatrix *=
            transform2D::Translate(0, sin(duck.flapSpeed * Time + AI_MATH_PI + (duck.Fwing2Offs + duck.Bwing2Offs) / 2) * 10);
        modelMatrix *= transform2D::Rotate(duck.bodyAngle);
        duck.bodyBox = modelMatrix * duck.bodyBox;

        RenderMesh2D(meshes["body" + duck.extraMeshId], shaders["VertexColor"], modelMatrix);
        RenderWings(duck, modelMatrix, deltaTimeSeconds);

        modelMatrix *= transform2D::Translate(123, -2);
        modelMatrix *= transform2D::Rotate(duck.headAngle - duck.bodyAngle);
        duck.headBox = modelMatrix * duck.headBox;
        RenderMesh2D(meshes["head" + duck.extraMeshId], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::UpdateTarget(float deltaTimeSeconds) {
    if (Time < .2)
    {
        target.pos = mousePos;
        return;
    }
    glm::vec2 err = mousePos - target.pos;
    glm::vec2 accel = err * target.P + target.vel * target.D;

    
    float accelL = glm::length(accel);
    
    if (accelL > target.maxAcc)
        accel *= target.maxAcc / accelL;

    target.vel += accel * deltaTimeSeconds;
    float velL = glm::length(target.vel);
    if (velL > target.maxVel)
        target.vel *= target.maxVel / velL;
    target.pos += target.vel * deltaTimeSeconds;


    if (target.pos.x < -50)
        target.pos.x = -50;
    if (target.pos.x > window->GetResolution().x + 50)
        target.pos.x = window->GetResolution().x + 50;
    if (target.pos.y < -50)
        target.pos.y = -50;
    if (target.pos.y > window->GetResolution().y + 50)
        target.pos.y = window->GetResolution().y + 50;


}


void Tema1::RenderWeapon(float xOffs, float gunLenghtScale, float sideLenght, float topLenght, float sideVOffset, float sideHOffset) {

    float gunDist = glm::length(glm::vec2(target.pos.x - xOffs, target.pos.y));
    float gunLength = gunLenghtScale / (gunDist + gunLenghtScale);

    target.gunTip = glm::vec2{ xOffs,0 } + glm::vec2{ (target.pos.x - xOffs),target.pos.y } *gunLength;

    modelMatrix = transform2D::Translate(xOffs, 0);
    modelMatrix *= transform2D::Skew(1, 0, ((target.pos.x - xOffs) / topLenght) * gunLength, (target.pos.y) / topLenght * gunLength);
    RenderMesh2D(meshes["rifleTop"], shaders["VertexColor"], modelMatrix);

    modelMatrix = transform2D::Translate(xOffs + sideHOffset, -sideVOffset);
    modelMatrix *= transform2D::Skew(((target.pos.x - xOffs) / sideLenght) * gunLength, (target.pos.y) / sideLenght * gunLength, 0, 1);
    RenderMesh2D(meshes["rifleSide"], shaders["VertexColor"], modelMatrix);
    modelMatrix = transform2D::Translate(xOffs + topLenght + sideHOffset, -sideVOffset);
    modelMatrix *= transform2D::Skew(((target.pos.x - xOffs) / sideLenght) * gunLength, (target.pos.y) / sideLenght * gunLength, 0, 1);
    RenderMesh2D(meshes["rifleSide"], shaders["VertexColor"], modelMatrix);

   

}

void Tema1::Duck::UpdateHead(float deltaTime) {


    if (timeOfLastUpdate + 1 < env->Time && !dead) {
        timeOfLastUpdate = env->Time;

        headDesiredDir = AI_DEG_TO_RAD(60 * (randF() * 2 - 1));
        
    }
    
    float headAngleDif = headDesiredDir - headAngle;
    float accel = headAngleDif * 3 + headSpeed * ((dead)?-1:-2);

    headSpeed += accel * deltaTime;
    if (abs(headSpeed) > 5)
        headSpeed = headSpeed / abs(headSpeed);

    headAngle += headSpeed * deltaTime;

}

void Tema1::Duck::UpdateBody(float deltaTime) {


    timeTillEvade -= deltaTime;
    if ((timeTillEvade < 0 || env->bullets == 0) && !env->lost)
        evading = true;


    if (dead && flapSpeed > 0) {
        Fwing2Offs += 8 * deltaTime * env->Time;
        Bwing2Offs += 8 * deltaTime * env->Time;
        flapSpeed -= 8 * deltaTime;
        
    }

    //cout << flapSpeed << endl;
    //cout << headDesiredDir << endl;
    bodyAngle += (headAngle - bodyAngle) * 1 * deltaTime;

    glm::vec2 vel = glm::vec2{  cos(bodyAngle) * flipped, - sin(bodyAngle)  } * 200.f;
    velo = vel;
    pos += vel * deltaTime;

   // cout << pos << endl;

    if (pos.x > env->window->GetResolution().x && flipped == 1) {
        pos.x = env->window->GetResolution().x + 50;
        flipped = -1;
    }
    if (pos.x < 0 && flipped == -1) {
        pos.x = -50;
        flipped = 1;
    }
   

    if (dead)
        headDesiredDir = AI_MATH_HALF_PI;
    else {

        if (pos.y > env->window->GetResolution().y - 200)
            headDesiredDir = abs(headDesiredDir);
        if (pos.y < 200)
            headDesiredDir = -abs(headDesiredDir);
        if (evading)
            headDesiredDir = -AI_MATH_HALF_PI;
    }

}


static float x=0, y=0, sx = 1, sy =1;

void Tema1::CreateDuck(bool slowMoAble) {

    Duck duck = Duck{ this };
    

    duck.pos = { rand() % window->GetResolution().x,0 }; //;
    duck.flipped = (rand() % 2) * 2 - 1;
    duck.headDesiredDir = AI_DEG_TO_RAD(-90 * randF());
    duck.timeOfLastUpdate = Time + randF();
    duck.extraMeshId = "_" + std::to_string(extraMeshId++);
    duck.timeTillEvade = 10 + randF() * 10 * DuckTimeScale;


    
    if (slowMoAble && rand() % 4 == 0) {
        duck.specialPower = 3;
    } else if (rand() % 5 == 0) {
        duck.specialPower = rand() % 2 + 1;
    }


    AddMeshToList(objects2D::CreateWing1("wing1" + duck.extraMeshId, 1.05, 20, -2, duck.specialPower));
    AddMeshToList(objects2D::CreateWing2("wing2" + duck.extraMeshId, 1.05, 20, -1, false, duck.specialPower));
    AddMeshToList(objects2D::CreateBody("body" + duck.extraMeshId, 10, -2.5, false, duck.specialPower));
    AddMeshToList(objects2D::CreateHead("head" + duck.extraMeshId, 7, -2.1, false));
    ducks.push_back(duck);
}

void Tema1::RenderKraken(float deltaTimeSeconds) {
    
}

void Tema1::incScore(int x) {
    score += x;
}

void Tema1::Update(float deltaTimeSeconds)
{
    using namespace transform2D;

    
    float screenX = window->GetResolution().x;
    float screenY = window->GetResolution().y;

    //RenderKraken(deltaTimeSeconds);
  //  if (!bossFight && Time > 1) {
   //     DuckTimeScale = 1.5;
  //      bossFight = true;
  //  }

    if (bossFight) {
        kraken.Update(deltaTimeSeconds * DuckTimeScale *.5);
    }
    //level update
    if (ducks.empty() && ducksWaiting == 0) {
        int newLevel = (score / 100) + 1;
        if (level != newLevel) {
            level = newLevel;
            if (level <= 5)
                DuckTimeScale += .5;
            else if (level <= 10)
                DuckTimeScale += .25;
            else
                DuckTimeScale += .1;

            if (level == 3 || level == 10) {
                bossFight = true;
                kraken = Kraken(this);

                kraken.timeSinceAlive = -10;
                bullets = maxBullets;
            }

        }

        
    }

    if (!bossFight) {
        if (ducks.empty() && ducksWaiting == 0) {
            timeToNextDuck -= deltaTimeSeconds;
        }

        timeToNextWaitingDuck -= deltaTimeSeconds;

        if (timeToNextDuck < 1 && ducks.empty() && ducksWaiting == 0 && !lost)
            bullets = maxBullets;

        //new ducks
        if (timeToNextDuck < 0 && ducks.empty() && ducksWaiting == 0) {
            while (rand() % 2 && ducksWaiting < (bullets - 1) / 2)
                ducksWaiting++;

            CreateDuck(ducksWaiting > 0);



            timeToNextWaitingDuck = randF();
            timeToNextDuck = 1 + randF() * 2; //wait time to next level start
        }

        if (ducksWaiting > 0 && timeToNextWaitingDuck < 0) {
            ducksWaiting--;
            CreateDuck(true);
            timeToNextWaitingDuck = randF() * 5;
        }

        if (lost && ducksWaiting + ducks.size() < 10) {
            ducksWaiting += 10;
        }
    }

    //slowMo
    if (slowMoTimeLeft > 0) {
        slowMoTimeLeft -= deltaTimeSeconds;
        TimeScale = .25;
        if (slowMoTimeLeft < 1)
            TimeScale = 1 - slowMoTimeLeft * .75;
    }
    else {
        TimeScale = 1;
    }


    //if (!bossFight)
    deltaTimeSeconds *= DuckTimeScale * TimeScale;
    Time += deltaTimeSeconds;
    

    //DUCK
    for (Duck& duck : ducks)
    {
        duck.bodyBox = Translate(13.6, -27.7001) * Scale(2.04, 0.87);
        duck.headBox = Translate(-8.7, -13.5) * Scale(1.36, 0.5);
        duck.wingBox = Translate(5, 0.9) * Scale(1.45, 0.82);
        duck.UpdateHead(deltaTimeSeconds );
        duck.UpdateBody(deltaTimeSeconds );
    }

    RenderDuck({}, deltaTimeSeconds);


    //FEATHERS
    {
        auto it = feathers.begin();
        for (; it != feathers.end();) {

         //   cout << it->timeLeft << endl;
            if (it->timeLeft < 0) {
                it = feathers.erase(it);
            }
            else {
                it->UpdateFeather(deltaTimeSeconds, this);
                it++;
            }
        }
    }

    //if (!bossFight)
    deltaTimeSeconds /= DuckTimeScale;


    // WEAPON 
    {
        gunModelx += (mousePos.x - gunModelx + 300) * 1 * deltaTimeSeconds;
        if (gunModelx < 0)
            gunModelx = 0;
        if (gunModelx > screenX + 100)
            gunModelx = screenX + 100;
        RenderWeapon(gunModelx, 200, 20, 20, 20, -10);
        UpdateTarget(deltaTimeSeconds);
        glLineWidth(1);
        RenderMesh2D(meshes["crossC"], shaders["VertexColor"], Translate(mousePos.x, mousePos.y));

        glLineWidth(3);
        RenderMesh2D(meshes["crossH"], shaders["VertexColor"], Translate(target.pos.x, target.pos.y));
    }




    // RENDER GRASS
    {
        glLineWidth(3);
       
        static float gOfs[6] = { 0 };
        for (int i = 0; i < 6; i++)
            gOfs[i] += randF() * deltaTimeSeconds * DuckTimeScale;


        RenderMesh2D(meshes["grass"], shaders["VertexColor"],
            Translate(-screenX /2, 0) * Skew(1, 0, sin(Time+gOfs[0]) * .1, 1));
        RenderMesh2D(meshes["grass"], shaders["VertexColor"], 
            Skew(1, 0, sin(Time+gOfs[1]) * .1, 1));
        RenderMesh2D(meshes["grass"], shaders["VertexColor"], 
            Translate(screenX /2, 0) * Skew(1, 0, sin(Time+gOfs[2]) * .1, 1));
        RenderMesh2D(meshes["grass2"], shaders["VertexColor"], 
            Translate(-screenX / 2+2.5, 0) * Skew(1, 0, sin(Time+gOfs[3]) * .1, 1));
        RenderMesh2D(meshes["grass2"], shaders["VertexColor"], 
            Translate(2.5, 0) *  Skew(1, 0, sin(Time+gOfs[4] * sqrt(2)) * .1, 1));
        RenderMesh2D(meshes["grass2"], shaders["VertexColor"], 
            Translate(screenX / 2 +2.5, 0) * Skew(1, 0, sin(Time+gOfs[5]) * .1, 1));
        RenderMesh2D(meshes["grassBG"], shaders["VertexColor"],
             Scale(screenX,50));
    }
    glLineWidth(1);

    //DUCK escaped
    {
        auto it = ducks.begin();
        for (; it != ducks.end();) {
            Duck& duck = *it;
            if (duck.evading && !duck.dead && duck.pos.y > window->GetResolution().y + 10)
            {
                if (!lost)
                    lives--;

                if (lives == 0) {
                    
                    lost = true;
                    bullets = 0;
                }

                meshes.erase("wing1" + duck.extraMeshId);
                meshes.erase("head" + duck.extraMeshId);
                meshes.erase("wing2" + duck.extraMeshId);
                meshes.erase("body" + duck.extraMeshId);
                it = ducks.erase(it);

            }

            //DUCK DIED
            else if (duck.pos.y < -10 && duck.dead)
            {
                incScore(20);

                meshes.erase("head" + duck.extraMeshId);
                meshes.erase("wing2" + duck.extraMeshId);
                meshes.erase("body" + duck.extraMeshId);
                it = ducks.erase(it);


            }

            else
                it++;
        }
    }

    //BOSSFIGHT
    if (bossFight) {
        if (kraken.pos.x > screenX && kraken.health > 0) {
            lives -= min(((int)(kraken.health * 3 / 100.f) + 1),3);
            if (lives <= 0) {
                lives = 0;
                lost = true;
                bullets = 0;
            }
            bossFight = false;
        }
        if (kraken.health <= 0 && kraken.pos.y < -500) {
            bossFight = false;
        }
    }


    //HUD AND EXTRA
    {
        glLineWidth(10);
        RenderMesh2D(meshes["rifleSight"], shaders["VertexColor"], Translate(target.gunTip.x, target.gunTip.y));
        

        for (int i = 0; i < maxHearts; i++) {

            if (i >= lives)
                heartStates[i] += 200 * deltaTimeSeconds * (heartStates[i] < 100);
            else
                heartStates[i] -= 100 * deltaTimeSeconds * (heartStates[i] > 0);

            RenderMesh2D(meshes["heart"], shaders["VertexColor"], Translate(75 * i + 50, screenY - 50 + heartStates[i]) * Scale(.5, .5));
        }

        for (int i = 0; i < maxBullets; i++) {

            if (i >= bullets)
                bulletStates[i] += 2 * deltaTimeSeconds * (bulletStates[i] < AI_MATH_HALF_PI);
            else
                bulletStates[i] -= 10 * deltaTimeSeconds * (bulletStates[i] > 0);

            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], Translate(75 * i - 100, 0) * Rotate(bulletStates[i]) * Translate(150, 0) * Scale(.5, .5));

        }
           
        
        timeSinceLastShot += deltaTimeSeconds;
        if (timeSinceLastShot < .1)
        {
            glm::vec2 relGunpos = glm::vec2{ target.gunTip.x - gunModelx, target.gunTip.y };
            glm::vec2 gunDir = glm::normalize(relGunpos);
          //  cout << gunDir.x;
            RenderMesh2D(meshes["mflash"], shaders["VertexColor"], Translate(target.gunTip.x, target.gunTip.y-10) *
            Rotate(acos(-gunDir.x) - AI_MATH_HALF_PI) * Scale(1,glm::length(relGunpos / 150.f)));

            RenderMesh2D(meshes["btrail"], shaders["VertexColor"], glm::mat3(1));
        }

        if (bossFight)
        {
            glLineWidth(5);
            RenderMesh2D(meshes["scoreBox"], shaders["VertexColor"], Translate(screenX/2, screenY - 100)* Scale(2, 1));
            RenderMesh2D(meshes["scoreBox"], shaders["VertexColor"], Translate(screenX / 2 +100, screenY - 100)* Scale(2, 1));
            RenderMesh2D(meshes["scoreBox"], shaders["VertexColor"], Translate(screenX / 2 +200, screenY - 100)* Scale(2, 1));
            RenderMesh2D(meshes["score"], Translate(screenX / 2, screenY - 100)* Scale(6 * min(1.f, (float)(kraken.health) / 100), 1), { 1,0,0 });
        }

        if (!lost)
        {
            glLineWidth(5);
            RenderMesh2D(meshes["scoreBox"], shaders["VertexColor"], Translate(screenX - 250, screenY - 100) * Scale(4, 1));
            RenderMesh2D(meshes["score"], shaders["VertexColor"], Translate(screenX - 250, screenY - 100) * Scale(4 * min(1.f,(float)(score - (level-1) * 100) / 100), 1));
        }
    }



    //Collision
    for (Duck& duck : ducks)

    {
        duck.mouseColBox = glm::inverse(duck.bodyBox) * glm::vec3{ target.pos.x, target.pos.y, 1 };
        duck.wingColBox = glm::inverse(duck.wingBox) * glm::vec3{ target.pos.x, target.pos.y, 1 };
        duck.headColBox = glm::inverse(duck.headBox) * glm::vec3{ target.pos.x, target.pos.y, 1 };
        if (showCollisionBoxes) {
            RenderMesh2D(meshes["colBox"], shaders["VertexColor"], duck.wingBox);
            RenderMesh2D(meshes["colBox"], shaders["VertexColor"], duck.bodyBox);
            RenderMesh2D(meshes["colBox"], shaders["VertexColor"], duck.headBox);
            RenderMesh2D(meshes["colBox"], shaders["VertexColor"], Translate(400, 400) * glm::identity<glm::mat3>());
            RenderMesh2D(meshes["crossH"], shaders["VertexColor"], Translate(400, 400) * Translate(duck.mouseColBox.x, duck.mouseColBox.y));
            RenderMesh2D(meshes["crossH"], shaders["VertexColor"], Translate(400, 400) * Translate(duck.wingColBox.x, duck.wingColBox.y));
            RenderMesh2D(meshes["crossH"], shaders["VertexColor"], Translate(400, 400) * Translate(duck.headColBox.x, duck.headColBox.y));
        }
    }

    if (bossFight) {
        int colBox = 0;
        kraken.mouseCol.clear();

        for (auto& box : kraken.hitBoxes) {

            glm::vec3 mouseCol = glm::inverse(box) * glm::vec3{ target.pos, 1 };
            kraken.mouseCol.push_back(mouseCol);
            if (showCollisionBoxes) {
                // RenderMesh2D(meshes["colBox"], shaders["VertexColor"], box);

                RenderMesh2D(meshes["colBox"], box, { kraken.damage[colBox] / 35.f, 1.f - kraken.damage[colBox] / 35.f,0 });

                // RenderMesh2D(meshes["crossH"], shaders["VertexColor"], Translate(400, 400) * Translate(mouseCol.x, mouseCol.y));
            }

            colBox++;
        }

    }
    //cout << x << " " << y << " " << sx << " " << sy << endl;

    //textRendering
    float scaleX = initRes.x / screenX;
    float scaleY = initRes.y / screenY;
    screenX *= scaleX;
    screenY *= scaleY;
    
     
    //GAMEOVER
    if (lost) {
        DuckTimeScale = 1;
        tr->RenderText("GAME OVER", 274, 283, 1);
        tr->RenderText("level " + std::to_string(level), screenX / 2 - 100 * scaleX, screenY / 2 + 50 * scaleY, .4);
        tr->RenderText("score " + std::to_string(score), screenX / 2 - 100 * scaleX, screenY / 2 + 100 * scaleY, .4);
    } else {
        tr->RenderText("level " + std::to_string(level), screenX - 250 * scaleX, 120, .2);
        tr->RenderText("score " + std::to_string(score), screenX - 250 * scaleX, 150, .2);
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

    if (window->KeyHold(GLFW_KEY_W))
        sy += .01;
    if (window->KeyHold(GLFW_KEY_S))
        sy -= .01;
    if (window->KeyHold(GLFW_KEY_A))
        sx += .01;
    if (window->KeyHold(GLFW_KEY_D))
        sx -= .01;
    if (window->KeyHold(GLFW_KEY_UP))
        y += 1;
    if (window->KeyHold(GLFW_KEY_DOWN))
        y -= 1;
    if (window->KeyHold(GLFW_KEY_RIGHT))
        x += 1;
    if (window->KeyHold(GLFW_KEY_LEFT))
        x -= 1;
    /*    if (window->KeyHold(GLFW_KEY_S) && showCollisionBoxes)
            TimeScale = .25;
        else if (showCollisionBoxes)
            TimeScale = 1;*/

}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_B)
        showCollisionBoxes = !showCollisionBoxes;
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    mousePos = { mouseX, window->GetResolution().y - mouseY };
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    cout << mouseX;
    if (button == 1 && bullets > 0) {
        bullets--;
        timeSinceLastShot = 0;


        if (bullets == 0 && !ducks.empty())
            ducks[rand()%ducks.size()].evading = true;

        bool hitA = false;

        for (Duck& duck : ducks) {
            bool hit = false;


            if (duck.wingColBox.x > -5 && duck.wingColBox.x < 55 && duck.wingColBox.y > -5 && duck.wingColBox.y < 55) {
                AddMeshToList(objects2D::CreateWing2("wing2" + duck.extraMeshId, 1.05, 20, -1, true, duck.specialPower));
                hit = true;

            }
            if (duck.mouseColBox.x > -5 && duck.mouseColBox.x < 55 && duck.mouseColBox.y > -5 && duck.mouseColBox.y < 55) {
                AddMeshToList(objects2D::CreateBody("body" + duck.extraMeshId, 10, -2.5, true, duck.specialPower));
                hit = true;
            }
            if (duck.headColBox.x > -5 && duck.headColBox.x < 55 && duck.headColBox.y > -5 && duck.headColBox.y < 55) {

                incScore(10);


                AddMeshToList(objects2D::CreateHead("head" + duck.extraMeshId, 7, -2.1, true ));
                hit = true;
            }

            if (hit) {
                hitA = true;
                int Nfeathers = rand() % 5 + 2;
                for (int i = 0; i < Nfeathers; i++) {
                    glm::vec2 rVel{ (randF() - .5) * 50,(randF() - .5) * 50 };
                    feathers.push_back(Feather{ rVel + target.pos,rVel + (duck.velo*3.f + target.vel)/4.f,
                        randF() * AI_MATH_TWO_PI_F, (randF() - .5f) / 10,randF() * 4 });
                }
                duck.dead = true;
            }

            if (hit && duck.dead)
            {
                duck.headDesiredDir = AI_MATH_HALF_PI;
                duck.flapSpeed = 16;
                if (duck.specialPower == 1)
                    maxHearts++, heartStates.push_back(100), lives++, duck.specialPower = 0;
                if (duck.specialPower == 2) {
                    maxBullets++, bulletStates.push_back(0), bullets++, duck.specialPower = 0;
                    bulletStates[bullets - 1] = AI_MATH_HALF_PI/2;
                }
                if (duck.specialPower == 3)
                    slowMoTimeLeft = 10;
            }
        }

        if (bossFight) {
            int mCol = 0;
            for (auto& mouseCol : kraken.mouseCol) {
                

                if (mouseCol.x > -5 && mouseCol.x < 55 && mouseCol.y > -5 && mouseCol.y < 55) {
                    hitA = true;
                    incScore(kraken.damage[mCol]);
                    kraken.health -= kraken.damage[mCol];
                }

                

                if (hitA && kraken.health <= 0)
                {
                    kraken.health = 0;
                }
                mCol++;
            }

    
            if (hitA) {
          
                int Nfeathers = rand() % 10 + 5;
                for (int i = 0; i < Nfeathers; i++) {
                    glm::vec2 rVel{ (randF() - .5) * 50,(randF() - .5) * 50 };
                    feathers.push_back(Feather{ rVel + target.pos,rVel + (target.vel) / 4.f,
                        randF() * AI_MATH_TWO_PI_F, (randF() - .5f) / 10,randF() * 4 + 2 });
                }
            }
            
        }


        

        AddMeshToList(objects2D::CreateBulletTrail("btrail", target.gunTip + glm::vec2{ 0, -10 }, target.pos, -0.2, !hitA));

        
        gunTipShootPos = target.gunTip;
        target.pos.y += 20;
        target.vel.y += 500;

    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}



void m1::Tema1::Feather::UpdateFeather(float deltaTime, m1::Tema1* env)
{
    timeLeft -= deltaTime;
    spd -= spd * deltaTime;
    spd += glm::vec2{ 0,-100 }*deltaTime;
    pos += spd * deltaTime;
    angle += angularspd * deltaTime * 200.f;
    using namespace transform2D;

    float scale = min(1.f, timeLeft) * .4;

    env->RenderMesh2D(env->meshes["feather"], env->shaders["VertexColor"],
        Translate(pos.x, pos.y)
        * Rotate(angle)
        * Scale(scale, scale));
}
void m1::Tema1::DebugLine(glm::vec2 orig, glm::vec2 tip, glm::vec3 color)
{

    vector<VertexFormat> ver = { {{orig.x,orig.y,0}, color}, {{tip.x,tip.y,0}, color} };
    vector<unsigned int > indx = { 0,1 };

    Mesh* ok = new Mesh("generated grass blade");
    ok->SetDrawMode(GL_LINES);
    ok->InitFromData(ver, indx);
    RenderMesh2D(ok, shaders["VertexColor"], glm::mat3(1));
}
void Tema1::Kraken::Update(float deltaTime) {

    timeSinceAlive += deltaTime;
    hitBoxes.clear();
    damage.clear();

    const float scaleValue = 2.2;
    pos.y = 500 + sin(2 * timeSinceAlive + AI_MATH_PI) * 50 + sin(timeSinceAlive / 3 * sqrt(2)) * 50;
    pos.x = timeSinceAlive * 100 + sin(timeSinceAlive / 2) * 100;

    float angle = sin(sin(timeSinceAlive)/4.f);

    float headHeightOffset = 0;

    if (health <= 0) {
        timeSinceDeath += deltaTime;
        pos.y -= timeSinceDeath * 100;


        angle += timeSinceDeath / 10.f;
        headHeightOffset = - timeSinceDeath * 80.f;
    }


    neckPos = pos + (glm::vec2)(transform2D::Rotate(-angle) * glm::vec3{ 123, -2 ,1});

    for (int i = 0; i < heads.size(); i++)
    {
        heads[i].pos = glm::vec2{ 500 + timeSinceAlive * 100 + i* 50 + sin(timeSinceAlive * sqrt(2) + i*3) * 100 + sin(timeSinceAlive / 2) * 100 ,
            400 + cos(timeSinceAlive + i) * 200  + headHeightOffset } ;

        necks[i][0].startPoint = pos + (neckPos - pos)* scaleValue;
        necks[i][necks[i].size() - 1].endPoint = heads[i].pos;

        for (int j = 1; j < 5; j++)
        {

            glm::vec2 linkPos = necks[i][j].startPoint;
            linkPos += ((necks[i][j - 1].startPoint + necks[i][j].endPoint)/2.f - linkPos) * deltaTime * 10.f;
            
            linkPos += ((necks[i][j - 1].startPoint + glm::normalize(linkPos) * 120.f) - linkPos) * deltaTime * .5f;



            necks[i][j - 1].endPoint = linkPos;
            necks[i][j].startPoint = linkPos;
            //env->DebugLine(linkPos)
        }
    }

   // env->DebugLine(pos, neckPos, { 0,1,0 });

    glm::mat3 scale = transform2D::Scale(scaleValue, scaleValue);
    glm::mat3 model = transform2D::Translate(pos.x, pos.y );

    model *= transform2D::Rotate(-angle);
 
   // env->DebugLine(pos, neckPos, { 1,1,0 });
    
    env->RenderMesh2D(env->meshes["Kbody"], env->shaders["VertexColor"],  model * scale);
    hitBoxes.push_back(model * scale * transform2D::Translate(13.6, -27.7001) * transform2D::Scale(2.04, 0.87));
    damage.push_back(10.f);
    
    for (int i = 0; i < heads.size(); i++)
    {

        for (int j = 0; j < 5; j++)
        {
            if (env->showCollisionBoxes)

            env->DebugLine(necks[i][j].startPoint, necks[i][j].endPoint, { 0,1,0 });
            glm::vec2 neckVec = necks[i][j].endPoint - necks[i][j].startPoint;

            glm::mat3 model = transform2D::Translate(necks[i][j].startPoint.x, necks[i][j].startPoint.y);
            model *= transform2D::Rotate(-atan2(neckVec.y,neckVec.x));

            env->RenderMesh2D(env->meshes["Kneck"], env->shaders["VertexColor"],  model * scale * transform2D::Scale(glm::length(neckVec)/120,1));
            hitBoxes.push_back(model * scale * transform2D::Scale(glm::length(neckVec) / 120, 1) * transform2D::Translate(-8.7, -13.5) * transform2D::Scale(1.36, 0.5));
            damage.push_back((float)j / 4 * 20.f);
        }

        glm::vec2 headVec = glm::normalize(glm::vec2{ 50,0 } + (necks[i][4].endPoint - necks[i][4].startPoint));
        heads[i].angle = atan2(headVec.y, headVec.x);

        if (env->showCollisionBoxes)
        env->DebugLine(heads[i].pos, heads[i].pos + glm::normalize(glm::vec2{ 100,0 } + (necks[i][4].endPoint - necks[i][4].startPoint)) * 30.f, { 0,0,1 });
        
        glm::mat3 model = transform2D::Translate(heads[i].pos.x, heads[i].pos.y);
        model *= transform2D::Rotate(-heads[i].angle);
        env->RenderMesh2D(env->meshes["Khead"], env->shaders["VertexColor"],  model * scale);
        hitBoxes.push_back(model * scale * transform2D::Translate(-8.7, -13.5) * transform2D::Scale(1.36, 0.5));
        damage.push_back(35.f);
    }


    {
        glm::mat3 modelCopy = model;

        for (int i = 0; i < 4; i++) {
            model = modelCopy;
            model *= transform2D::Translate(40 + i%2 * 100, 1 + sin(2 * timeSinceAlive + i%2) * 10 - (i >=2 )*15);
            model *= transform2D::Scale(1.3f, 3);


            float baseHeight = sin(2 * timeSinceAlive + i%2) * .75 + .25;
            float tipHeight = sin(2 * timeSinceAlive - AI_MATH_HALF_PI + i%2 ) * .25 - .3;
            float baseHeightB = sin(2 * timeSinceAlive + 1 + i%2 ) * .75 + .25;
            float tipHeightB = sin(2 * timeSinceAlive - AI_MATH_HALF_PI + 1 + i%2) * .25 - .3;


            auto wingBase = model * transform2D::Scale(1, baseHeight);
            env->RenderMesh2D(env->meshes["Kwing1"], env->shaders["VertexColor"], wingBase);
            hitBoxes.push_back(wingBase * transform2D::Translate(5, 0.9) * transform2D::Scale(1.45, 0.82));
            damage.push_back(5.f);

            auto wingTip = model * transform2D::Translate(0, baseHeight * 20 * 2) * transform2D::Scale(1, tipHeight);
            env->RenderMesh2D(env->meshes["Kwing2"], env->shaders["VertexColor"], wingTip);

            auto wingBaseB = model * transform2D::Scale(1, baseHeightB);
            env->RenderMesh2D(env->meshes["wing1B"], env->shaders["VertexColor"], wingBaseB);

            auto wingTipB = model * transform2D::Translate(0, baseHeightB * 20 * 2) * transform2D::Scale(1, tipHeightB);
            env->RenderMesh2D(env->meshes["wing2B"], env->shaders["VertexColor"], wingTipB);
        }
    }


}

m1::Tema1::Kraken::Kraken(Tema1* env)
{
    this->env = env;
    pos = { -400,400 };
    neckPos = pos + glm::vec2{123, -2};
    angle = 0;

    heads.push_back({ { -200,400}, 0 });
    heads.push_back({ { -200,500}, 0 });
    heads.push_back({ { -200,600}, 0 });


    necks.resize(heads.size());
    for (int i = 0; i < heads.size(); i++)
    {
        for (int j = 0; j < 5; j++)
        {
            necks[i].push_back({ pos + neckPos, heads[i].pos });
        }
    }

    for (int i = 0; i < 100; i++)
    Update(.1);

}
