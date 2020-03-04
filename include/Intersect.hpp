//
// Created by Timothée Couble on 17/10/2019.
//

#ifndef INTERSECT_HPP
# define INTERSECT_HPP

#include <iostream>
#include <cmath>

#include "glm/glm.hpp"

#include "Misc.hpp"

namespace scene {
    class Intersect {
    public:
        static bool squares(const glm::vec3 &aMin, const glm::vec3 &aMax,
                              const glm::vec3 &bMin, const glm::vec3 &bMax);
        static bool spheres(const glm::vec3 &aSphere, const float &aRadius,
                            const glm::vec3 &bSphere, const float &bRadius);
        static bool sphereSquare(const glm::vec3 &aMin, const glm::vec3 &aMax,
                            const glm::vec3 &sphere, const float &radius);

        static normal_collision_t vectorNormal(const glm::vec3 &aCenter, const glm::vec3 &aSize,
                const glm::vec3 &bCenter);
    };
}

#endif /* !MISC_HPP */
