#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


#define V3(playerPos) {playerPos.x, 0, playerPos.y}
#define randF() ((float)rand()/RAND_MAX) 


void Tema3::Init()
{

    tr = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x - window->GetResolution().x / 4, window->GetResolution().y);

    //std::cout << "C:\\Users\\vladn\\Desktop\\EGC\\gfx-framework-master\\assets\\fonts\\Hack-Bold.ttf" << endl;
    tr->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);



    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "textures");



    auto loadModelandTexture = [&](string name) {
        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures", name + ".png").c_str(), GL_REPEAT);
            mapTextures[name] = texture;
        }
        {
            Mesh* mesh = new Mesh(name);
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "models"), name + ".obj");
            meshes[mesh->GetMeshID()] = mesh;
        }
    };

   

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("TexShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("GroundShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderG.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    loadModelandTexture("tree_top");
    loadModelandTexture("rock");
    loadModelandTexture("tree_trunk");
    loadModelandTexture("light_pole");
    loadModelandTexture("gift");
    loadModelandTexture("light_top");
    loadModelandTexture("ski");
    loadModelandTexture("skier");
    loadModelandTexture("ground");


    while (objs.size() < 20) {
        Object newObj;
        newObj.position.x = playerPos.x - 20 + randF() * 40;
        newObj.position.y = playerPos.y - 20 + randF() * 40;
        newObj.type = (Object::Type)(rand() % 4);
        


        objs.push_back(newObj);
    }
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}



static float hitCooldown;

static int score = 0;
static int hp = 100;

void Tema3::Update(float deltaTimeSeconds)
{
    hitCooldown += deltaTimeSeconds;

    if (hitCooldown > 1.f)
            hitCooldown = 1.f;

    playerPos += glm::normalize(playerSpeed) * 10.f * hitCooldown * deltaTimeSeconds;
    




    GetSceneCamera()->SetPositionAndRotation({ playerPos.x + 15, 15, playerPos.y },
        glm::quatLookAt(glm::normalize(glm::vec3{ playerPos.x+5, 0, playerPos.y } - glm::vec3{ playerPos.x + 15, 15, playerPos.y }), { 0,1,0 }));


    glm::mat4 skier = glm::mat4(1);
    skier = glm::translate(skier, V3(playerPos));
    skier *= glm::toMat4(glm::rotation(glm::vec3{ 1, 0, 0 }, glm::normalize(glm::vec3(playerSpeed.x,0, playerSpeed.y))));

    bool notHit = hitCooldown == 1.f;

    for (auto o : objs) {
        float d = glm::length(o.position - playerPos);

        if (o.type == o.ROCK && d < 1.f)
            hitCooldown = 0.1f;
        if (o.type == o.TREE && d < 2.f)
            hitCooldown = 0.1f;
        if (o.type == o.LAMP && d < 0.5f)
            hitCooldown = 0.1f;
               
    }

    if (notHit &&hitCooldown == 0.1f)
        hp -= 10;


    vector<Object> remainingObjs;
    for (auto o : objs) {
        if (glm::length(o.position - playerPos) < 30.f) {
            
            if (o.type == o.GIFT && glm::length(o.position - playerPos) < 1.f) {
                score++;
                continue;
            }
            remainingObjs.push_back(o);
        }
    }



    objs = remainingObjs;





    while (objs.size() < 20) {
        Object newObj;
        newObj.position.x = playerPos.x + 20 + randF() * 20;
        newObj.position.y = playerPos.y - 20 + randF() * 40;
        newObj.type = (Object::Type)(rand() % 4);
        objs.push_back(newObj);
    }


    int N_lights = 0;


    auto addLight = [&](Object o) {


        glUseProgram(shaders["GroundShader"]->program);

        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].position");
            GLuint location = glGetUniformLocation(shaders["GroundShader"]->program, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(o.light.position));
        }
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].color");
            GLuint location = glGetUniformLocation(shaders["GroundShader"]->program, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(o.light.color));
        }
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].type");
            GLuint location = glGetUniformLocation(shaders["GroundShader"]->program, name.c_str());
            glUniform1i(location, o.light.type);
        }
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].direction");
            GLuint location = glGetUniformLocation(shaders["GroundShader"]->program, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(o.light.direction));
        }

        glUseProgram(shaders["TexShader"]->program);
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].position");
            GLuint location = glGetUniformLocation(shaders["TexShader"]->program, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(o.light.position));
        }
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].color");
            GLuint location = glGetUniformLocation(shaders["TexShader"]->program, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(o.light.color));
        }
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].type");
            GLuint location = glGetUniformLocation(shaders["TexShader"]->program, name.c_str());
            glUniform1i(location, o.light.type);
        }
        {
            std::string name = std::string("lights[") + std::to_string(N_lights) + std::string("].direction");
            GLuint location = glGetUniformLocation(shaders["TexShader"]->program, name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(o.light.direction));
        }


        N_lights++;
    };
   


    for (auto o : objs) {
        if (o.type == o.GIFT) {
            
            o.light.color = { 1,.5,.5 };
            o.light.position = { o.position.x,1,o.position.y };
            o.light.type = 0;
            
            addLight(o);
        }
        if (o.type == o.TREE) {

            o.light.color = { .5,1,.5 };
            o.light.position = { o.position.x,2,o.position.y };
            o.light.type = 0;

            addLight(o);
        }
        if (o.type == o.LAMP) {

            o.light.color = { 1,1,.3 };
            o.light.position = { o.position.x + 1,7,o.position.y };
            o.light.direction = { 0,-1,0 };
            o.light.type = 1;

            addLight(o);
        }

    }

    glUseProgram(shaders["GroundShader"]->program);

    {
        glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
        int eye_position = glGetUniformLocation(shaders["GroundShader"]->program, "eye_position");
        glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);
    }

    {
        int pos = glGetUniformLocation(shaders["GroundShader"]->program, "N_lights");
        glUniform1i(pos, N_lights);
    }
    glUseProgram(shaders["TexShader"]->program);
    {
        glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
        int eye_position = glGetUniformLocation(shaders["TexShader"]->program, "eye_position");
        glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);
    }

    {
        int pos = glGetUniformLocation(shaders["TexShader"]->program, "N_lights");
        glUniform1i(pos, N_lights);
    }




    Render_Skier(skier);
    for (auto o : objs) {
        if (o.type == Object::Type::TREE)
            Render_Tree(glm::translate(glm::mat4(1), V3(o.position)));
        if (o.type == Object::Type::ROCK)
            Render_Rock(glm::translate(glm::mat4(1), V3(o.position)));
        if (o.type == Object::Type::LAMP)
            Render_Lamp(glm::translate(glm::mat4(1), V3(o.position)));
        if (o.type == Object::Type::GIFT)
            Render_Gift(glm::translate(glm::mat4(1), V3(o.position)));
    }
    RenderGroundMesh(meshes["ground"], glm::translate(glm::mat4(1), V3(playerPos)), mapTextures["ground"], playerPos);

    string text;
    text = "score: " + std::to_string(score);
    tr->RenderText(text, 0, 50, .3);
    text = "hp: " + std::to_string(hp);
    tr->RenderText(text, 0, 100, .3);

    if (hp == 0) {
        hp = 100;
        score = 0;
    }

}


