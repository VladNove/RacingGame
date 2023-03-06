#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Lab4 : public gfxc::SimpleScene
    {
    public:
        Lab4();
        ~Lab4();

        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };


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
        void CreateCube(std::string name, glm::vec3 color);
        void CreateCar();
        void CreateEngine();
        void CreateRock();

    protected:
        glm::mat4 modelMatrix;
        GLenum polygonMode;
        ViewportArea miniViewportArea;

        struct Car {

            Lab4* env;

            glm::vec3 userPos = { 0,2,0 };
            


            glm::vec3 vel{};
            glm::vec3 pos{};
            glm::quat rot = glm::quat_identity<float,glm::packed_highp>();
            glm::vec3 angularRot{};
            glm::vec3 desiredAngularAccel{};
            float heading = 0;
            float fwdSpeed = 0;
            float lrSpeed = 0;
           // float desiredAlt = 2;
            glm::vec3 pidI{0,2,0};
            //glm::vec3 desiredVel{};

            glm::vec3 desiredAccel{};

            glm::vec3 cfwd{};

            struct Engine {
                glm::vec3 relPos{};
                glm::vec3 orient{};
                float str;
                float accVal;
                glm::vec3 actualOrient;
                glm::vec3 actualRelPos;
                float queuedAccel;
                float prevAccel;
                float timeSinceStart;
                float timeSinceEnd;
                void applyForce(float t, float deltaTime, Car& car);

            }
            eYl{ {2,.5,-1.5} ,{0,0,1},1 }, eYr{ {2,.5,1.5} ,{0,0,-1},1},
                eFl{ {2,0,-1.5} ,{-sqrt(1. / 4),sqrt(2. / 4), sqrt(1. / 4)},10 }, eFr{ {2,0,1.5} ,{-sqrt(1. / 4),sqrt(2. / 4), -sqrt(1. / 4)},10 },
                eBl{ {-1.5,0,-.5} ,{sqrt(1. / 2),sqrt(1. / 2),0},10 }, eBr{ {-1.5,0,.5} ,{sqrt(1. / 2),sqrt(1. / 2),0},10 };
           // eFl{ {2,0,-2} ,{0,1,0},10 }, eFr{ {2,0,2} ,{0,1,0},10 };

            int nextCp = 0;
            bool playerCar = false;
            int cpOffset = 4;
            float autoPilotspeed = 20.f;
            float timeSinceLastCp = 0;
            float sideOffset = 0;
            float desiredHeight = 4;
            
        } car{ this };

        std::vector<Car> enemyCars;
        std::vector<Car*> allCars;

        struct Rock {
            glm::vec3 pos;
            float orient;
            float scale;
        };

        std::vector<Rock> rocks;
    
        float timeOut = 1.f;

        void DebugLine(glm::vec3 orig, glm::vec3 tip, glm::vec3 color);

        std::vector<glm::vec3> trackPoints;

        bool enablePID = true;
        bool miniMap = true;
        bool grounded = false;

        gfxc::Camera* secondCamera;
        GLint carPosLoc;
        GLint carPosLoc2;
        GLint timeLoc;
        GLint carVelLoc2;
        GLint extraTimeLoc;

    };
}   // namespace m1
