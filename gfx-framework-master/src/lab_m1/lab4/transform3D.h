#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        // TODO(student): Implement the translation matrix
        glm::mat4 r = glm::identity<glm::mat4>();
        r[3][0] = translateX;
        r[3][1] = translateY;
        r[3][2] = translateZ;


        return r;

    }

    // Scale matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        glm::mat4 r = glm::identity<glm::mat4>();
        r[0][0] = scaleX;
        r[1][1] = scaleY;
        r[2][2] = scaleZ;


        return r;

    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        glm::mat4 r = glm::identity<glm::mat4>();
        r[0][0] = cos(radians);
        r[1][1] = cos(radians);
        r[1][0] = -sin(radians);
        r[0][1] = sin(radians);

        return r;

    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        glm::mat4 r = glm::identity<glm::mat4>();
        r[0][0] = cos(radians);
        r[2][2] = cos(radians);
        r[2][0] = -sin(radians);
        r[0][2] = sin(radians);

        return r;

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        glm::mat4 r = glm::identity<glm::mat4>();
        r[1][1] = cos(radians);
        r[2][2] = cos(radians);
        r[2][1] = -sin(radians);
        r[1][2] = sin(radians);

        return r;


    }
}   // namespace transform3D