void Tema3::FrameEnd()
{

}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderGroundMesh(Mesh* mesh, const glm::mat4& modelMatrix, Texture2D* texture1, glm::vec2 position) {

    Shader* shader = shaders["GroundShader"];
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glUniform2fv(glGetUniformLocation(shader->program, "textureOffs"), 1, glm::value_ptr(position));
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    playerSpeed.y = -((float)mouseX / window->GetResolution().x - 0.5) * 40;
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}



void m1::Tema3::Render_Skier(glm::mat4 modelMatrix)
{
   
    modelMatrix = glm::scale(modelMatrix, glm::vec3(.4));
    RenderSimpleMesh(meshes["skier"], shaders["TexShader"], modelMatrix, mapTextures["skier"]);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
    modelMatrix = glm::translate(modelMatrix,{ 0.5, 0, 1 });
    RenderSimpleMesh(meshes["ski"], shaders["TexShader"], modelMatrix, mapTextures["ski"]);
    modelMatrix = glm::translate(modelMatrix,{ 0, 0, -2 });
    RenderSimpleMesh(meshes["ski"], shaders["TexShader"], modelMatrix, mapTextures["ski"]);
}
void m1::Tema3::Render_Tree(glm::mat4 modelMatrix)
{

    modelMatrix = glm::rotate(modelMatrix, 30.f * 0.0174533f, glm::vec3{ 0,0,1 });
    RenderSimpleMesh(meshes["tree_trunk"], shaders["TexShader"], modelMatrix, mapTextures["tree_trunk"]);
    RenderSimpleMesh(meshes["tree_top"], shaders["TexShader"], modelMatrix, mapTextures["tree_top"]);
}
void m1::Tema3::Render_Rock(glm::mat4 modelMatrix) {
    modelMatrix = glm::rotate(modelMatrix, 30.f *0.0174533f, glm::vec3{ 0,0,1 });

    modelMatrix = glm::scale(modelMatrix, glm::vec3(.4));
    RenderSimpleMesh(meshes["rock"], shaders["TexShader"], modelMatrix, mapTextures["rock"]);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1, -.2, .2));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(.4));

    RenderSimpleMesh(meshes["rock"], shaders["TexShader"], modelMatrix, mapTextures["rock"]);
}
void m1::Tema3::Render_Lamp(glm::mat4 modelMatrix) {
    modelMatrix = glm::rotate(modelMatrix, 30.f*0.0174533f, glm::vec3{ 0,0,1 });

        modelMatrix = glm::scale(modelMatrix, glm::vec3(.5,0.75,.5));
        RenderSimpleMesh(meshes["light_pole"], shaders["TexShader"], modelMatrix, mapTextures["light_pole"]);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, 2.9, 0)*2.f);
        RenderSimpleMesh(meshes["light_top"], shaders["TexShader"], modelMatrix, mapTextures["light_top"]);
    
}

void m1::Tema3::Render_Gift(glm::mat4 modelMatrix)
{
    //modelMatrix = glm::rotate(modelMatrix, 30.f * 0.0174533f, glm::vec3{ 0,0,1 });

    RenderSimpleMesh(meshes["gift"], shaders["TexShader"],
        glm::scale(modelMatrix, glm::vec3(0.5)), mapTextures["gift"]);
}
