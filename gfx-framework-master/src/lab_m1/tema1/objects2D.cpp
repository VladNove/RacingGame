#include "objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* objects2D::CreateWing1(const std::string& name, float color, float scale, float plane, int special)
{


    std::vector<VertexFormat> vertices =
    {
        VertexFormat({0,0,plane}, color * glm::vec3{.61,.47,.41}),
        VertexFormat({1 * scale,2 * scale,plane}, color * glm::vec3{.61,.57,.51}),
        VertexFormat({4 * scale,2 * scale,plane}, color * glm::vec3{.61,.47,.41}),
        VertexFormat({3 * scale,0,plane}, color * glm::vec3{.61,.47,.41})
    };

    if (special) {
        glm::vec3 scol;
        if (special == 1)
            scol = { 1,0,0 };
        if (special == 2)
            scol = { 1,1,0 };
        if (special == 3)
            scol = { 0,0,1 };

        for (auto& ver : vertices)
            ver.color = glm::length(ver.color) * scol;
    }

    Mesh* square = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0};


    square->InitFromData(vertices, indices);
    return square;
}
Mesh* objects2D::CreateWing2(const std::string& name, float color, float scale, float plane, bool damage, int special)
{


    std::vector<VertexFormat> vertices =
    {
        VertexFormat({1 * scale,0,plane}, color * glm::vec3{.61,.57,.51}),
        VertexFormat({0,4 * scale,plane}, color * glm::vec3{.61,.67,.61}),
        VertexFormat({3 * scale,3 * scale,plane}, color * glm::vec3{.61,.47,.41}),
        VertexFormat({4 * scale,0,plane}, color * glm::vec3{.61,.47,.41}),
    };


    if (special) {
        glm::vec3 scol;
        if (special == 1)
            scol = { 1,0,0 };
        if (special == 2)
            scol = { 1,1,0 };
        if (special == 3)
            scol = { 0,0,1 };

        for (auto& ver : vertices)
            ver.color = glm::length(ver.color) * scol;
    }


    Mesh* square = new Mesh(name);
    if (damage)
    {
        if (special != 1)
            vertices[rand() % vertices.size()].color = { .8,0,0 };
        else
            vertices[rand() % vertices.size()].color = { 0,0,0 };
    }


    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0};


    square->InitFromData(vertices, indices);
    return square;
}
Mesh* objects2D::CreateBody(const std::string& name, float scale, float plane, bool damage, int special)
{
    

    std::vector<VertexFormat> vertices =
    {
        VertexFormat({0 * scale, 0 * scale, plane}, glm::vec3{.61,.57,.51}),
        VertexFormat({3.2 * scale, 1.005 * scale, plane}, glm::vec3{.61,.57,.51}),
        VertexFormat({5.16 * scale, 1.01 * scale, plane}, glm::vec3{.61,.57,.51}),
        VertexFormat({8.64 * scale, 1.09 * scale, plane}, glm::vec3{.61,.57,.51}),
        VertexFormat({10.28 * scale, 0.9 * scale, plane}, glm::vec3{.51,.37,.31}),

        VertexFormat({11.3 * scale, 0.59 * scale, plane}, glm::vec3{.51,.37,.31}),
        VertexFormat({12.8 * scale, 0.26 * scale, plane}, glm::vec3{.51,.37,.31}),
        VertexFormat({12.76 * scale, -0.73 * scale, plane}, glm::vec3{.51,.37,.31}),
        VertexFormat({11.45 * scale, -1.12 * scale, plane}, glm::vec3{.51,.37,.31}),


        VertexFormat({10.23 * scale, -2.36 * scale, plane}, glm::vec3{.51,.37,.31}),
        VertexFormat({7.99 * scale, -3.03 * scale, plane}, glm::vec3{.51,.37,.31}),
        VertexFormat({4.34 * scale, -2.75 * scale, plane}, glm::vec3{.61,.57,.51}),
        VertexFormat({2.27 * scale, -1.62 * scale, plane}, glm::vec3{.61,.57,.51}),
    };

    Mesh* square = new Mesh(name);

    if (special) {
        glm::vec3 scol;
        if (special == 1)
            scol = { 1,0,0 };
        if (special == 2)
            scol = { 1,1,0 };
        if (special == 3)
            scol = { 0,0,1 };

        for (auto& ver : vertices)
            ver.color = glm::length(ver.color) * scol;
    }

    if (damage)
    {
        int dmgV = rand() % vertices.size();
        vertices[dmgV].color = { .8,0,0 };
        vertices[(dmgV+1)%vertices.size()].color = { .8,0,0 };
        vertices[(dmgV + 2) % vertices.size()].color = { .8,0,0 };
    }

    square->SetDrawMode(GL_TRIANGLE_STRIP);
    std::vector<unsigned int> indices = {
    0,1,12,2,11,3,10,4,9,5,8,6,7
    };

    square->InitFromData(vertices, indices);
    return square;
}

