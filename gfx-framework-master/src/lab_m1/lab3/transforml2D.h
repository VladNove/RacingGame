#pragma once

#include "utils/glm_utils.h"


namespace transforml2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        
        glm::mat3 mat = glm::identity<glm::mat3>();
  
        mat[2][0] = translateX;
        mat[2][1] = translateY;

        return mat;

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix



        glm::mat3 mat = glm::identity<glm::mat3>();

        mat[0][0] = scaleX;
        mat[1][1] = scaleY;

        return mat;

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix

        glm::mat3 mat = glm::identity<glm::mat3>();

        mat[0][0] = cos(radians);
        mat[0][1] = -sin(radians);
        mat[1][0] = sin(radians);
        mat[1][1] = cos(radians);

        return mat;

    }
}   // namespace transform2D
