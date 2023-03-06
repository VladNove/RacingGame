#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace objects2D
{
    
    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateWing1(const std::string& name, float color, float scale = 20, float plane = -1, int special = 0);
    Mesh* CreateWing2(const std::string& name, float color, float scale = 20, float plane = 0, bool damage = false, int special = 0);
    Mesh* CreateBody(const std::string& name, float scale = 20, float plane = 0, bool damage = false, int special = 0);
    Mesh* CreateHead(const std::string& name, float scale = 20, float plane = 0, bool damage = false);
    Mesh* CreateNeck(const std::string& name, float scale = 20, float plane = 0, bool damage = false);
    Mesh* CreateCircle(const std::string& name, float radius = 20, float plane = 0, int Points = 10, glm::vec3 color = { 1,0,0 }, bool fill = false);
    Mesh* CreateGrass(const std::string& name, float height = 20, float plane = 0, float spacing = 2, int blades = 10);
    Mesh* CreateBulletTrail(const std::string& name, glm::vec2 start, glm::vec2 end, float plane = 0, bool miss = false);
    Mesh* CreateBullet(const std::string& name, float plane);
    Mesh* CreateHeart(const std::string& name, float plane);
    Mesh* CreateMuzzleFlash(const std::string& name, float planee);
    Mesh* CreateFeather(const std::string& name, float plane);
}

/*

TODO: clouds
TODO: X eyes on death
TODO: different color ducks
TODO: killing spree
TODO: boss fight (bigger duck, more health)
TODO: feathers upon shot


*/