#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transforml2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 200;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
  

    Mesh* square1 = object2D::CreateSquare("square", corner, squareSide, glm::vec3(1, 1, 1));
    Mesh* square2 = object2D::CreateSquare("squareFill", corner, squareSide, glm::vec3(1, 1, 1), true);
   // Mesh* test = object2D::CreateTest("test");
    AddMeshToList(square1);
    AddMeshToList(square2);
    //AddMeshToList(test);

}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
float rotCol = 0;

void Lab3::Update(float deltaTimeSeconds)
{
    rotCol += deltaTimeSeconds;
    glPointSize(5);

    RendPoints();
    RendMesh();
    RendLines();

    if (!points.empty()) {
        RenderMesh2D(meshes["points"], shaders["VertexColor"], transforml2D::Translate(0, 0));

        if (!triangles.empty()) {
            RenderMesh2D(meshes["mesh"], shaders["VertexColor"], transforml2D::Translate(0, 0));
            RenderMesh2D(meshes["lines"], shaders["VertexColor"], transforml2D::Translate(0, 0));
        }

        closestPoint = 0;
        for (int i = 1; i < points.size(); i++)
        {
            if (glm::length(mousePos - points[closestPoint]) > glm::length(mousePos - points[i]))
                closestPoint = i;
        }
     
        

        boolIsClose = glm::length(mousePos - points[closestPoint]) < 20;

        

        if (boolIsClose)
            RenderMesh2D(meshes["square"], 
                transforml2D::Translate(points[closestPoint].x-10, points[closestPoint].y-10) * transforml2D::Scale(.1,.1), { 0,1,0 });

            for (auto p : selectedPoints) {
                RenderMesh2D(meshes["square"],
                    transforml2D::Translate(points[p].x - 10, points[p].y - 10) * transforml2D::Scale(.1, .1), { .5,.5,0 });
            }
            for (auto p : selectedColourPoints) {
                RenderMesh2D(meshes["square"],
                    transforml2D::Translate(points[p].x - 10, points[p].y - 10) * transforml2D::Scale(.1, .1), { 0,.5,.5 });
            }
        

    }
    else
    {
        boolIsClose = false;
    }

    


    RenderMesh2D(meshes["square"], transforml2D::Translate(0, 100)* transforml2D::Scale(1,.25), { 1,0,0 });
    RenderMesh2D(meshes["square"], transforml2D::Translate(0, 50) * transforml2D::Scale(1, .25), { 0,1,0 });
    RenderMesh2D(meshes["square"], transforml2D::Translate(0, 0) * transforml2D::Scale(1, .25), { 0,0,1 });

    RenderMesh2D(meshes["squareFill"], transforml2D::Translate(0, 100) * transforml2D::Scale(Rper, .25), { 1,0,0 });
    RenderMesh2D(meshes["squareFill"], transforml2D::Translate(0, 50) * transforml2D::Scale(Gper, .25), { 0,1,0 });
    RenderMesh2D(meshes["squareFill"], transforml2D::Translate(0, 0) * transforml2D::Scale(Bper, .25), { 0,0,1 });

    RenderMesh2D(meshes["square"], transforml2D::Translate(600, 400), { 1,1,1 });
    RenderMesh2D(meshes["square"], transforml2D::Translate(600, 200), { 1,1,1 });
    RenderMesh2D(meshes["square"], transforml2D::Translate(400, 400), { 1,1,1 });
    RenderMesh2D(meshes["square"], transforml2D::Translate(400, 200), { 1,1,1 });
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{

}


void Lab3::OnKeyPress(int key, int mods)
{

    if (key == GLFW_KEY_C) {
        selectedColourPoints.clear();
        colouring = true;
        if (boolIsClose)
            selectedColourPoints.insert(closestPoint);
    }

    if (key == GLFW_KEY_D) {

        if (boolIsClose && !selectingTr) {
            points.erase(points.begin() + closestPoint);
            colours.erase(colours.begin() + closestPoint);

            set<int> newSelected;
            
           
            auto it = triangles.begin();

            while (it != triangles.end()) {
                bool contains = false;
                for (int& t : *it) {
                    if (t == closestPoint)
                        contains = true;
                }
                if (contains) {
                    it = triangles.erase(it);
                }
                else ++it;
            }

            for (int i = 0; i < triangles.size(); i++)
                for (int& t : triangles[i])
                    if (t > closestPoint)
                        t--;

            for (auto i : selectedColourPoints)
            {
                if (i == closestPoint)
                    continue;
                newSelected.insert(i - (int)(i > closestPoint));
            }

            selectedColourPoints = newSelected;
        }
    }

    if (key == GLFW_KEY_S) {
        cout << endl << endl;
        cout << "vertices = {" << endl;
        for (int i = 0; i < points.size(); i++)
        {
            cout << "{ {" << points[i].x-600 << "," << points[i].y-400 << "," << 0 << "} , {" 
                << colours[i].x << "," << colours[i].y << "," << colours[i].z << "} }," << endl;
        }
        cout << "}" << "\n\n\n" << "indices = { " << endl;
        for (auto tri : triangles) {
            for (int i : tri)
                cout << i << ",";
            cout << endl;
        }
        cout << "}" << endl;
    }
}


void Lab3::OnKeyRelease(int key, int mods)
{
    if (key == GLFW_KEY_C) {
        colouring = false;
    }
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mousePos = { mouseX, window->GetResolution().y - mouseY };

    if (pointHeld)
        points[closestPoint] = mousePos;


    if (colourHeld)
    {
        *selColor = min((float)mouseX / 200,1.f);

        for (int i : selectedColourPoints) {
            colours[i] = { Rper,Gper,Bper };
        }
    }

    //cout << boolIsClose <<" " <<colouring<< endl;


    if (colouring && boolIsClose)
    {
        //cout << "INSERTED!!!!" << endl;
        selectedColourPoints.insert(closestPoint);
    }



    if (selectingTr && boolIsClose)
    {
        bool alreadyIn = false;
        for (int i = 0; i < selectedPoints.size(); i++)
        {
            if (selectedPoints[i] == closestPoint)
                alreadyIn = true;
        }

        if (!alreadyIn) {
            selectedPoints.push_back(closestPoint);
            if (selectedPoints.size() >= 3) {
                triangles.push_back({ selectedPoints[0], selectedPoints[selectedPoints.size() - 2], closestPoint });
  
            }
        }

    }
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    mouseY = window->GetResolution().y - mouseY;
    if (button == 1) {

        if (mouseX < 200 && mouseY < 150) {
            colourHeld = true;
            if (mouseY < 50)
                selColor = &Bper;
            else if (mouseY < 100)
                selColor = &Gper;
            else
                selColor = &Rper;


        } else {
            
            if (!boolIsClose) {
                points.push_back({ mouseX, mouseY });
                colours.push_back({ Rper,Gper,Bper });
                closestPoint = points.size();
            }
            pointHeld = true;
        }
    }
    if (button == 2) {
        selectingTr = true;
        
    }
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1) {
        pointHeld = false;
        colourHeld = false;
    }
    if (button == 2) {
        selectingTr = false;
        selectedPoints.clear();
    }
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}

