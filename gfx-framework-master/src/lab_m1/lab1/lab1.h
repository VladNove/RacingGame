#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab1 : public gfxc::SimpleScene
    {
     public:
        Lab1();
        ~Lab1();

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


        

        struct Color {
            GLclampf r,g,b;
        } bg;

        enum Colors {
            BLACK, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE
        } bgColorEnum;

        const Color colors[8] = { 
        { 0,0,0 },
        { 1,0,0 },
        { 0,1,0 },
        { 0,0,1 },
        { 1,1,0 },
        { 0,1,1 },
        { 1,0,1 },
        { 1,1,1 } };

        float colorRange = 0;


        std::string selectableModels[3] = { "teapot","box","sphere" };
        int selectedModel = 0;

        glm::vec3 position;
        const float objSpeed = 2;
        float verticalSpeed = 0;


    };
}   // namespace m1
