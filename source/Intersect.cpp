//
// Created by Timothée Couble on 03/03/2020.
//

#include "Intersect.hpp"

bool scene::Intersect::squares(const glm::vec3 &aMin, const glm::vec3 &aMax,
        const glm::vec3 &bMin, const glm::vec3 &bMax) {
    return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z && aMax.z >= bMin.z);
}

bool scene::Intersect::spheres(const glm::vec3 &aSphere, const float &aRadius,
        const glm::vec3 &bSphere, const float &bRadius) {
    float distance = std::sqrt(
            (aSphere.x - bSphere.x) * (aSphere.x - bSphere.x) +
            (aSphere.y - bSphere.y) * (aSphere.y - bSphere.y) +
            (aSphere.z - bSphere.z) * (aSphere.z - bSphere.z));

    return distance < (aRadius + bRadius);
}

bool scene::Intersect::sphereSquare(const glm::vec3 &aMin, const glm::vec3 &aMax,
        const glm::vec3 &sphere, const float &radius) {
    float x = std::max(aMin.x, std::min(sphere.x, aMax.x));
    float y = std::max(aMin.y, std::min(sphere.y, aMax.y));
    float z = std::max(aMin.z, std::min(sphere.z, aMax.z));

    float distance = std::sqrt(
            (x - sphere.x) * (x - sphere.x) +
            (y - sphere.y) * (y - sphere.y) +
            (z - sphere.z) * (z - sphere.z));

    return distance < radius;
}

scene::normal_collision_t scene::Intersect::vectorNormal(const glm::vec3 &aCenter, const glm::vec3 &aSize,
        const glm::vec3 &bCenter) {
    const glm::vec2 c(bCenter.x, bCenter.z);
    const glm::vec2 r(aCenter.x, aCenter.z);
    const glm::vec2 e(-aSize.x / 2, -aSize.z / 2);
    const glm::vec2 ux(1.0f, 0.0f);
    const glm::vec2 uy(0.0f, 1.0f);
    const glm::vec2 diff = c - r;

    // project d onto ux to get distance along ux from c
    float dx = glm::dot(diff, ux);
    if (dx > e.x)
        dx = e.x;
    if (dx < -e.x)
        dx = -e.x;

    // project d onto uy to get distance along uy from c
    float dy = glm::dot(diff, uy);
    if (dy > e.y)
        dy = e.y;
    if (dy < -e.y)
        dy = -e.y;

    // calculate closest point p on box to c
    glm::vec2 p = r + dx * ux + dy * uy;
    glm::vec2 result = glm::normalize(c - p);

    glm::vec3 dist(0.0f);
    if (std::round(result.x) == 1)
        dist.x = c.x - r.x - e.x;
    else if (std::round(result.x) == -1)
        dist.x = c.x - r.x + e.x;
    else if (std::round(result.y) == 1)
        dist.z = c.y - r.y - e.y;
    else if (std::round(result.y) == -1)
        dist.z = c.y - r.y + e.y;
    else
        throw std::runtime_error("Intersection error");

    return std::make_tuple(true, glm::vec3(std::round(result.x), 0.0f, std::round(result.y)), dist);
}