#include <iostream>

Mesh* objects2D::CreateHead(const std::string& name, float scale, float plane, bool damage)
{

#define sqr(a,b,c,d) a,b,c,c,d,a
    glm::vec3 sp{ scale,scale,plane };

    std::vector<VertexFormat> vertices =
    {
        //WHITE STRIP
        VertexFormat(sp * glm::vec3{0, -1, 1}, {.91,.91,.91}), //0A
        VertexFormat(sp * glm::vec3{-0.65, -0.57, 1}, {.91,.91,.91}), //1B
        VertexFormat(sp * glm::vec3{-0.62, 0.62, 1}, {.91,.91,.91}), //2C
        VertexFormat(sp * glm::vec3{0, 1., 1}, {.91,.91,.91}), //3D
        VertexFormat(sp * glm::vec3{0.54, 1, 1}, {.91,.91,.91}), //4E
        VertexFormat(sp * glm::vec3{0, 0.61, 1}, {.91,.91,.91}), //5F
        VertexFormat(sp * glm::vec3{0, -0.56, 1}, {.91,.91,.91}), //6G
        VertexFormat(sp * glm::vec3{0.54, -0.94, 1}, {.91,.91,.91}), //7H

        //BODY
        VertexFormat(sp * glm::vec3{0.54, 1, 1},{.11,.51,.31}), //8E
        VertexFormat(sp * glm::vec3{0, 0.61, 1}, {.11,.51,.31}), //9F
        VertexFormat(sp * glm::vec3{0, -0.56, 1}, {.11,.51,.31}), //10G
        VertexFormat(sp * glm::vec3{0.54, -0.94, 1}, {.11,.51,.31}), //11H

        VertexFormat(sp * glm::vec3{2, 1, 1}, {.11,.61,.61}), //12I
        VertexFormat(sp * glm::vec3{3.66, 1.4, 1}, {.11,.61,.31}), //13J
        VertexFormat(sp * glm::vec3{5.3, 1, 1}, {.11,.61,.31}), //14K
        VertexFormat(sp * glm::vec3{6.25, 0.4, 1}, {.11,.31,.31}), //15L
        VertexFormat(sp * glm::vec3{6.15, -1, 1}, {.11,.31,.31}), //16M
        VertexFormat(sp * glm::vec3{3.73, -1.4, 1}, {.11,.61,.31}), //17N
        VertexFormat(sp * glm::vec3{1.92, -0.91, 1}, {.11,.61,.31}), //18O
        
        //BEAK
        VertexFormat(sp * glm::vec3{6.25, 0.4, 1}, {.91,.45,.08}), //19L
        VertexFormat(sp * glm::vec3{6.15, -1, 1}, {.91,.45,.08}), //20M

        VertexFormat(sp * glm::vec3{8.29, -0.32, 1}, {.91,.45,.08}), //21P
        VertexFormat(sp * glm::vec3{8.37, -1.17, 1}, {.91,.65,.28}), //22Q
        VertexFormat(sp * glm::vec3{10.14,-0.7, 1}, {.91,.45,.08}), //23R

        //EYES
        VertexFormat(sp * glm::vec3{4.28, 0.54, 1 * .99}, {.05,.05,.05}), //24S
        VertexFormat(sp * glm::vec3{4.77, 0.7, 1 * .99}, {.05,.05,.05}), //25T
        VertexFormat(sp * glm::vec3{5.11, 0.34, 1 * .99}, {.05,.05,.05}), //26U

    };

    Mesh* square = new Mesh(name);


    int rotCol = std::atoi(name.c_str() + 5);
    

    if (rotCol != 0)
    for (int i = 8; i <= 18; i++) {
        vertices[i].color = glm::length(vertices[i].color)* glm::vec3{
            (sin(rotCol + 0)+1) / 2, 
            (sin(rotCol + AI_MATH_TWO_PI / 3) + 1) / 2,
            (sin(rotCol + 2 * AI_MATH_TWO_PI / 3) + 1) / 2
        };
    }

    if (damage)
    {
        for (int i = 0; i < 10; i++)
            vertices[rand() % vertices.size()].color = { .8,0,0 };

    }

    square->SetDrawMode(GL_TRIANGLES);
    std::vector<unsigned int> indices = {
        sqr(0,1,6,7),
        sqr(1,2,5,6),
        sqr(2,3,4,5),

        sqr(10,9,8,11),
        sqr(11,8,12,18),
        sqr(18,12,13,17),
        sqr(17,13,14,16),
        14,15,16,

        sqr(19,21,22,20),
        21,23,22,

        24,25,26

    };

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* objects2D::CreateNeck(const std::string& name, float scale, float plane, bool damage)
{

#define sqr(a,b,c,d) a,b,c,c,d,a
    glm::vec3 sp{ scale,scale,plane };

    std::vector<VertexFormat> vertices =
    {
        //WHITE STRIP
        VertexFormat(sp * glm::vec3{0, -1, 1}, {.91,.91,.91}), //0A
        VertexFormat(sp * glm::vec3{-0.65, -0.57, 1}, {.91,.91,.91}), //1B
        VertexFormat(sp * glm::vec3{-0.62, 0.62, 1}, {.91,.91,.91}), //2C
        VertexFormat(sp * glm::vec3{0, 1., 1}, {.91,.91,.91}), //3D
        VertexFormat(sp * glm::vec3{0.54, 1, 1}, {.91,.91,.91}), //4E
        VertexFormat(sp * glm::vec3{0, 0.61, 1}, {.91,.91,.91}), //5F
        VertexFormat(sp * glm::vec3{0, -0.56, 1}, {.91,.91,.91}), //6G
        VertexFormat(sp * glm::vec3{0.54, -0.94, 1}, {.91,.91,.91}), //7H

        //BODY
        VertexFormat(sp * glm::vec3{0.54, 1, 1},{.11,.51,.31}), //8E
        VertexFormat(sp * glm::vec3{0, 0.61, 1}, {.11,.51,.31}), //9F
        VertexFormat(sp * glm::vec3{0, -0.56, 1}, {.11,.51,.31}), //10G
        VertexFormat(sp * glm::vec3{0.54, -0.94, 1}, {.11,.51,.31}), //11H

        VertexFormat(sp * glm::vec3{2, 1, 1}, {.11,.61,.61}), //12I
        VertexFormat(sp * glm::vec3{3.66, 1.4, 1}, {.11,.61,.31}), //13J
        VertexFormat(sp * glm::vec3{5.3, 1, 1}, {.11,.61,.31}), //14K
        VertexFormat(sp * glm::vec3{10.25, 0.4, 1}, {.11,.31,.31}), //15L
        VertexFormat(sp * glm::vec3{10.15, -1, 1}, {.11,.31,.31}), //16M
        VertexFormat(sp * glm::vec3{3.73, -1.4, 1}, {.11,.61,.31}), //17N
        VertexFormat(sp * glm::vec3{1.92, -0.91, 1}, {.11,.61,.31}), //18O

    };

    Mesh* square = new Mesh(name);


    int rotCol = std::atoi(name.c_str() + 5);


    if (rotCol != 0)
        for (int i = 8; i <= 18; i++) {
            vertices[i].color = glm::length(vertices[i].color) * glm::vec3{
                (sin(rotCol + 0) + 1) / 2,
                (sin(rotCol + AI_MATH_TWO_PI / 3) + 1) / 2,
                (sin(rotCol + 2 * AI_MATH_TWO_PI / 3) + 1) / 2
            };
        }

    if (damage)
    {
        for (int i = 0; i < 10; i++)
            vertices[rand() % vertices.size()].color = { .8,0,0 };

    }

    square->SetDrawMode(GL_TRIANGLES);
    std::vector<unsigned int> indices = {
        sqr(0,1,6,7),
        sqr(1,2,5,6),
        sqr(2,3,4,5),

        sqr(10,9,8,11),
        sqr(11,8,12,18),
        sqr(18,12,13,17),
        sqr(17,13,14,16),
        14,15,16,

        sqr(19,21,22,20),
        21,23,22,

        24,25,26

    };

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateCircle(const std::string& name, float radius, float plane, int Points, glm::vec3 color, bool fill)
{
    

   


    std::vector<VertexFormat> vertices;

    for (float i = 0; i <= AI_MATH_TWO_PI; i += AI_MATH_TWO_PI / Points)
    {
        vertices.push_back({ {cos(i) * radius,sin(i) * radius,plane}, color });
    }

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 0; i < vertices.size(); i++)
    {
        indices.push_back(i);
    }
    
    square->SetDrawMode(GL_TRIANGLE_FAN);
    if (!fill)
    square->SetDrawMode(GL_LINE_LOOP);

  

    square->InitFromData(vertices, indices);
    return square;
}


float randF01() {
    return ((float)rand() / RAND_MAX);
}

Mesh* objects2D::CreateGrass(const std::string& name, float height, float plane, float spacing, int blades)
{



    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < blades; i++)
    {
        vertices.push_back(VertexFormat({ i * spacing                  , 0              ,plane }, { .3,.4,0 }));
        vertices.push_back(VertexFormat({ i * spacing + randF01() * 2 - 1, height/2 * (randF01()*.5 + 1)    ,plane }, { .3,.6,0 }));
        vertices.push_back(VertexFormat({ i * spacing + randF01() * 4 - 2, height * (randF01()*.5 + 1)     ,plane }, { .4,.8,.1 }));
        indices.push_back(i * 3);
        indices.push_back(i * 3 + 1);
        indices.push_back(i * 3 + 1);
        indices.push_back(i * 3 + 2);
    }

    Mesh* square = new Mesh(name);

    

    square->SetDrawMode(GL_LINES);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateBulletTrail(const std::string& name, glm::vec2 start, glm::vec2 end, float plane, bool hit)
{



    std::vector<VertexFormat> vertices = {
        {{start.x,start.y,plane},{1,1,1}},
        {{end.x,end.y,plane},{1,1,0}},
    };
    std::vector<unsigned int> indices = { 0,1 };
    if (hit) {
        glm::vec2 end2 = end * 2.f - start;
        vertices.push_back({ {end.x,end.y,-50}, {1,1,0} });
        vertices.push_back({ {end2.x,end2.y,-50}, {.8, .8, .85} });
        indices.push_back(2);
        indices.push_back(3);
    }
    Mesh* square = new Mesh(name);
    


    square->SetDrawMode(GL_LINES);




    square->InitFromData(vertices, indices);
    return square;
}


Mesh* objects2D::CreateBullet(const std::string& name,float planee)
{


    glm::vec3 plane{ 0,0,planee };

    std::vector<VertexFormat> vertices = {
        { glm::vec3{-45,8,0} + plane, {0.705,0.72,-0.185} },
        { glm::vec3{45,8,0} + plane, {0.705,0.72,-0.185} },
        { glm::vec3{-46,28,0} + plane, {0.705,0.72,-0.185} },
        { glm::vec3{46,28,0} + plane, {0.705,0.72,-0.185} },
        { glm::vec3{-40,28,0} + plane, {0.54,0.535,0.525} },
        { glm::vec3{40,28,0} + plane, {0.54,0.535,0.525} },
        { glm::vec3{-38,40,0} + plane, {0.54,0.535,0.525} },
        { glm::vec3{38,40,0} + plane, {0.54,0.535,0.525} },
        { glm::vec3{50,188,0} + plane, {0.895,0.86,0.345} },
        { glm::vec3{-50,188,0} + plane, {0.705,0.72,-0.185} },
        { glm::vec3{-50,40,0} + plane, {0.54,0.535,-0.185} },
        { glm::vec3{50,40,0} + plane, {0.895,0.86,-0.185} },
    };
    

    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_TRIANGLES);
    std::vector<unsigned int> indices = {
        2,3,1,
        2,1,0,
        4,6,5,
        6,7,5,
        10,11,8,
        10,8,9,
    };

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateHeart(const std::string& name, float planee)
{

    glm::vec3 plane{ 0,0,planee };

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices = {
        { glm::vec3{-74,0,0} + plane , {0.245,-0.44,-0.17} },
        { glm::vec3{-74,47,0} + plane , {1,-0.135,-0.005} },
        { glm::vec3{-51,60,0} + plane , {1,-0.135,-0.005} },
        { glm::vec3{-1,19,0} + plane , {1,-0.135,-0.005} },
        { glm::vec3{52,60,0} + plane , {1,0.44,0.355} },
        { glm::vec3{70,47,0} + plane , {1,0.44,0.355} },
        { glm::vec3{70,0,0} + plane , {1,0.44,0.355} },
        { glm::vec3{42,-36,0} + plane , {1,-0.135,-0.005} },
        { glm::vec3{1,-71,0} + plane , {0.245,-0.44,-0.17} },
        { glm::vec3{-42,-36,0} + plane , {0.245,-0.44,-0.17} },
        { glm::vec3{-16,47,0} + plane , {1,-0.135,-0.005} },
        { glm::vec3{16,47,0} + plane , {1,0.44,0.355} },
    };


    indices = {
    3,2,1,
    3,1,0,
    3,0,9,
    3,9,8,
    3,8,7,
    3,7,6,
    3,6,5,
    3,5,4,
    3,10,2,
    3,11,4,
    };


    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_TRIANGLES);
    

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateMuzzleFlash(const std::string& name, float planee)
{

    glm::vec3 plane{ 0,0,planee };

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices = {
    { plane + glm::vec3{-11,0,0} , {1,1,1} },
    { plane + glm::vec3{9,1,0} , {1,1,1} },
    { plane + glm::vec3{-20,48,0} , {1,1,-0.725} },
    { plane + glm::vec3{-10,68,0} , {1,1,-0.725} },
    { plane + glm::vec3{9,68,0} , {1,1,-0.725} },
    { plane + glm::vec3{19,47,0} , {1,1,-0.725} },
    };


    indices = {
    1,0,3,
    1,3,4,
    1,4,5,
    0,2,3,
    };


    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_TRIANGLES);


    square->InitFromData(vertices, indices);
    return square;
}
Mesh* objects2D::CreateFeather(const std::string& name, float planee)
{

    glm::vec3 plane{ 0,0,planee };

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices = {
    { glm::vec3{-16,-11,0} + planee, {0.605,0.51,0.345} },
    { glm::vec3{-21,60,0} + planee, {0.88,0.845,0.835} },
    { glm::vec3{-15,59,0} + planee, {0.605,0.51,0.345} },
    { glm::vec3{-21,-34,0} + planee, {0.88,0.845,0.835} },
    { glm::vec3{-3,40,0} + planee, {0.605,0.51,0.345} },
    { glm::vec3{-3,0,0} + planee, {0.71,0.675,0.615} },
    };


    indices = {
    0,1,2,
    3,1,0,
    0,5,4,
    0,4,2,
    };


    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_TRIANGLES);


    square->InitFromData(vertices, indices);
    return square;
}

