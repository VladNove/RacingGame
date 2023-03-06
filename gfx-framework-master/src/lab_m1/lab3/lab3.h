#pragma once

#include "components/simple_scene.h"

#include <set>
namespace m1
{
    class Lab3 : public gfxc::SimpleScene
    {
     public:
        Lab3();
        ~Lab3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RendPoints();
        void RendMesh();
        void RendLines();
     protected:

        std::vector <glm::vec2> points;
        std::vector <glm::vec3> colours;


        std::vector <std::vector<int>> triangles;
        std::vector <int> selectedPoints;
        glm::vec2 mousePos;


        int boolIsClose = false;
        int closestPoint;

        bool pointHeld = false;

        bool selectingTr = false;

        float  Rper = .5, Gper = .5, Bper = .5;

        bool colourHeld = false;
        float* selColor;

        bool colouring = false;
        std::set <int> selectedColourPoints;


    };
}   // namespace m1
