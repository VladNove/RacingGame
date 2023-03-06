#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "components/text_renderer.h"

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL);
        void RenderGroundMesh(Mesh* mesh, const glm::mat4& modelMatrix, Texture2D* texture1, glm::vec2 position);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void Render_Skier(glm::mat4 modelMatrix);
        void Render_Tree(glm::mat4 modelMatrix);
        void Render_Rock(glm::mat4 modelMatrix);
        void Render_Lamp(glm::mat4 modelMatrix);
        void Render_Gift(glm::mat4 modelMatrix);


        std::unordered_map<std::string, Texture2D*> mapTextures;



        glm::vec2 playerPos{};
        glm::vec2 playerSpeed{ 10,0 };
        struct light_source
        {
            int  type;
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 direction;
        };
        struct Object {
            glm::vec2 position;
            enum Type {GIFT,LAMP,ROCK,TREE} type ;
            light_source light;
        };

        std::vector<Object> objs;
        gfxc::TextRenderer* tr;


    };
}   // namespace m1