void m1::Lab3::RendPoints()
{

    if (points.empty())
        return;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    int i = 0;
    for (auto p : points) {
        vertices.push_back(VertexFormat{ {p.x,p.y,-.5}, colours[i] });
        indices.push_back(i++);
    }


Mesh* square = new Mesh("points");
square->SetDrawMode(GL_POINTS);
square->InitFromData(vertices, indices);
AddMeshToList(square);



}




void m1::Lab3::RendMesh()
{
    if (points.empty() || triangles.empty())
        return;

    std::vector<VertexFormat> vertices;



    std::vector<unsigned int> indices;
    int i = 0;
    for (auto p : points) {
        vertices.push_back(VertexFormat{ {p.x,p.y,-1}, colours[i++] });
        /*
        vertices.push_back(VertexFormat{ {p.x,p.y,-1}, {(sin(rotCol + 0) + 1) / 2,
            (sin(rotCol + AI_MATH_TWO_PI / 3) + 1) / 2,
            (sin(rotCol + 2 * AI_MATH_TWO_PI / 3) + 1) / 2 } });
        */
    }

    for (auto tri : triangles) {
        for (int i : tri)
            indices.push_back(i);
    }


    Mesh* square = new Mesh("mesh");
    square->SetDrawMode(GL_TRIANGLES);
    square->InitFromData(vertices, indices);
    AddMeshToList(square);


}

void m1::Lab3::RendLines()
{
    if (points.empty() || triangles.empty())
        return;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    for (auto p : points) {
        vertices.push_back(VertexFormat{ {p.x,p.y,-.7},{0,0,0} });
    }

    for (auto tri : triangles) {

        indices.push_back(tri[0]);

        for (int i = 1; i < tri.size(); i++)
        {
            indices.push_back(tri[i]);
            indices.push_back(tri[i]);
        }

        indices.push_back(tri[0]);
    }


    Mesh* square = new Mesh("lines");
    square->SetDrawMode(GL_LINES);
    square->InitFromData(vertices, indices);
    AddMeshToList(square);
}
