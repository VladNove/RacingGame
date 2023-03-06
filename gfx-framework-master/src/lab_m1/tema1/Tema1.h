#pragma once

#include "components/simple_scene.h"

#include "components/text_renderer.h"
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

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
    protected:
        
        glm::vec2 initRes;

        glm::mat3 modelMatrix;
        float Time = 0;
        float TimeScale =  1;
        float DuckTimeScale = 1;
        void RenderDuck(glm::mat3 modelMatrix, float deltaTimeSeconds);
        void CreateDuck(bool slowMoAble = false);
        void RenderKraken(float deltaTimeSeconds);

        void UpdateTarget(float deltaTimeSeconds);

        void RenderWeapon(float xOffs, float gunLenghtScale, float sideLenght, float topLenght, float sideVOffset, float sideHOffset);

        struct
        {
            glm::vec2 pos{};
            glm::vec2 vel{};
            const float maxAcc = 10000;
            const float maxVel = 10000;
            const float P = 250;
            const float D = -20;

            glm::vec2 gunTip;
        } target;

        struct Duck {

            m1::Tema1* env;
            float headAngle = 0;
            float headSpeed = 0;
            float headDesiredDir = -AI_MATH_HALF_PI;
            float timeOfLastUpdate = 0;

            float Fwing2Offs = 0;
            float Bwing2Offs = 1;

            float flapSpeed = 4;

            float bodyAngle = 0;

            int flipped = 1;
            glm::vec2 pos = { 0,0 };
            bool dead = false;
            bool evading = false;

            float timeTillEvade = 10;

            int specialPower = 0; // 1 -> extraHealth, 2 -> extraBullet, 3 -> slowMo

            glm::mat3 wingBox, bodyBox, headBox;
            glm::vec3 mouseColBox, wingColBox, headColBox;
            std::string extraMeshId = "";

            glm::vec2 velo;

            void UpdateHead(float deltaTime);
            void UpdateBody(float deltaTime);
        };


        bool bossFight;
        struct Kraken {
            glm::vec2 pos;
            glm::vec2 neckPos;
            float angle;
           

            struct Head {
                glm::vec2 pos;
                
                float angle;
                bool dead = false;
            };

            struct Neck {
                glm::vec2 startPoint;
                glm::vec2 endPoint;
            };

            std::vector<Head> heads;
            std::vector<std::vector<Neck>> necks;

            float health = 100;
            std::vector<glm::mat3> hitBoxes;
            std::vector<glm::vec3> mouseCol;
            std::vector<float> damage;

            float timeSinceDeath = 0;
            float timeSinceAlive = -20;


            Kraken() {}

            Kraken(Tema1* env);

            void Update(float deltaTime);

            Tema1* env;
            
        } kraken;


        std::vector<Duck> ducks;

        glm::vec2 mousePos;
        bool showCollisionBoxes = false;

        float gunModelx = 1000;
        float timeSinceLastShot = 10;
        

        glm::vec2 gunTipShootPos;

        int lives = 3;
        int bullets = 3;

        std::vector<float> bulletStates = { 0,0,0 };
        std::vector<float> heartStates = { 0,0,0 };
        bool lost = false;

        int score = 0;
        gfxc::TextRenderer *tr;
        int level = 1;

        void incScore(int x);

        void RenderWings(Duck& duck, glm::mat3 modelMatrix, float deltaTimeSeconds);
        int extraMeshId = 0;

        float timeToNextDuck = 1;
        float timeToNextWaitingDuck = 0;
        int ducksWaiting = 0;

        int maxBullets = 3;
        int maxHearts = 3;

        float slowMoTimeLeft = 0;


        struct Feather {
            glm::vec2 pos;
            glm::vec2 spd;
            float angle;
            float angularspd;
            float timeLeft = 1;
            void UpdateFeather(float deltaTime, m1::Tema1* env);
        };

        std::vector<Feather> feathers;

        void DebugLine(glm::vec2 orig, glm::vec2 tip, glm::vec3 color);
        
        
    };
}   // namespace m1
