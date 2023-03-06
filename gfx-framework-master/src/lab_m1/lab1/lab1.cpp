#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    bg = { 0,0,0 };
    bgColorEnum = BLACK;
    position = glm::vec3(3, .75, 0);
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}


void Lab1::FrameStart()
{
}


static float timef = 0;

void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(bg.r, bg.g, bg.b, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    timef += deltaTimeSeconds;

    
    colorRange += deltaTimeSeconds / 3;
    bg.r = (sin(colorRange * AI_MATH_TWO_PI) + 1) / 2;
    bg.g = (sin(colorRange * AI_MATH_TWO_PI * 2) + 1) / 2;
    bg.b = (sin(colorRange * AI_MATH_TWO_PI * 4) + 1) / 2;

    float sum = sqrt(bg.r * bg.r + bg.g * bg.g + bg.b * bg.b);
    bg.r /= sum;
    bg.g /= sum;
    bg.b /= sum;
    
    // Render the object


    glm::vec3 pos;
    //glm::vec3(1, 0.5f, 0)
    if (position.y > 0)
        verticalSpeed -= 9.8 * deltaTimeSeconds;

    position.y += verticalSpeed * deltaTimeSeconds;

    if (position.y < 0)
        position.y = 0;

    for (int i = 0; i <= 1000; i++) {

        float t = (float)i / 1000 * 10;

        pos = { sin(t), sin(timef) + sin((sin(timef ) + 2) * t) / 3, cos(t) };

        RenderMesh(meshes["box"],pos , glm::vec3(0.5f));
    }
    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-3, 0.5f, 0));

   

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    RenderMesh(meshes[selectableModels[selectedModel]], position);


}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        return;

    if (window->KeyHold(GLFW_KEY_W))
        position.x += deltaTime * objSpeed;
    if (window->KeyHold(GLFW_KEY_S))
        position.x -= deltaTime * objSpeed;
    if (window->KeyHold(GLFW_KEY_E)) {
        if (position.y == 0)
            verticalSpeed = 7;
    }
    if (window->KeyHold(GLFW_KEY_Q))
        position.y -= deltaTime * objSpeed;
    if (window->KeyHold(GLFW_KEY_D))
        position.z += deltaTime * objSpeed;
    if (window->KeyHold(GLFW_KEY_A))
        position.z -= deltaTime * objSpeed;

    if (window->KeyHold(GLFW_KEY_H)) {
        

    }

    

}




void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.

        if (bgColorEnum == WHITE)
            bgColorEnum = BLACK;
        else
            bgColorEnum = (Colors)(bgColorEnum + 1);
        
        bg = colors[(int)bgColorEnum];
        
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

    if (key == GLFW_KEY_G) {
        // TODO(student): Change the values of the color components.

        if (selectedModel == 2)
            selectedModel = 0;
        else
            selectedModel++;

    }

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
