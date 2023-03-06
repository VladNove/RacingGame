#pragma once

#include "components/simple_scene.h"
#include <unordered_map>
#include "components/text_renderer.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

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
   
        void CreateCar();
        void CreateEngine();
        void CreateRock();
        void GetTrackPoints();
    protected:
        

        glm::mat4 modelMatrix;
        GLenum polygonMode;
        ViewportArea miniViewportArea;

        struct Car {

            Tema2* env;

            glm::vec3 userPos = { 0,2,0 };



            glm::vec3 vel{};
            glm::vec3 pos{};
            glm::quat rot = glm::quat_identity<float, glm::packed_highp>();
            glm::vec3 angularRot{};
            glm::vec3 desiredAngularAccel{};
            float heading = 0;
            float fwdSpeed = 0;
            float lrSpeed = 0;
            // float desiredAlt = 2;
            glm::vec3 pidI{ 0,2,0 };
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
            eYl{ {2,.5,-1.5} ,{0,0,1},1 }, eYr{ {2,.5,1.5} ,{0,0,-1},1 },
                eFl{ {2,0,-1.5} ,{-sqrt(1. / 4),sqrt(2. / 4), sqrt(1. / 4)},10 }, eFr{ {2,0,1.5} ,{-sqrt(1. / 4),sqrt(2. / 4), -sqrt(1. / 4)},10 },
                eBl{ {-1.5,0,-.5} ,{sqrt(1. / 2),sqrt(1. / 2),0},10 }, eBr{ {-1.5,0,.5} ,{sqrt(1. / 2),sqrt(1. / 2),0},10 };
            // eFl{ {2,0,-2} ,{0,1,0},10 }, eFr{ {2,0,2} ,{0,1,0},10 };


            //ai stuff
            int nextCp = 0;
            int cpOffset = 4;
            float autoPilotspeed = 20.f;
            float timeSinceLastCp = 0;
            float sideOffset = 0;
            float desiredHeight = 4;


            //player stuff
            bool playerCar = true;
            bool enablePID = true;
            float timeOut = 1.f;
            bool grounded = false;
            glm::vec3 oldCameraPos{};
            glm::vec3 oldLookAtPoint{};

            gfxc::Camera playerCamera;
            bool secondPlayer = false;


            float lapTime = 0;
            float prevLapTime = INFINITY;
            float bestLapTime = INFINITY;

            bool key_W, key_A, key_D, key_SHIFT;

        } car{ this }, player2car{ this }, player3car{ this }, player4car{ this };

        struct ModelRenderInfo {
            std::string mesh;
            glm::mat4 model;
        };
        struct DustRenderInfo {
            float tss;
            float tse;
            glm::vec3 carVel;
            glm::mat4 dustMat;
            std::string mesh;
            glm::vec3 carPos;
        };
        struct LineRenderInfo {
            glm::vec3 orig;
            glm::vec3 tip;
            glm::vec3 col;
            glm::vec3 carPos;
            float scale = 6.5 / 5;
        };

        std::vector<ModelRenderInfo> ri_m;
        std::vector<DustRenderInfo> ri_d;
        std::vector<LineRenderInfo> ri_l;


        std::vector<Car> enemyCars;
        std::vector<Car*> allCars;

        struct Rock {
            glm::vec3 pos;
            float orient;
            float scale;
        };

        std::vector<Rock> rocks;

        void CarInputKeyboard(Car& car, float keyCodeW, float keyCodeS, float keyCodeA, float keyCodeD, float keyCodeShift, float deltaTime);

        void DebugLine(glm::vec3 orig, glm::vec3 tip, glm::vec3 color);
        void CarUpdateWithTimer(Car& car, float deltaTimeSeconds);
        void UpdateCar(Car& car, float deltaTimeSeconds);
        void RenderScene(Car& car);
        void RenderMiniMap(Car& car);
        void ResetCar(Car& car);
        std::vector<glm::vec3> trackPoints;

        bool miniMap = true;
        bool p2miniMap = true;
        bool secondPlayer = false;
        bool thirdPlayer = false;
        bool fourthPlayer = false;
        gfxc::Camera secondCamera;
        GLint carPosLoc;
        GLint carPosLoc2;
        GLint timeLoc;
        GLint carVelLoc2;
        GLint extraTimeLoc;
        

        std::unordered_map<float, Mesh*> lines;
        gfxc::TextRenderer* tr;
        bool showTime =false;

        glm::vec3 bgColor;
        GLint bgColorLoc;
        GLint bgColorLoc2;

        const bool generatingTrack = false;
        int track =0;

        bool reInit = false;
        bool showFps = false;
        bool permaBoost = false;

        
    };
}   // namespace m1

